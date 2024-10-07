#ifndef STAITON_H
#define STAITON_H

#include <string>
#include <vector>
#include <memory>

#include "ssh_connection.h"

enum class Role {
    arm_engineer = 0,
    arm_operator = 1,
    arm_kip = 2,
    input_output_server = 3,
    history_server = 4,
    integration_server = 5
};

struct AdditionalTask {};

class Station {
   public:
    explicit Station(const ConnectionSettings& settings);
    explicit Station(ConnectionSettings&& settings);

    bool SetName(std::string name);
    bool SetDescription(std::string description);
    void SetRole(Role role);
    void AddAdditionalTask(AdditionalTask task);

    const std::string& GetName() const { return name_; }
    const std::string& GetDescription() const { return description_; }
    const ConnectionSettings& GetSettings() const { return settings_; }
    Role GetRole() const { return role_; }
    const std::vector<AdditionalTask>& GetAdditionalTasks() const {
        return additional_tasks_;
    }

    bool CheckConnection() const;

    void StartSetupProccess();

   private:
    const int MAX_DESCRIPTION_SYMBOLS_COUNT = 100;

    std::unique_ptr<SSHConnection> ssh_connection_;
    mutable bool is_connected = false;

    std::string name_;
    std::string description_;
    ConnectionSettings settings_;
    Role role_;
    std::vector<AdditionalTask> additional_tasks_;
};

class MainStation : public Station {
   public:
    explicit MainStation(const ConnectionSettings& settings);
    explicit MainStation(ConnectionSettings&& settings);

    void AddChildStation(Station&& station);

    void StartSetupProccessAllStation();

   private:
    std::vector<Station> child_stations_;
};

#endif
