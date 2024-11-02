#include "config.h"

Config::Config(std::unique_ptr<MainStation> root) : root_(std::move(root)) {}

void Config::SetInstallerPath(System system,
                              const std::filesystem::path& path) {
    systems_installer_paths_[system] = path;
}

std::filesystem::path Config::GetInstallerPath(System system) const {
    if (systems_installer_paths_.count(system) > 0) {
        return systems_installer_paths_.at(system);
    }

    return "";
}

std::unique_ptr<Config> CreateConfigPtr(StationBuilder& manager) {
    assert(manager.GetMainStation());

    return std::make_unique<Config>(
        std::unique_ptr<MainStation>(manager.ReleaseMainStation()));
}
