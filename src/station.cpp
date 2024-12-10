#include "station.h"

#include <qforeach.h>
#include <QDebug>

Station::Station(QString host_name, QString name, ConnectionSettings settings, Roles::Role role,
                 QObject* parent)
    : QObject(parent),
      ssh_connection_(std::make_unique<SSHConnection>(host_name, std::move(settings))),
      name_(name),
      role_(role) {}

bool Station::SetHostName(QString host_name) {
    ssh_connection_->SetHostName(host_name);
    is_connected = false;
    emit hostNameChanged();
    return true;
}

bool Station::SetName(QString name) {
    if (name.length() > MAX_NAME_SYMBOLS_COUNT) {
        return false;
    }

    this->name_ = name;
    emit nameChanged();
    return true;
}

bool Station::SetDescription(QString description) {
    if (description.length() > MAX_DESCRIPTION_SYMBOLS_COUNT) {
        return false;
    }

    this->description_ = description;
    emit descriptionChanged(); 
    return true;
}

void Station::SetUsername(QString username) {
    ssh_connection_->GetSettings().username = username;
    is_connected = false;
    emit usernameChanged();
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

void Station::SetRole(Roles::Role role) { 
    this->role_ = role;
    emit roleChanged();
}

void Station::AddAdditionalTask(AdditionalTask task) {
    additional_tasks_.push_back(task);
}

bool Station::CheckConnection() const {
    qDebug() << "CheckConnection invoked"; 
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
    //FIXME
    std::ostringstream outputStream;
    ssh_connection_->ExecuteCommand("touch justCreatedFile", outputStream);
    // ssh_connection_->UploadFile(config);
    
    // throw std::runtime_error("Not implemented");
}

void MainStation::AddChildStation(std::unique_ptr<Station> station) {
    child_stations_.emplace_back(std::move(station));
}

void MainStation::RemoveChildStation(int index) {
    if (index < 0 || index >= static_cast<int>(child_stations_.size())) {
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

Systems::System Station::CheckSystem() {
    if (system_.has_value()) {
        return system_.value();
    }

    std::ostringstream outputStream;

    if (ssh_connection_->ExecuteCommand("ver", outputStream)) {
        std::string output = outputStream.str();
        if (output.find("Windows") != std::string::npos) {
            system_ = Systems::System::Windows;
            return Systems::System::Windows;
        }
    }

    if (ssh_connection_->ExecuteCommand("lsb_release -a | grep Description",
                                       outputStream)) {
        std::string output = outputStream.str();
        if (output.find("Astra Linux") != std::string::npos) {
            system_ = Systems::System::AstraLinux;
            return Systems::System::AstraLinux;
        } else if (output.find("RED OS") != std::string::npos) {
            if (output.find("8.") != std::string::npos) {
                system_ = Systems::System::Redos8;
                return Systems::System::Redos8;
            } else {
                system_ = Systems::System::Redos7;
                return Systems::System::Redos7;
            }
        }
    }
    
    throw std::runtime_error("Can't detect system");
}