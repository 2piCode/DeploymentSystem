#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <iostream>

#include "config.h"
#include "language_controller.h"
#include "user_settings.h"
#include "utils.h"
#include "xml_config_writer.h"

static QObject* rolesSingletonProvider(QQmlEngine* engine,
                                       QJSEngine* scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return Roles::instance();
}

static QObject* systemsSingletonProvider(QQmlEngine* engine,
                                         QJSEngine* scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return Systems::instance();
}

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    LanguageController langController(app, engine);

    engine.rootContext()->setContextProperty("languageController",
                                             &langController);
    Utils utils;

    engine.rootContext()->setContextProperty("utils", &utils);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);

    std::unique_ptr<UserSettings> user_settings =
        std::make_unique<UserSettings>(std::make_unique<XMLConfigWriter>(),
                                       app.instance());

    auto mainStation = user_settings->GetBuilder()->GetMainStation();
    if (mainStation) {
        ConnectionSettings settings;
        settings.port = 22;
        settings.username = "astra";
        settings.password = "123456789";
        mainStation->AddChildStation(
            std::make_unique<Station>("192.168.1.20", "name1", settings));
        mainStation->AddChildStation(
            std::make_unique<Station>("192.168.1.22", "name2", settings));
        mainStation->GetChildStations().at(1)->SetRole(Roles::Role::arm_kip);
    } else {
        std::cerr << "MainStation is null. Cannot add child station."
                  << std::endl;
    }

    engine.rootContext()->setContextProperty("userSettings",
                                             user_settings.get());
    engine.rootContext()->setContextProperty("stationBuilder",
                                             user_settings->GetBuilder().get());
    engine.rootContext()->setContextProperty("config",
                                             user_settings->GetConfig().get());

    QObject::connect(user_settings.get(), &UserSettings::configChanged, [&]() {
        engine.rootContext()->setContextProperty("userSettings",
                                                 user_settings.get());
        engine.rootContext()->setContextProperty(
            "stationBuilder", user_settings->GetBuilder().get());
        engine.rootContext()->setContextProperty(
            "config", user_settings->GetConfig().get());
    });

    qmlRegisterSingletonType<Roles>("com.roles", 1, 0, "Roles",
                                    rolesSingletonProvider);
    qmlRegisterSingletonType<Systems>("com.systems", 1, 0, "Systems",
                                      systemsSingletonProvider);
    qmlRegisterType<Station>("com.stations", 1, 0, "Station");

    const QUrl url(QStringLiteral("qrc:/DeldeliveryApp/qml/Main.qml"));
    engine.load(url);

    return app.exec();
}
