#ifndef STAITON_H
#define STAITON_H

#include <qglobal.h>

#include <QList>
#include <QUrl>
#include <QVector>
#include <memory>

#include "roles.h"
#include "ssh_connection.h"
#include "systems.h"

struct AdditionalTask {};

enum class SystemType { WINDOWS = 0, ASTRALINUX = 1, REDOS7 = 2, REDOS8 = 3 };

class Station : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString hostName READ GetHostName WRITE SetHostName)
    Q_PROPERTY(QString name READ GetName WRITE SetName)
    Q_PROPERTY(QString description READ GetDescription WRITE SetDescription)
    Q_PROPERTY(Roles::Role role READ GetRole WRITE SetRole)
    Q_PROPERTY(QVector<AdditionalTask> additionalTasks READ GetAdditionalTasks)

    Q_PROPERTY(QString username READ GetUsername WRITE SetUsername)
    Q_PROPERTY(QString password READ GetPassword WRITE SetPassword)
    Q_PROPERTY(quint16 port READ GetConnectionPort WRITE SetConnectionPort)
    Q_PROPERTY(QString filePath READ GetUrlPath WRITE SetPath)

   public:
    explicit Station(const QString host_name, const QString name,
                     ConnectionSettings settings, QObject* parent = nullptr);

    virtual ~Station() = default;

    Station(const Station&) = delete;
    Station& operator=(const Station&) = delete;
    Station(Station&&) = delete;
    Station& operator=(Station&&) = delete;

    Q_INVOKABLE bool SetHostName(QString host_name);
    Q_INVOKABLE bool SetName(QString name);
    Q_INVOKABLE bool SetDescription(QString description);
    Q_INVOKABLE void SetUsername(QString username);
    Q_INVOKABLE void SetPassword(QString password);
    Q_INVOKABLE void SetConnectionPort(quint16 port);
    Q_INVOKABLE void SetPath(QString path);
    Q_INVOKABLE void SetRole(Roles::Role role);
    Q_INVOKABLE void AddAdditionalTask(AdditionalTask task);

    QString GetName() const { return name_; }
    QString GetDescription() const { return description_; }
    QString GetHostName() const { return ssh_connection_->GetHostName(); }
    QString GetUsername() const {
        return ssh_connection_->GetSettings().username;
    }
    QString GetPassword() const {
        return ssh_connection_->GetSettings().password;
    }
    quint16 GetConnectionPort() const {
        return ssh_connection_->GetSettings().port;
    }
    QString GetUrlPath() const {
        if (ssh_connection_->GetSettings().path_to_private_key.has_value()) {
            return QString::fromStdString(
                ssh_connection_->GetSettings().path_to_private_key.value());
        }
        return "";
    }
    std::optional<std::filesystem::path> GetPath() const {
        return ssh_connection_->GetSettings().path_to_private_key;
    }
    Roles::Role GetRole() const { return role_; }
    QList<AdditionalTask> GetAdditionalTasks() const {
        return additional_tasks_;
    }

    Systems::System CheckSystem();

    std::optional<Systems::System> GetSystem() const { return system_; }

        Q_INVOKABLE bool CheckConnection() const;
    Q_INVOKABLE void StartSetupProccess();

   private:
    const int MAX_NAME_SYMBOLS_COUNT = 100;
    const int MAX_DESCRIPTION_SYMBOLS_COUNT = 256;

    std::unique_ptr<SSHConnection> ssh_connection_;
    mutable bool is_connected = false;

    QString name_;
    QString description_;
    Roles::Role role_;
    QList<AdditionalTask> additional_tasks_;
    std::optional<Systems::System> system_;
};

class MainStation : public Station {
   public:
    using Station::Station;

    std::vector<std::unique_ptr<Station>>& GetChildStations() {
        return child_stations_;
    }
    void AddChildStation(std::unique_ptr<Station> station);
    void RemoveChildStation(int index);

    void StartSetupProccessAllStation();

   private:
    std::vector<std::unique_ptr<Station>> child_stations_;
};

#endif
