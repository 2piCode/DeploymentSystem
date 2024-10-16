#include "xml_config_writer.h"

void XMLConfigWriter::WriteInFile(const std::unique_ptr<Config>& config,
                                  const std::filesystem::path& path,
                                  bool is_save_password) const {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(CONFIG_KEY);
    pugi::xml_node stations_node = root.append_child(STATIONS_KEY);
    pugi::xml_node main_station_node = root.append_child(MAIN_STATION_KEY);
    WriteStation(main_station_node, *config->GetRoot(), is_save_password);

    for (const Station& station : config->GetRoot()->GetChildStations()) {
        pugi::xml_node node = stations_node.append_child(STATION_KEY);
        WriteStation(node, station, is_save_password);
    }
    pugi::xml_node installers_node = root.append_child(INSTALLERS_KEY);
    WriteInstallersPath(config, installers_node);
    doc.save_file(path.c_str());
}

void XMLConfigWriter::WriteStation(pugi::xml_node& station_node,
                                   const Station& station,
                                   bool is_save_password) const {
    station_node.append_child(HOSTNAME_KEY)
        .text()
        .set(station.GetHostName().c_str());
    station_node.append_child(NAME_KEY).text().set(station.GetName().c_str());
    station_node.append_child(DESCRIPTION_KEY)
        .text()
        .set(station.GetDescription().c_str());
    station_node.append_child(ROLE_KEY).text().set(
        role_converter::toString(station.GetRole()).c_str());

    pugi::xml_node settings_node =
        station_node.append_child(CONNECTION_SETTINGS_KEY);
    settings_node.append_child(USERNAME_KEY)
        .text()
        .set(station.GetSettings().username.c_str());

    if (station.GetSettings().path_to_private_key.has_value()) {
        settings_node.append_child(PATH_KEY).text().set(
            station.GetSettings().path_to_private_key.value().c_str());
    }

    if (is_save_password) {
        settings_node.append_child(PASSWORD_KEY)
            .text()
            .set(station.GetSettings().password.c_str());
    }

    settings_node.append_child(PORT_KEY).text().set(station.GetSettings().port);
}

void XMLConfigWriter::WriteInstallersPath(
    const std::unique_ptr<Config>& config,
    pugi::xml_node& installers_node) const {
    WriteInstallerPath(config, installers_node, System::Windows);
    WriteInstallerPath(config, installers_node, System::AstraLinux);
    WriteInstallerPath(config, installers_node, System::Redos7);
    WriteInstallerPath(config, installers_node, System::Redos8);
}

void XMLConfigWriter::WriteInstallerPath(const std::unique_ptr<Config>& config,
                                         pugi::xml_node& installers_node,
                                         System system) const {
    installers_node.append_child(system_converter::toString(system).c_str())
        .text()
        .set(config->GetInstallerPath(system).string().c_str());
}

std::unique_ptr<Config> XMLConfigWriter::ReadFromFile(
    const std::filesystem::path& path) {
    pugi::xml_document doc;
    doc.load_file(path.c_str());
    pugi::xml_node root = doc.child(CONFIG_KEY);
    pugi::xml_node main_station_node = root.child(MAIN_STATION_KEY);
    std::unique_ptr<MainStation> main_station = std::make_unique<MainStation>(
        ReadStation<MainStation>(main_station_node));

    pugi::xml_node stations_node = root.child(STATIONS_KEY);

    for (pugi::xml_node station_node = stations_node.first_child();
         station_node; station_node = station_node.next_sibling()) {
        main_station->AddChildStation(ReadStation<Station>(station_node));
    }

    std::unique_ptr<Config> config =
        std::make_unique<Config>(std::move(main_station));
    pugi::xml_node installers_node = root.child(INSTALLERS_KEY);
    ReadInstallersPath(config, installers_node);
    return std::move(config);
}

template <typename T>
T XMLConfigWriter::ReadStation(const pugi::xml_node& station_node) {
    std::string hostname = station_node.child(HOSTNAME_KEY).text().as_string();
    std::string name = station_node.child(NAME_KEY).text().as_string();
    std::string description =
        station_node.child(DESCRIPTION_KEY).text().as_string();
    std::string role_str = station_node.child(ROLE_KEY).text().as_string();
    Role role = role_converter::fromString(role_str);

    pugi::xml_node settings_node = station_node.child(CONNECTION_SETTINGS_KEY);

    T station(hostname, name);
    station.SetDescription(description);
    station.SetRole(role);
    station.SetConnectionPort(
        static_cast<quint16>(settings_node.child(PORT_KEY).text().as_int()));
    station.SetUsername(settings_node.child(USERNAME_KEY).text().as_string());
    station.SetPassword(settings_node.child(PASSWORD_KEY).text().as_string());

    const std::string path_to_private_key =
        settings_node.child(PATH_KEY).text().as_string();
    if (path_to_private_key != "") {
        station.SetPath(path_to_private_key);
    }
    return station;
}

template Station XMLConfigWriter::ReadStation<Station>(
    const pugi::xml_node& station_node);
template MainStation XMLConfigWriter::ReadStation<MainStation>(
    const pugi::xml_node& station_node);

void XMLConfigWriter::ReadInstallersPath(
    std::unique_ptr<Config>& config, const pugi::xml_node& installers_node) {
    ReadInstallerPath(config, installers_node, System::Windows);
    ReadInstallerPath(config, installers_node, System::AstraLinux);
    ReadInstallerPath(config, installers_node, System::Redos7);
    ReadInstallerPath(config, installers_node, System::Redos8);
}

void XMLConfigWriter::ReadInstallerPath(std::unique_ptr<Config>& config,
                                        const pugi::xml_node& installers_node,
                                        System system) {
    config->SetInstallerPath(
        system,
        installers_node.child(system_converter::toString(system).c_str())
            .text()
            .as_string());
}
