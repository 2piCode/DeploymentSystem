#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <iostream>

#include "config.h"
#include "user_settings.h"
#include "xml_config_writer.h"
#include "language_controller.h"
#include "station_model.h"


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
    StationModel *stationModel = new StationModel(&app);


    //FIXME
    stationModel->addStation("127.0.0.99", "АРМ инженера", "Описание", Roles::Role::arm_engineer, "user1", "pass1", 22, "/path/to/key1");
    stationModel->addStation("127.0.0.1", "АРМ оператора", "Описание и в этот раз с пробелами и длиннее", Roles::Role::arm_operator, "user2", "pass2", 22, "/path/to/key2");
    stationModel->addStation("127.0.0.4", "Сервер истории", "Описание", Roles::Role::arm_kip, "user3", "pass3", 22, "/path/to/key3");
    stationModel->addStation("192.168.1.15", "Сервер истории", "Описание", Roles::Role::history_server, "user3", "pass3", 22, "/path/to/key3");
    stationModel->addStation("192.168.1.17", "Сервер ввода/вывода", "Описание", Roles::Role::input_output_server, "user3", "pass3", 22, "/path/to/key3");
    stationModel->addStation("192.168.1.19", "Сервер интеграции", "Описание", Roles::Role::integration_server, "user3", "pass3", 22, "/path/to/key3");


    engine.rootContext()->setContextProperty("languageController",
                                             &langController);

    
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    

    std::unique_ptr<UserSettings> user_settings =
        std::make_unique<UserSettings>(std::make_unique<XMLConfigWriter>(),
                                       app.instance());

    engine.rootContext()->setContextProperty("userSettings",
                                             user_settings.get());
    engine.rootContext()->setContextProperty("stationBuilder",
                                             user_settings->GetBuilder().get());
    engine.rootContext()->setContextProperty("config",
                                             user_settings->GetConfig().get());
    engine.rootContext()->setContextProperty("stationModel", stationModel);
                                             
    qmlRegisterSingletonType<Roles>("com.roles", 1, 0, "Roles",
                                    rolesSingletonProvider);
    qmlRegisterSingletonType<Roles>("com.systems", 1, 0, "Systems",
                                    systemsSingletonProvider);
    qmlRegisterType<Station>("com.stations", 1, 0, "Station");

    const QUrl url(QStringLiteral("qrc:/DeldeliveryApp/qml/Main.qml"));
    engine.load(url);

    return app.exec();
}

