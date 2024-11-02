#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <iostream>

#include "config.h"
#include "user_settings.h"
#include "xml_config_writer.h"

void TestExportAndImport(StationBuilder& builder) {
    auto root = builder.CreateStation(
        "127.0.0.1", "root_name",
        ConnectionSettings{.username = "root",
                           .password = "root_password",
                           .path_to_private_key = "/home/alex/.ssh/id_rsa"});

    root->SetDescription("This is a root station");
    root->SetRole(Roles::Role::arm_kip);

    assert(root == builder.GetStation());

    auto child_station =
        builder.CreateStation("244.178.44.111", "child_station");
    child_station->SetDescription("This is a child station");
    child_station->SetRole(Roles::Role::arm_engineer);
    child_station->SetConnectionPort(16);
    child_station->SetUsername("child_station_username");
    child_station->SetPassword("child_station_password");

    assert(child_station == builder.GetStation(1));

    std::unique_ptr<Config> config = CreateConfigPtr(builder);
    config->SetInstallerPath(System::Windows, "C:\\Users\\alex\\Downloads\\");
    config->SetInstallerPath(System::AstraLinux, "/home/alex/astra-linux/");
    config->SetInstallerPath(System::Redos7, "/home/alex/redos7/");
    config->SetInstallerPath(System::Redos8, "/home/alex/redos8/");

    UserSettings settings(std::move(config),
                          std::make_unique<XMLConfigWriter>());

    settings.ExportConfig("config.xml");

    settings.ImportConfig("config.xml");

    // MainStation test
    auto& main_station_imported = settings.GetConfig()->GetRoot();
    assert(main_station_imported->GetHostName() == "127.0.0.1");
    assert(main_station_imported->GetName() == "root_name");
    assert(main_station_imported->GetDescription() == "This is a root station");
    assert(main_station_imported->GetRole() == Roles::Role::arm_kip);
    assert(main_station_imported->GetUsername() == "root");
    assert(main_station_imported->GetPath() == "/home/alex/.ssh/id_rsa");

    // ChildStation test
    assert(settings.GetConfig()->GetRoot()->GetChildStations().size() == 1);
    auto& child_station_imported =
        settings.GetConfig()->GetRoot()->GetChildStations().front();
    assert(child_station_imported->GetHostName() == "244.178.44.111");
    assert(child_station_imported->GetName() == "child_station");
    assert(child_station_imported->GetDescription() ==
           "This is a child station");
    assert(child_station_imported->GetRole() == Roles::Role::arm_engineer);
    assert(child_station_imported->GetConnectionPort() == 16);
    assert(child_station_imported->GetUsername() == "child_station_username");
    assert(!child_station_imported->GetPath().has_value());

    // InstallerPath test
    assert(settings.GetConfig()->GetInstallerPath(System::Windows) ==
           "C:\\Users\\alex\\Downloads\\");
    assert(settings.GetConfig()->GetInstallerPath(System::AstraLinux) ==
           "/home/alex/astra-linux/");
    assert(settings.GetConfig()->GetInstallerPath(System::Redos7) ==
           "/home/alex/redos7/");
    assert(settings.GetConfig()->GetInstallerPath(System::Redos8) ==
           "/home/alex/redos8/");

    // Because we don't have a password saving in the config file
    assert(main_station_imported->GetPassword().isEmpty());
    assert(child_station_imported->GetPassword().isEmpty());
}

void TestPasswordSaving() {
    std::unique_ptr<MainStation> root = std::make_unique<MainStation>(
        "127.0.0.1", "root_name",
        ConnectionSettings{.username = "",
                           .password = "root_password",
                           .path_to_private_key = ""});

    auto config = std::make_unique<Config>(std::move(root));
    UserSettings settings(std::move(config),
                          std::make_unique<XMLConfigWriter>());
    settings.SetSavePasswordInConfig(true);

    settings.ExportConfig("config_with_password.xml");

    settings.ImportConfig("config_with_password.xml");

    auto& main_station_imported = settings.GetConfig()->GetRoot();
    assert(main_station_imported->GetPassword() == "root_password");
}

static QObject* roles_singletontype_provider(QQmlEngine* engine,
                                             QJSEngine* scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return Roles::instance();
}

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/DeldeliveryApp/qml/Main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
    engine.load(url);

    StationBuilder stationBuilder;
    TestExportAndImport(stationBuilder);
    TestPasswordSaving();

    engine.rootContext()->setContextProperty("stationBuilder", &stationBuilder);
    qmlRegisterSingletonType<Roles>("com.roles", 1, 0, "Roles",
                                    roles_singletontype_provider);
    qmlRegisterType<Station>("com.stations", 1, 0, "Station");

    return app.exec();
}
