#include "config.h"

Config::Config(std::shared_ptr<MainStation> root) : root_(root) {}

void Config::SetInstallerPath(Systems::System system, QString path) {
    systems_installer_paths_[system] =
        std::filesystem::path(path.toStdString());
    emit installersPathsChanged();
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

Q_INVOKABLE QVariantMap Config::getInstallersPaths() const {
    QVariantMap map;
    for (const auto& [key, value] : systems_installer_paths_) {
        map[QString::number(static_cast<int>(key))] =
            QString::fromStdString(value.string());
    }
    return map;
}
