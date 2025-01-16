#ifndef CONFIG_H
#define CONFIG_H

#ifndef Q_MOC_RUN
#include <filesystem>
#endif
#include <memory>
#include <pugixml.hpp>
#include <unordered_map>

#include "station.h"
#include "station_builder.h"
#include "systems.h"

class Config : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantMap installersPath READ getInstallersPaths NOTIFY
                   installersPathsChanged)

   public:
    explicit Config(std::shared_ptr<MainStation> root);

    Q_INVOKABLE void SetInstallerPath(Systems::System system, QString path);
    Q_INVOKABLE QString GetInstallerPathString(Systems::System system) const;
    Q_INVOKABLE QVariantMap getInstallersPaths() const;

    std::filesystem::path GetInstallerPath(Systems::System system) const;

    const std::shared_ptr<MainStation> GetRoot() const { return root_; }
    std::shared_ptr<MainStation> GetRoot() { return root_; }
    void SetRoot(std::shared_ptr<MainStation> new_root) { root_ = new_root; }
    std::unordered_map<Systems::System, std::filesystem::path>
    GetInstallersPaths() const {
        return systems_installer_paths_;
    };
    void SetInstallersPaths(
        std::unordered_map<Systems::System, std::filesystem::path>
            new_installers) {
        systems_installer_paths_ = new_installers;
        emit installersPathsChanged();
    };
    bool IsSavePassword() const { return is_save_password_; }
    void SetSavePassword(bool value) { is_save_password_ = value; }
   signals:
    void installersPathsChanged();

   private:
    std::shared_ptr<MainStation> root_;
    std::unordered_map<Systems::System, std::filesystem::path>
        systems_installer_paths_;
    bool is_save_password_ = false;
};

#endif
