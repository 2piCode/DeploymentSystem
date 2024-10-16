#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <libssh2.h>
#include <libssh2_sftp.h>

#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <filesystem>
#include <optional>
#include <string>

#include "libssh2_manager.h"

struct ConnectionSettings {
    quint16 port = 22;
    std::string username;
    std::string password;
    std::optional<std::filesystem::path> path_to_private_key;
};

class SSHConnection : public QObject {
    Q_OBJECT
   public:
    explicit SSHConnection(std::string hostname, QObject* parent = nullptr);
    explicit SSHConnection(std::string hostname, ConnectionSettings&& settings,
                           QObject* parent = nullptr);

    ~SSHConnection() final;

    bool ConnectToHost();
    void DisconnectFromHost();

    bool ExecuteCommand(const std::string& command) const;
    bool UploadFile(const std::filesystem::path& local_path,
                    const std::filesystem::path& remote_path) const;

    void SetHostName(std::string host_name);

    const std::string& GetHostName() const { return host_name_; }
    const ConnectionSettings& GetSettings() const { return connection_settings_; }
    ConnectionSettings& GetSettings() { return connection_settings_; }

   signals:
    void connected();
    void disconnected();
    void commandExecuted(const std::string& output) const;
    void errorOccurred(const std::string& errorMessage) const;

   private:
    const int CONNECTION_TIME = 5000;     // 5 seconds
    const int DISCONNECTION_TIME = 3000;  // 3 seconds

    std::string host_name_;
    ConnectionSettings connection_settings_;

    LIBSSH2_SESSION* session_;
    LIBSSH2_SFTP* sftp_session_;

    QTcpSocket* socket_;

    bool authenticate();
    bool initializeSFTP();
};

#endif
