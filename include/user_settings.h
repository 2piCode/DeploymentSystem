#ifndef USER_SETTINGS_H
#define USER_SETTINGS_H

#include <memory>

#include "config.h"
#include "config_writer.h"

class UserSettings : public QObject {
    Q_OBJECT
   public:
    UserSettings(std::unique_ptr<ConfigWriter> config_writer,
                 QObject* parent = nullptr);

    std::unique_ptr<Config>& GetConfig() { return config_; }
    const std::unique_ptr<Config>& GetConfig() const { return config_; }

    std::unique_ptr<StationBuilder>& GetBuilder() { return builder_; }
    const std::unique_ptr<StationBuilder>& GetBuilder() const {
        return builder_;
    }

    Q_INVOKABLE void ExportConfig(QString path) const;
    Q_INVOKABLE void ImportConfig(QString path);
    Q_INVOKABLE void ClearSettings();
    Q_INVOKABLE void SetSavePasswordInConfig(bool is_save);

   private:
    std::unique_ptr<StationBuilder> builder_;
    std::unique_ptr<Config> config_;
    std::shared_ptr<MainStation> main_station_;
    std::unique_ptr<ConfigWriter> writer_;
    bool is_save_password_in_config_ = false;
};

#endif
