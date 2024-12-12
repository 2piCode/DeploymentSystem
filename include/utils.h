#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QUrl>

class Utils : public QObject {
    Q_OBJECT
   public:
    explicit Utils(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE QString urlToLocalFile(const QUrl &url) const {
        return url.toLocalFile();
    }

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
};

#endif  // UTILS_H
