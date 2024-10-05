#ifndef STAITON_H
#define STAITON_H

#include <string>
#include <vector>

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
    explicit Station(ConnectionSettings settings);

    bool SetName(std::string name);
    bool SetDescription(std::string description);
    void SetRole(Role role);
    void AddAdditionalTask(AdditionalTask task);

    const std::string& GetName() const { return name; }
    const std::string& GetDescription() const { return description; }
    const ConnectionSettings& GetSettings() const { return settings; }
    Role GetRole() const { return role; }
    const std::vector<AdditionalTask>& GetAdditionalTasks() const {
        return additional_tasks;
    }

    bool CheckConnection() const;
    bool ExecuteCommand(const std::string& command);
    bool UploadFile(const std::filesystem::path& local_path,
                    const std::filesystem::path& remote_path);

   private:
    const int MAX_DESCRIPTION_SYMBOLS_COUNT = 100;

    std::string name;
    std::string description;
    ConnectionSettings settings;
    Role role;
    std::vector<AdditionalTask> additional_tasks;

    std::unique_ptr<SSHConnection> ssh_connection_;
};

#endif
