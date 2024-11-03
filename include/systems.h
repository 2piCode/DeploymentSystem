#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <QObject>
#include <string>

class Systems : public QObject {
    Q_OBJECT

   public:
    enum class System { Windows = 0, AstraLinux = 1, Redos7 = 2, Redos8 = 3 };

    Q_ENUM(System)

    static Systems* instance();

    Q_INVOKABLE QString toString(System);
    Q_INVOKABLE System fromString(const QString& system_str);

   private:
    explicit Systems(QObject* parent = nullptr);
};

namespace system_converter {
inline std::string toString(Systems::System system) {
    return Systems::instance()->toString(system).toStdString();
}

inline Systems::System fromString(const std::string& system_str) {
    return Systems::instance()->fromString(QString::fromStdString(system_str));
}
};  // namespace system_converter

#endif
