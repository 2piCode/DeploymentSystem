#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QUrl>

class Utils : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString defaultConfigPath READ getDefaultConfigPath WRITE setDefaultConfigPath);
    
   public:
    explicit Utils(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE QString urlToLocalFile(const QUrl &url) const {
        return url.toLocalFile();
    }
    Q_INVOKABLE QString getDefaultConfigPath() const{
        return defaultConfigPath_;
    };
    Q_INVOKABLE QString setDefaultConfigPath(QString new_path){
        defaultConfigPath_ = new_path;
        return defaultConfigPath_;
    };

    Q_INVOKABLE bool isValidIP(const QString &ip_address) const {
        QRegularExpression ipPattern(
            "^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
            "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
            "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\."
            "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

        QRegularExpressionMatch match = ipPattern.match(ip_address);

        return match.hasMatch() &&
               match.capturedLength() == ip_address.length();
    }

   private:
    QString defaultConfigPath_ = "C:/temp/main.xml";
};

#endif  // UTILS_H
