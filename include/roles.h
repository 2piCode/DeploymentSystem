#ifndef ROLES_H
#define ROLES_H

#include <array>
#include <stdexcept>
#include <string>

enum class AppLanguage { ru = 0, en = 1 };

static AppLanguage Language = AppLanguage::ru;

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
    const char* name_ru;
    const char* name_en;
};

constexpr std::array<RoleMapping, 6> role_mapping = {
    {{Role::arm_engineer, "АРМ инженера", "Engineer workstation"},
     {Role::arm_operator, "АРМ оператора", "Operator workstation"},
     {Role::arm_kip, "АРМ КИП инженера", "C&I Engineer Workstation"},
     {Role::input_output_server, "Сервер ввода/вывода", "Input/Output server"},
     {Role::history_server, "Сервер истории", "History server"},
     {Role::integration_server, "Сервер интеграции", "Integration server"}}};

namespace role_converter {
inline std::string toString(Role role) {
    for (const auto& mapping : role_mapping) {
        if (mapping.role == role) {
            return Language == AppLanguage::ru ? mapping.name_ru
                                               : mapping.name_en;
        }
    }
    throw std::runtime_error("Unknown role");
}

inline Role fromString(const std::string& role_str) {
    for (const auto& mapping : role_mapping) {
        if (role_str == mapping.name_en || role_str == mapping.name_ru) {
            return mapping.role;
        }
    }
    throw std::runtime_error("Unknown role string");
}
};  // namespace role_converter

#endif
