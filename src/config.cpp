#include "config.h"

#include "systems.h"

Config::Config(std::shared_ptr<MainStation> root) : root_(root) {}

void Config::SetInstallerPath(Systems::System system, QString path) {
    systems_installer_paths_[system] =
        std::filesystem::path(path.toStdString());
}

Q_INVOKABLE QString
Config::GetInstallerPathString(Systems::System system) const {
    return QString::fromStdString(GetInstallerPath(system).string());
}

std::filesystem::path Config::GetInstallerPath(Systems::System system) const {
    if (systems_installer_paths_.count(system) > 0) {
        return systems_installer_paths_.at(system);
    }

    return "";
}
