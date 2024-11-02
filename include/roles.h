#ifndef ROLES_H
#define ROLES_H

#include <QObject>
#include <QString>

enum class AppLanguage { ru = 0, en = 1 };

static AppLanguage Language = AppLanguage::ru;

class Roles : public QObject {
    Q_OBJECT

   public:
    enum class Role {
        arm_engineer = 0,
        arm_operator = 1,
        arm_kip = 2,
        input_output_server = 3,
        history_server = 4,
        integration_server = 5
    };

    Q_ENUM(Role)

    static Roles* instance();

    Q_INVOKABLE QString toString(Role);
    Q_INVOKABLE Role fromString(const QString& role_str);

   private:
    explicit Roles(QObject* parent = nullptr);
};

namespace role_converter {
inline std::string toString(Roles::Role role) {
    return Roles::instance()->toString(role).toStdString();
}

inline Roles::Role fromString(const std::string& role_str) {
    return Roles::instance()->fromString(QString::fromStdString(role_str));
}
};  // namespace role_converter

#endif
