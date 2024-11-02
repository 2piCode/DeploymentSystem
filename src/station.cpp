#include "station.h"

#include <qforeach.h>

Station::Station(QString host_name, QString name, ConnectionSettings settings,
                 QObject* parent)
    : QObject(parent),
      ssh_connection_(
          std::make_unique<SSHConnection>(host_name, std::move(settings))),
      name_(name) {}

bool Station::SetHostName(QString host_name) {
    ssh_connection_->SetHostName(host_name);
    is_connected = false;
    return true;
}

bool Station::SetName(QString name) {
    if (name.length() > MAX_NAME_SYMBOLS_COUNT) {
        return false;
    }

    this->name_ = name;
    return true;
}

bool Station::SetDescription(QString description) {
    if (description.length() > MAX_DESCRIPTION_SYMBOLS_COUNT) {
        return false;
    }

    this->description_ = description;
    return true;
}

void Station::SetUsername(QString username) {
    ssh_connection_->GetSettings().username = username;
    is_connected = false;
}

void Station::SetPassword(QString password) {
    ssh_connection_->GetSettings().password = password;
    is_connected = false;
}

void Station::SetConnectionPort(quint16 port) {
    ssh_connection_->GetSettings().port = port;
    is_connected = false;
}

void Station::SetPath(QString path) {
    ssh_connection_->GetSettings().path_to_private_key =
        std::filesystem::path(path.toStdString());
}

void Station::SetRole(Roles::Role role) { this->role_ = role; }

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

void MainStation::AddChildStation(std::unique_ptr<Station> station) {
    child_stations_.emplace_back(std::move(station));
}

void MainStation::RemoveChildStation(int index) {
    if (index < 0 || index >= child_stations_.size()) {
        return;
    }

    child_stations_.erase(child_stations_.begin() + index);
}

void MainStation::StartSetupProccessAllStation() {
    StartSetupProccess();
    for (auto& station : child_stations_) {
        station->StartSetupProccess();
    }
}
