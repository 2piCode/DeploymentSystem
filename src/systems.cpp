#include "systems.h"

#include <array>
#include <stdexcept>

Systems* Systems::instance() {
    static Systems instance;
    return &instance;
}

Systems::Systems(QObject* parent) : QObject(parent) {}

struct SystemMapping {
    Systems::System system;
    const char* name;
};

constexpr std::array<SystemMapping, 4> system_mapping = {
    {{Systems::System::Windows, "Windows"},
     {Systems::System::AstraLinux, "AstraLinux"},
     {Systems::System::Redos7, "Redos7"},
     {Systems::System::Redos8, "Redos8"}}};

QString Systems::toString(System system) {
    for (const auto& mapping : system_mapping) {
        if (mapping.system == system) {
            return mapping.name;
        }
    }
    throw std::runtime_error("Unknown system");
}

Systems::System Systems::fromString(const QString& systemStr) {
    for (const auto& mapping : system_mapping) {
        if (systemStr == mapping.name) {
            return mapping.system;
        }
    }
    throw std::runtime_error("Unknown system string");
}
