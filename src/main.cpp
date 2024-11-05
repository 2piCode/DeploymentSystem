#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <iostream>

#include "config.h"
#include "const.h"
#include "user_settings.h"
#include "xml_config_writer.h"

// void TestExportAndImport(std::unique_ptr<UserSettings>& user_settings) {
//     std::unique_ptr<StationBuilder>& builder = user_settings->GetBuilder();
//     auto root = builder->GetStation();
//     root->SetHostName("127.0.0.1");
//     root->SetName("root_name");
//     root->SetUsername("root");
//     root->SetPassword("root_password");
//     root->SetPath("/home/alex/.ssh/id_rsa");

//     root->SetDescription("This is a root station");
//     root->SetRole(Roles::Role::arm_kip);

//     assert(root == builder->GetStation());
//     assert(root == user_settings->GetConfig()->GetRoot().get());

//     auto child_station =
//         builder->CreateStation("244.178.44.111", "child_station");
//     child_station->SetDescription("This is a child station");
//     child_station->SetRole(Roles::Role::arm_engineer);
//     child_station->SetConnectionPort(16);
//     child_station->SetUsername("child_station_username");
//     child_station->SetPassword("child_station_password");

//     assert(child_station == builder->GetStation(1));

//     auto& config = user_settings->GetConfig();
//     config->SetInstallerPath(Systems::System::Windows,
//                              "C:\\Users\\alex\\Downloads\\");
//     config->SetInstallerPath(Systems::System::AstraLinux,
//                              "/home/alex/astra-linux/");
//     config->SetInstallerPath(Systems::System::Redos7, "/home/alex/redos7/");
//     config->SetInstallerPath(Systems::System::Redos8, "/home/alex/redos8/");

//     user_settings->ExportConfig("config.xml");

//     user_settings->ImportConfig("config.xml");

//     // MainStation test
//     auto& imported_config = user_settings->GetConfig();
//     auto main_station_imported = imported_config->GetRoot();

//     assert(main_station_imported->GetHostName() == "127.0.0.1");
//     assert(main_station_imported->GetName() == "root_name");
//     assert(main_station_imported->GetDescription() == "This is a root station");
//     assert(main_station_imported->GetRole() == Roles::Role::arm_kip);
//     assert(main_station_imported->GetUsername() == "root");
//     assert(main_station_imported->GetPath() == "/home/alex/.ssh/id_rsa");

//     // ChildStation test
//     assert(main_station_imported->GetChildStations().size() == 1);
//     auto& child_station_imported =
//         main_station_imported->GetChildStations().front();
//     assert(child_station_imported->GetHostName() == "244.178.44.111");
//     assert(child_station_imported->GetName() == "child_station");
//     assert(child_station_imported->GetDescription() ==
//            "This is a child station");
//     assert(child_station_imported->GetRole() == Roles::Role::arm_engineer);
//     assert(child_station_imported->GetConnectionPort() == 16);
//     assert(child_station_imported->GetUsername() == "child_station_username");
//     assert(!child_station_imported->GetPath().has_value());

//     // InstallerPath test
//     assert(imported_config->GetInstallerPath(Systems::System::Windows) ==
//            "C:\\Users\\alex\\Downloads\\");
//     assert(imported_config->GetInstallerPath(Systems::System::AstraLinux) ==
//            "/home/alex/astra-linux/");
//     assert(imported_config->GetInstallerPath(Systems::System::Redos7) ==
//            "/home/alex/redos7/");
//     assert(imported_config->GetInstallerPath(Systems::System::Redos8) ==
//            "/home/alex/redos8/");

//     // Because we don't have a password saving in the config file
//     assert(main_station_imported->GetPassword().isEmpty());
//     assert(child_station_imported->GetPassword().isEmpty());

//     // Clear settings
//     user_settings->ClearSettings();
// }

// void TestPasswordSaving() {
//     std::unique_ptr<MainStation> root = std::make_unique<MainStation>(
//         "127.0.0.1", "root_name",
//         ConnectionSettings{.username = "",
//                            .password = "root_password",
//                            .path_to_private_key = ""});

//     auto config = std::make_unique<Config>(std::move(root));
//     UserSettings settings(std::move(config),
//                           std::make_unique<XMLConfigWriter>());
//     settings.SetSavePasswordInConfig(true);

//     user_settings->ExportConfig("config_with_password.xml");

//     user_settings->ImportConfig("config_with_password.xml");

//     auto main_station_imported = user_settings->GetConfig()->GetRoot();
//     assert(main_station_imported->GetPassword() == "root_password");

//     // Clean settings
//     user_settings->ClearSettings();
// }

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

    std::unique_ptr<UserSettings> user_settings =
        std::make_unique<UserSettings>(std::make_unique<XMLConfigWriter>(),
                                       app.instance());

    // TestExportAndImport(user_settings);
    // TestPasswordSaving(user_settings);

    engine.rootContext()->setContextProperty("userSettings",
                                             user_settings.get());
    engine.rootContext()->setContextProperty("stationBuilder",
                                             user_settings->GetBuilder().get());
    engine.rootContext()->setContextProperty("config",
                                             user_settings->GetConfig().get());
    qmlRegisterSingletonType<Roles>("com.roles", 1, 0, "Roles",
                                    rolesSingletonProvider);
    qmlRegisterSingletonType<Roles>("com.systems", 1, 0, "Systems",
                                    systemsSingletonProvider);
    qmlRegisterType<Station>("com.stations", 1, 0, "Station");

    return app.exec();
}

#include "main.moc"
