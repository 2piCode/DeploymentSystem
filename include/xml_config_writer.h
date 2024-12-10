#ifndef XML_CONFIG_WRITER
#define XML_CONFIG_WRITER

#include "config_writer.h"

class XMLConfigWriter : public ConfigWriter {
   public:
    ~XMLConfigWriter() = default;
    void WriteInFile(const std::unique_ptr<Config>& config,
                     const std::filesystem::path& path,
                     bool is_save_password = false) const override;
    std::unique_ptr<Config> ReadFromFile(
        const std::filesystem::path& path) override;

   private:
    const char* CONFIG_KEY = "config";
    const char* STATIONS_KEY = "stations";
    const char* MAIN_STATION_KEY = "main_station";
    const char* STATION_KEY = "station";
    const char* INSTALLERS_KEY = "installers";

    const char* HOSTNAME_KEY = "hostname";
    const char* NAME_KEY = "name";
    const char* DESCRIPTION_KEY = "description";
    const char* ROLE_KEY = "role";

    const char* CONNECTION_SETTINGS_KEY = "connection_settings";
    const char* USERNAME_KEY = "username";
    const char* PASSWORD_KEY = "password";
    const char* PORT_KEY = "port";
    const char* PATH_KEY = "path_to_private_key";

    template <typename TPtr>
    void WriteStation(pugi::xml_node& station_node,
                      const TPtr& station,
                      bool is_save_password) const;
    void WriteInstallersPath(const std::unique_ptr<Config>& config,
                             pugi::xml_node& installers_node) const;
    void WriteInstallerPath(const std::unique_ptr<Config>&,
                            pugi::xml_node& installers_node,
                            Systems::System system) const;

    std::unique_ptr<MainStation> ReadStations(
        const pugi::xml_node& stations_node);
    template <typename T>
    std::unique_ptr<T> ReadStation(const pugi::xml_node& station_node);
    void ReadInstallersPath(std::unique_ptr<Config>& config,
                            const pugi::xml_node& installers_node);
    void ReadInstallerPath(std::unique_ptr<Config>& config,
                           const pugi::xml_node& installers_node,
                           Systems::System system);
};

#endif
