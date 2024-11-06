#include "language_controller.h"
#include <QCoreApplication>
#include <QDebug>

LanguageController* LanguageController::s_instance = nullptr;

LanguageController::LanguageController(QGuiApplication& app,
                                       QQmlApplicationEngine& engine,
                                       QObject* parent)
    : QObject(parent),
      m_app(app),
      m_engine(engine),
      m_currentLanguage("ru")
{
    if (s_instance == nullptr) {
        s_instance = this;
    } else {
        throw std::runtime_error("LanguageController instance already exists");
    }
    loadTranslator();
}

LanguageController* LanguageController::instance() {
    return s_instance;
}

void LanguageController::switchLanguage() {
    m_currentLanguage = (m_currentLanguage == "en") ? "ru" : "en";
    loadTranslator();
}

QString LanguageController::getCurrentLanguage() const {
    return m_currentLanguage;
}

void LanguageController::loadTranslator() {
    m_app.removeTranslator(&m_translator);

    QString qmPath = QCoreApplication::applicationDirPath() + "/translations";
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
