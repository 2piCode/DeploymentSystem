#include "user_settings.h"

UserSettings::UserSettings(std::unique_ptr<Config> config,
                           std::unique_ptr<ConfigWriter> config_writer)
    : config_(std::move(config)), writer_(std::move(config_writer)) {}

void UserSettings::ExportConfig(const std::filesystem::path& path) const {
    writer_->WriteInFile(config_, path, is_save_password_in_config_);
}

void UserSettings::ImportConfig(const std::filesystem::path& path) {
    config_ = std::move(writer_->ReadFromFile(path));
}

void UserSettings::SetSavePasswordInConfig(bool is_save) {
    is_save_password_in_config_ = is_save;
}
