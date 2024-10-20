#include "station.h"

#include <algorithm>
Station::Station(std::string host_name, std::string name)
    : ssh_connection_(std::make_unique<SSHConnection>(host_name)),
      name_(name) {}

Station::Station(std::string host_name, std::string name,
                 ConnectionSettings&& settings)
    : ssh_connection_(
          std::make_unique<SSHConnection>(host_name, std::move(settings))),
      name_(name) {}

bool Station::SetHostName(std::string host_name) {
    ssh_connection_->SetHostName(host_name);
    is_connected = false;
    return true;
}

bool Station::SetName(std::string name) {
    if (name.length() > MAX_NAME_SYMBOLS_COUNT) {
        return false;
    }

    this->name_ = name;
    return true;
}

bool Station::SetDescription(std::string description) {
    if (description.length() > MAX_DESCRIPTION_SYMBOLS_COUNT) {
        return false;
    }

    this->description_ = description;
    return true;
}

void Station::SetUsername(std::string username) {
    ssh_connection_->GetSettings().username = username;
    is_connected = false;
}

void Station::SetPassword(std::string password) {
    ssh_connection_->GetSettings().password = password;
    is_connected = false;
}

void Station::SetConnectionPort(quint16 port) {
    ssh_connection_->GetSettings().port = port;
    is_connected = false;
}

void Station::SetPath(std::filesystem::path path_to_private_key) {
    ssh_connection_->GetSettings().path_to_private_key = path_to_private_key;
}

void Station::SetRole(Role role) { this->role_ = role; }

void Station::AddAdditionalTask(AdditionalTask task) {
    additional_tasks_.push_back(task);
}

bool Station::CheckConnection() const {
    if (!is_connected) {
        is_connected = ssh_connection_->ConnectToHost();
    }
    return is_connected;
}

void Station::StartSetupProccess() {
    if (!CheckConnection()) {
        return;
    }

    /*
    TODO(coder): Add setup proccess:
     1) Define system
     2) Upload installation app
     3) Start installation proccess by execute command
     4) Proccess additional tasks
    */
    throw std::runtime_error("Not implemented");
}

void MainStation::AddChildStation(Station&& station) {
    child_stations_.push_back(std::move(station));
}

void MainStation::StartSetupProccessAllStation() {
    StartSetupProccess();
    for (auto& station : child_stations_) {
        station.StartSetupProccess();
    }
}
