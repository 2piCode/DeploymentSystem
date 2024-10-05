#include "station.h"

#include <algorithm>

Station::Station(ConnectionSettings settings)
    : ssh_connection_(std::make_unique<SSHConnection>(settings)) {}

bool Station::SetName(std::string name) {
    if (!std::all_of(name.begin(), name.end(), [](char c) {
            return std::isdigit(c) || std::isalpha(c);
        })) {
        return false;
    }

    this->name = name;
    return true;
}

bool Station::SetDescription(std::string description) {
    if (description.length() > MAX_DESCRIPTION_SYMBOLS_COUNT) {
        return false;
    }

    this->description = description;
    return true;
}

void Station::SetRole(Role role) { this->role = role; }

void Station::AddAdditionalTask(AdditionalTask task) {
    additional_tasks.push_back(task);
}

bool Station::CheckConnection() const {
    return ssh_connection_->ConnectToHost();
}

bool Station::ExecuteCommand(const std::string& command) {
    return ssh_connection_->ExecuteCommand(command);
}

bool Station::UploadFile(const std::filesystem::path& local_path,
                         const std::filesystem::path& remote_path) {
    return ssh_connection_->UploadFile(local_path, remote_path);
}
