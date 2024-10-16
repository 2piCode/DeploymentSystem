#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <array>
#include <stdexcept>
#include <string>

enum class System { Windows = 0, AstraLinux = 1, Redos7 = 2, Redos8 = 3 };

struct SystemMapping {
    System system;
    const char* name;
};

constexpr std::array<SystemMapping, 4> system_mapping = {
    {{System::Windows, "Windows"},
     {System::AstraLinux, "AstraLinux"},
     {System::Redos7, "Redos7"},
     {System::Redos8, "Redos8"}}};

namespace system_converter {
inline std::string toString(System system) {
    for (const auto& mapping : system_mapping) {
        if (mapping.system == system) {
            return mapping.name;
        }
    }
    throw std::runtime_error("Unknown system");
}

inline System fromString(const std::string& systemStr) {
    for (const auto& mapping : system_mapping) {
        if (systemStr == mapping.name) {
            return mapping.system;
        }
    }
    throw std::runtime_error("Unknown system string");
}
};  // namespace system_converter

#endif
