#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <iostream>
#include "const.h"

class LanguageController : public QObject {
    Q_OBJECT
   public:
    explicit LanguageController(QGuiApplication& app,
                                QQmlApplicationEngine& engine,
                                QObject* parent = nullptr)
        : QObject(parent),
          m_app(app),
          m_engine(engine),
          m_currentLanguage("ru") {
        loadTranslator();
    }

    Q_INVOKABLE void switchLanguage() {
        m_currentLanguage = (m_currentLanguage == "en") ? "ru" : "en";
        loadTranslator();
    }

   signals:
    void languageChanged();

   private:
    void loadTranslator() {
        m_app.removeTranslator(&m_translator);

        QString qmPath =
            QCoreApplication::applicationDirPath() + "/translations";
        QString qmFile = QString("translations_%1.qm").arg(m_currentLanguage);

        QString fullQmPath = qmPath + "/" + qmFile;

        if (m_translator.load(fullQmPath)) {
            m_app.installTranslator(&m_translator);
            qDebug() << "Loaded translation:" << fullQmPath;
        } else {
            qWarning() << "Failed to load translation file:" << fullQmPath;
        }

        m_engine.retranslate();
        emit languageChanged();
    }

    QGuiApplication& m_app;
    QQmlApplicationEngine& m_engine;
    QTranslator m_translator;
    QString m_currentLanguage;
};

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    LanguageController langController(app, engine);
    engine.rootContext()->setContextProperty("languageController",
                                             &langController);

    Constants constants;
    engine.rootContext()->setContextProperty("Const", &constants);

    const QUrl url(QStringLiteral("qrc:/DeldeliveryApp/qml/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

#include "main.moc"
