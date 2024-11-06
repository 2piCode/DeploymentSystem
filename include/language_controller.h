#ifndef LANGUAGECONTROLLER_H
#define LANGUAGECONTROLLER_H

#include <QObject>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QString>

class LanguageController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentLanguage READ getCurrentLanguage NOTIFY languageChanged)

public:
    explicit LanguageController(QGuiApplication& app,
                                QQmlApplicationEngine& engine,
                                QObject* parent = nullptr);

    static LanguageController* instance();

    Q_INVOKABLE void switchLanguage();
    Q_INVOKABLE QString getCurrentLanguage() const;

signals:
    void languageChanged();

private:
    void loadTranslator();

    QGuiApplication& m_app;
    QQmlApplicationEngine& m_engine;
    QTranslator m_translator;
    QString m_currentLanguage;

    static LanguageController* s_instance;
};

#endif // LANGUAGECONTROLLER_H
