#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <memory>

#include "config.h"
#include "config_writer.h"

class UserSettings {
   public:
    UserSettings(std::unique_ptr<ConfigWriter> config_writer);

    UserSettings(std::unique_ptr<Config> config,
                 std::unique_ptr<ConfigWriter> config_writer);

    std::unique_ptr<Config>& GetConfig() { return config_; }
    const std::unique_ptr<Config>& GetConfig() const { return config_; }

    void ExportConfig(const std::filesystem::path& path) const;
    void ImportConfig(const std::filesystem::path& path);

    void SetSavePasswordInConfig(bool is_save);

   private:
    std::unique_ptr<Config> config_;
    std::unique_ptr<ConfigWriter> writer_;
    bool is_save_password_in_config_ = false;
};

#endif
