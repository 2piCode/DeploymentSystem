#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <iostream>

#include "config.h"
#include "const.h"
#include "user_settings.h"
#include "xml_config_writer.h"

void TestExportAndImport() {
    std::unique_ptr<MainStation> root = std::make_unique<MainStation>(
        "127.0.0.1", "root_name",
        ConnectionSettings{.username = "root",
                           .password = "root_password",
                           .path_to_private_key = "/home/alex/.ssh/id_rsa"});

    root->SetDescription("This is a root station");
    root->SetRole(Role::arm_kip);

    Station child_station("244.178.44.111", "child_station");
    child_station.SetDescription("This is a child station");
    child_station.SetRole(Role::arm_engineer);
    child_station.SetConnectionPort(16);
    child_station.SetUsername("child_station_username");
    child_station.SetPassword("child_station_password");

    root->AddChildStation(std::move(child_station));

    auto config = std::make_unique<Config>(std::move(root));
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
    assert(main_station_imported->GetRole() == Role::arm_kip);
    assert(main_station_imported->GetSettings().username == "root");
    assert(main_station_imported->GetSettings().path_to_private_key ==
           "/home/alex/.ssh/id_rsa");

    // ChildStation test
    assert(settings.GetConfig()->GetRoot()->GetChildStations().size() == 1);
    auto& child_station_imported =
        settings.GetConfig()->GetRoot()->GetChildStations()[0];
    assert(child_station_imported.GetHostName() == "244.178.44.111");
    assert(child_station_imported.GetName() == "child_station");
    assert(child_station_imported.GetDescription() ==
           "This is a child station");
    assert(child_station_imported.GetRole() == Role::arm_engineer);
    assert(child_station_imported.GetSettings().port == 16);
    assert(child_station_imported.GetSettings().username ==
           "child_station_username");
    if (child_station_imported.GetSettings().path_to_private_key.has_value()) {
        std::cout
            << "pizec: "
            << child_station_imported.GetSettings().path_to_private_key.value()
            << std::endl;
    }
    assert(
        !child_station_imported.GetSettings().path_to_private_key.has_value());

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
    assert(main_station_imported->GetSettings().password.empty());
    assert(child_station_imported.GetSettings().password.empty());
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
    assert(main_station_imported->GetSettings().password == "root_password");
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

    // TestExportAndImport();
    // TestPasswordSaving();

    return app.exec();
}

#include "main.moc"
