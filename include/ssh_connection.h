#ifndef SSHCONNECTION_H
#define SSHCONNECTION_H

#include <libssh2.h>
#include <libssh2_sftp.h>

#include <QHostAddress>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <filesystem>
#include <string>

#include "libssh2_manager.h"

struct ConnectionSettings {
    std::string hostname;
    quint16 port = 22;
    std::string username;
    std::string password;
    std::filesystem::path path_to_private_key;
};

class SSHConnection : public QObject {
    Q_OBJECT
   public:
    explicit SSHConnection(const ConnectionSettings& config, QObject* parent = nullptr);
    ~SSHConnection() final;

    bool connectToHost();
    void disconnectFromHost();

    bool executeCommand(const std::string& command);
    bool uploadFile(const std::filesystem::path& local_path,
                    const std::filesystem::path& remote_path);

   signals:
    void connected();
    void disconnected();
    void commandExecuted(const std::string& output);
    void errorOccurred(const std::string& errorMessage);

   private:
    const int CONNECTION_TIME = 5000;     // 5 seconds
    const int DISCONNECTION_TIME = 3000;  // 3 seconds

    ConnectionSettings config_;

    LIBSSH2_SESSION* session_;
    LIBSSH2_SFTP* sftp_session_;

    QTcpSocket* socket_;

    bool authenticate();
    bool initializeSFTP();
};

#endif
