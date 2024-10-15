#ifndef ROLES_H
#define ROLES_H

#include <array>
#include <stdexcept>
#include <string>

enum class Role {
    arm_engineer = 0,
    arm_operator = 1,
    arm_kip = 2,
    input_output_server = 3,
    history_server = 4,
    integration_server = 5
};

struct RoleMapping {
    Role role;
    const char* name;
};

constexpr std::array<RoleMapping, 6> role_mapping = {
    {{Role::arm_engineer, "АРМ инженер"},
     {Role::arm_operator, "АРМ оператор"},
     {Role::arm_kip, "АРМ КИП инженер"},
     {Role::input_output_server, "Сервер ввода вывода"},
     {Role::history_server, "Сервер истории"},
     {Role::integration_server, "Сервер интеграции"}}};

namespace role_converter {
inline std::string toString(Role role) {
    for (const auto& mapping : role_mapping) {
        if (mapping.role == role) {
            return mapping.name;
        }
    }
    throw std::runtime_error("Unknown role");
}

inline Role fromString(const std::string& roleStr) {
    for (const auto& mapping : role_mapping) {
        if (roleStr == mapping.name) {
            return mapping.role;
        }
    }
    throw std::runtime_error("Unknown role string");
}
};  // namespace role_converter

#endif
