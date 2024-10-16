#ifndef STAITON_H
#define STAITON_H

#include <memory>
#include <string>
#include <vector>
#include <memory>

#include "roles.h"
#include "ssh_connection.h"

struct AdditionalTask {};

class Station {
   public:
    explicit Station(std::string host_name, std::string name);
    explicit Station(const std::string host_name, const std::string name,
                     ConnectionSettings&& settings);

    virtual ~Station() = default;

    Station(const Station&) = delete;
    Station& operator=(const Station&) = delete;

    Station(Station&&) = default;
    Station& operator=(Station&&) = default;

    bool SetHostName(std::string host_name);
    bool SetName(std::string name);
    bool SetDescription(std::string description);
    void SetUsername(std::string username);
    void SetPassword(std::string password);
    void SetConnectionPort(quint16 port);
    void SetPath(std::filesystem::path path_to_private_key);
    void SetRole(Role role);
    void AddAdditionalTask(AdditionalTask task);

    const std::string& GetName() const { return name_; }
    const std::string& GetDescription() const { return description_; }
    const std::string& GetHostName() const {
        return ssh_connection_->GetHostName();
    }
    const ConnectionSettings& GetSettings() const {
        return ssh_connection_->GetSettings();
    }
    Role GetRole() const { return role_; }
    const std::vector<AdditionalTask>& GetAdditionalTasks() const {
        return additional_tasks_;
    }

    bool CheckConnection() const;
    void StartSetupProccess();

   private:
    const int MAX_NAME_SYMBOLS_COUNT = 100;
    const int MAX_DESCRIPTION_SYMBOLS_COUNT = 256;

    std::unique_ptr<SSHConnection> ssh_connection_;
    mutable bool is_connected = false;

    std::string name_;
    std::string description_;
    Role role_;
    std::vector<AdditionalTask> additional_tasks_;
};

class MainStation : public Station {
   public:
    using Station::Station;

    std::vector<Station>& GetChildStations() { return child_stations_; }
    void AddChildStation(Station&& station);

    // void StartSetupProccessAllStation();

   private:
    std::vector<Station> child_stations_;
};

#endif
