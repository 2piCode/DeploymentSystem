#ifndef USERSETTINGSWRAPPER_H
#define USERSETTINGSWRAPPER_H

#include <QObject>
#include <memory>
#include <QString>
#include <filesystem>
#include "user_settings.h"
#include "config.h"
#include "config_writer.h"

class UserSettingsWrapper : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool savePasswordInConfig READ isSavePasswordInConfig WRITE
                   setSavePasswordInConfig NOTIFY savePasswordInConfigChanged)

   public:
    static UserSettingsWrapper* instance(QQmlEngine* engine = nullptr,
                                         QJSEngine* scriptEngine = nullptr) {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        static UserSettingsWrapper instance;
        return &instance;
    }

    Q_INVOKABLE void exportConfig(const QString& path) const {
        std::filesystem::path fsPath = path.toStdString();
        userSettings_.ExportConfig(fsPath);
    }
    Q_INVOKABLE

    Q_INVOKABLE void importConfig(const QString& path) {
        std::filesystem::path fsPath = path.toStdString();
        userSettings_.ImportConfig(fsPath);
    }

    bool isSavePasswordInConfig() const {
        return userSettings_.is_save_password_in_config_;
    }

    void setSavePasswordInConfig(bool is_save) {
        if (userSettings_.is_save_password_in_config_ != is_save) {
            userSettings_.SetSavePasswordInConfig(is_save);
            emit savePasswordInConfigChanged();
        }
    }

   signals:
    void savePasswordInConfigChanged();

   private:
    UserSettingsWrapper() : userSettings_(std::make_unique<ConfigWriter>()) {}

    UserSettingsWrapper(const UserSettingsWrapper&) = delete;
    UserSettingsWrapper& operator=(const UserSettingsWrapper&) = delete;

    UserSettings userSettings_;
};

#endif
