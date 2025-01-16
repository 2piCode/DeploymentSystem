#include "user_settings.h"

#include "xml_config_writer.h"

const QString default_ip = "127.0.0.1";
const QString default_name = "Main station";

UserSettings::UserSettings(std::unique_ptr<ConfigWriter> config_writer,
                           QObject* parent)
    : writer_(std::move(config_writer)) {
    ClearSettings();
}

void UserSettings::ExportConfig(QString path) const {
    writer_->WriteInFile(config_, std::filesystem::path(path.toStdString()));
}

void UserSettings::ImportConfig(QString path) {
    std::unique_ptr<Config> newConfig =
        writer_->ReadFromFile(std::filesystem::path(path.toStdString()));
    
    config_->SetRoot(newConfig->GetRoot());
    config_->SetInstallersPaths(newConfig->GetInstallersPaths());
    config_->SetSavePassword(newConfig->IsSavePassword());
    main_station_ = std::move(config_->GetRoot());
    builder_.reset(new StationBuilder(main_station_));
    emit configChanged();
}

void UserSettings::ClearSettings() {
    main_station_.reset(
        new MainStation(default_ip, default_name, ConnectionSettings{}));
    builder_.reset(new StationBuilder(main_station_));
    config_.reset(new Config(main_station_));
    SetSavePasswordInConfig(false);
}

void UserSettings::SetSavePasswordInConfig(bool is_save) {
    config_->SetSavePassword(is_save);
    emit configChanged();
}

bool UserSettings::GetSavePasswordInConfig() const {
    return config_->IsSavePassword();
}