#include "station.h"

#include <algorithm>

Station::Station(const ConnectionSettings& settings)
    : ssh_connection_(std::make_unique<SSHConnection>(settings)),
      settings_(settings) {}

Station::Station(ConnectionSettings&& settings)
    : ssh_connection_(std::make_unique<SSHConnection>(std::move(settings))),
      settings_(settings) {}

bool Station::SetName(std::string name) {
    if (!std::all_of(name.begin(), name.end(), [](char c) {
            return std::isdigit(c) || std::isalpha(c);
        })) {
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

MainStation::MainStation(const ConnectionSettings& settings)
    : Station(settings) {}

MainStation::MainStation(ConnectionSettings&& settings)
    : Station(std::move(settings)) {}

// void MainStation::AddChildStation(Station& station) {
//     child_stations_.push_back(station);
// }

// void MainStation::StartSetupProccessAllStation() {
//     StartSetupProccess();
//     for (auto& station : child_stations_) {
//         station.StartSetupProccess();
//     }
// }
