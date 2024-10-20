#include "ssh_connection.h"

#include <QDebug>
#include <QFile>

SSHConnection::SSHConnection(std::string hostname, QObject* parent)
    : SSHConnection(hostname, ConnectionSettings{}, parent) {}

SSHConnection::SSHConnection(std::string host_name,
                             ConnectionSettings&& connection_settings,
                             QObject* parent)
    : QObject(parent),
      host_name_(host_name),
      connection_settings_(connection_settings),
      session_(nullptr),
      sftp_session_(nullptr),
      socket_(new QTcpSocket(this)) {
    connect(socket_, &QTcpSocket::connected, this,
            []() { qDebug() << "TCP Socket connected"; });

    connect(socket_, &QTcpSocket::disconnected, this, [this]() {
        qDebug() << "TCP Socket disconnected";
        emit disconnected();
    });

    connect(socket_,
            QOverload<QAbstractSocket::SocketError>::of(
                &QAbstractSocket::errorOccurred),
            this, [this](QAbstractSocket::SocketError socker_error) {
                Q_UNUSED(socker_error)
                emit errorOccurred(socket_->errorString().toStdString());
            });
}

SSHConnection::~SSHConnection() {
    DisconnectFromHost();
    delete socket_;
}

bool SSHConnection::ConnectToHost() {
    if (!LibSSH2Manager::instance().initialize()) {
        emit errorOccurred("Failed to initialize libssh2");
        return false;
    }

    if (host_name_.empty()) {
        emit errorOccurred("Hostname is empty");
        return false;
    }

    socket_->connectToHost(QString::fromStdString(host_name_),
                           connection_settings_.port);
    if (!socket_->waitForConnected(CONNECTION_TIME)) {
        emit errorOccurred("Failed to connect to host: " +
                           socket_->errorString().toStdString());
        return false;
    }

    int sock = socket_->socketDescriptor();
    if (sock == -1) {
        emit errorOccurred("Invalid socket descriptor");
        return false;
    }

    session_ = libssh2_session_init();
    if (!session_) {
        emit errorOccurred("Failed to create libssh2 session");
        return false;
    }

    if (int rc = libssh2_session_handshake(session_, sock)) {
        emit errorOccurred("Failure establishing SSH session: " + rc);
        return false;
    }

    if (!authenticate()) {
        emit errorOccurred("Authentication failed");
        return false;
    }

    if (!initializeSFTP()) {
        emit errorOccurred("Failed to initialize SFTP session");
        return false;
    }

    emit connected();
    return true;
}

void SSHConnection::DisconnectFromHost() {
    if (sftp_session_) {
        libssh2_sftp_shutdown(sftp_session_);
        sftp_session_ = nullptr;
    }

    if (session_) {
        // TODO(coder): What is the description message?
        libssh2_session_disconnect(session_, "Normal Shutdown");
        libssh2_session_free(session_);
        session_ = nullptr;
    }

    if (socket_->isOpen()) {
        socket_->disconnectFromHost();
        if (socket_->state() != QAbstractSocket::UnconnectedState) {
            socket_->waitForDisconnected(DISCONNECTION_TIME);
        }
    }

    emit disconnected();
}

bool SSHConnection::authenticate() {
    if (!connection_settings_.password.empty()) {
        if (libssh2_userauth_password(session_,
                                      connection_settings_.username.c_str(),
                                      connection_settings_.password.c_str())) {
            emit errorOccurred("Authentication by password failed");
            return false;
        }
        return true;
    }

    if (connection_settings_.path_to_private_key.has_value() ||
        !connection_settings_.path_to_private_key.value().empty()) {
        int rc = libssh2_userauth_publickey_fromfile(
            session_, connection_settings_.username.c_str(),
            nullptr,  // Публичный ключ можно определить автоматически
            connection_settings_.path_to_private_key.value().string().c_str(),
            nullptr  // Пароль для ключа, если требуется
        );
        if (rc != 0) {
            emit errorOccurred("Authentication by public key failed");
            return false;
        }
        return true;
    }

    emit errorOccurred("No authentication method provided");
    return false;
}

bool SSHConnection::initializeSFTP() {
    sftp_session_ = libssh2_sftp_init(session_);
    if (!sftp_session_) {
        emit errorOccurred("Unable to init SFTP session");
        return false;
    }
    return true;
}

bool SSHConnection::ExecuteCommand(const std::string& command) const {
    if (!session_) {
        emit errorOccurred("Not connected to any SSH session");
        return false;
    }

    LIBSSH2_CHANNEL* channel = libssh2_channel_open_session(session_);
    if (!channel) {
        emit errorOccurred("Unable to open channel.");
        return false;
    }

    if (int rc = libssh2_channel_exec(channel, command.c_str()); rc != 0) {
        libssh2_channel_close(channel);
        libssh2_channel_free(channel);
        emit errorOccurred("Unable to execute command: " + rc);
        return false;
    }

    std::string output;
    char buffer[1024];
    ssize_t bytesRead;
    while ((bytesRead = libssh2_channel_read(channel, buffer, sizeof(buffer))) >
           0) {
        output.append(std::string(buffer, bytesRead));
    }

    libssh2_channel_close(channel);
    libssh2_channel_free(channel);

    if (bytesRead < 0) {
        emit commandExecuted(output);
        emit errorOccurred("Error reading command output");
        return false;
    }

    emit commandExecuted(output);
    return true;
}

bool SSHConnection::UploadFile(const std::filesystem::path& local_path,
                               const std::filesystem::path& remote_path) const {
    if (!sftp_session_) {
        emit errorOccurred("SFTP session not initialized");
        return false;
    }

    QFile local_file(local_path);
    if (!local_file.open(QIODevice::ReadOnly)) {
        emit errorOccurred("Failed to open local file: " + local_path.string());
        return false;
    }

    LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_open(
        sftp_session_, remote_path.string().c_str(),
        LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,
        LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | LIBSSH2_SFTP_S_IRGRP |
            LIBSSH2_SFTP_S_IROTH);

    if (!sftp_handle) {
        emit errorOccurred("Unable to open remote file: " +
                           remote_path.string());
        local_file.close();
        return false;
    }

    QByteArray data;
    bool success = true;
    while (!local_file.atEnd()) {
        data = local_file.read(1024);
        ssize_t rc =
            libssh2_sftp_write(sftp_handle, data.constData(), data.size());
        if (rc < 0) {
            emit errorOccurred("Error writing to remote file");
            success = false;
            break;
        }
    }

    libssh2_sftp_close(sftp_handle);
    local_file.close();

    return success;
}

void SSHConnection::SetHostName(std::string host_name) {
    host_name_ = host_name;
}
