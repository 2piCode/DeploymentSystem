#include "roles.h"
#include "language_controller.h"

#include <array>
#include <stdexcept>

Roles* Roles::instance() {
    static Roles instance;
    return &instance;
}

Roles::Roles(QObject* parent) : QObject(parent) {}

struct RoleMapping {
    Roles::Role role;
    const char* name_ru;
    const char* name_en;
};

std::array<RoleMapping, 6> role_mapping = {
    {{Roles::Role::arm_engineer, "АРМ инженера", "Engineer workstation"},
     {Roles::Role::arm_operator, "АРМ оператора", "Operator workstation"},
     {Roles::Role::arm_kip, "АРМ КИП инженера", "C&I Engineer Workstation"},
     {Roles::Role::input_output_server, "Сервер ввода/вывода",
      "Input/Output server"},
     {Roles::Role::history_server, "Сервер истории", "History server"},
     {Roles::Role::integration_server, "Сервер интеграции",
      "Integration server"}}};

QString Roles::toString(Role role) {
    for (const auto& mapping : role_mapping) {
        if (mapping.role == role) {
            return Language == AppLanguage::ru ? mapping.name_ru
                                               : mapping.name_en;
        }
    }
    throw std::runtime_error("Unknown role");
}

Roles::Role Roles::fromString(const QString& role_str) {
    for (const auto& mapping : role_mapping) {
        if (role_str == mapping.name_en || role_str == mapping.name_ru) {
            return mapping.role;
        }
    }
    throw std::runtime_error("Unknown role string");
}

QVariantList Roles::getAllRoles() const{
            QVariantList rolesList;
            QString currentLanguage = LanguageController::instance()->getCurrentLanguage();
            for (const auto& mapping : role_mapping) {
                QVariantMap roleMap;
                roleMap["value"] = static_cast<int>(mapping.role);
                
                if (currentLanguage == "ru") {
                    roleMap["name"] = QString::fromUtf8(mapping.name_ru);
                } else {
                    roleMap["name"] = QString::fromUtf8(mapping.name_en);
                }
                
                rolesList.append(roleMap);
            }
            return rolesList;
        }
