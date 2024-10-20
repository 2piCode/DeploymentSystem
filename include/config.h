#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <memory>
#include <pugixml.hpp>
#include <unordered_map>

#include "station.h"
#include "systems.h"

class Config {
   public:
    explicit Config(std::unique_ptr<MainStation> root);

    void SetInstallerPath(System system, const std::filesystem::path& path);
    std::filesystem::path GetInstallerPath(System system) const;

    const std::unique_ptr<MainStation>& GetRoot() const { return root_; }
    std::unique_ptr<MainStation>& GetRoot() { return root_; }

   private:
    std::unique_ptr<MainStation> root_;
    std::unordered_map<System, std::filesystem::path> systems_installer_paths_;
};

#endif
