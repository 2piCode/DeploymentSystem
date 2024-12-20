#ifndef CONFIG_H
#define CONFIG_H

#include <filesystem>
#include <memory>
#include <unordered_map>

#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include "station.h"
#include "station_builder.h"
#include "systems.h"

class Config : public QObject {
    Q_OBJECT
   public:
    explicit Config(std::shared_ptr<MainStation> root);

    Q_INVOKABLE void SetInstallerPath(Systems::System system, QString path);
    Q_INVOKABLE QString GetInstallerPathString(Systems::System system) const;

    std::filesystem::path GetInstallerPath(Systems::System system) const;

    const std::shared_ptr<MainStation> GetRoot() const { return root_; }
    std::shared_ptr<MainStation> GetRoot() { return root_; }

   private:
    std::shared_ptr<MainStation> root_;
    std::unordered_map<Systems::System, std::filesystem::path>
        systems_installer_paths_;
};

#endif
