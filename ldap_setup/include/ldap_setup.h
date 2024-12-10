#ifndef LDAP_SETUP_H
#define LDAP_SETUP_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

#include "pugixml.hpp"

enum class SystemType { AstraLinux, Redos};


class SecuritySetuper {
   public:
    void SetupAstra(const std::string& name, const std::string& port);
    void SetupRedos(const std::string& name, const std::string& port);

   private:
    std::string GetEncryptedPassword(const std::string& password);
    void WriteConfig(const std::string& filepath, const std::string& content);
    void RedosConfigureLDAP();
    void AstraConfigureLDAP();
    void ConfigureAstraSecurityAgent(const std::string& password, const std::string& port);
    void ConfigureAstraSecurityUseractivity();
    void ConfigureMonitorServices();

    void ConfigureAstraDomainAgent(const std::string& name, const std::string& port);
    void ConfigureAstraNetAgent(const std::string& name, const std::string& port);

    void BackupFile(const std::string& original_path);
    pugi::xml_document ParseXml(const std::string& filepath);
    pugi::xml_node FindTag(pugi::xml_document& doc, const std::string& tag);
    void UpdateAttribute(pugi::xml_node& element, const std::string& attr_name, const std::string& new_value);
    void WriteXml(pugi::xml_document& doc, const std::string& file_path);
    void RestartService(const std::string& service_name);
    
    const std::string DOMAIN = "maxcrc.com";
    const std::string ORG ="reglab";
    const std::string PASSWORD ="secret";
    
    std::string SLAPD_RECONFIGURE_COMMAND =
        "echo 'slapd slapd/no_configuration boolean false' | debconf-set-selections && "
        "echo 'slapd slapd/domain string " + DOMAIN + "' | debconf-set-selections && "
        "echo 'slapd shared/organization string " + ORG + "' | debconf-set-selections && "
        "echo 'slapd slapd/password1 password " + PASSWORD + "' | debconf-set-selections && "
        "echo 'slapd slapd/password2 password " + PASSWORD + "' | debconf-set-selections && "
        "echo 'slapd slapd/purge_database boolean false' | debconf-set-selections && "
        "echo 'slapd slapd/move_old_database boolean true' | debconf-set-selections && "
        "dpkg-reconfigure slapd";

    const std::string REDOS_CONFIG_LDIF_CONTENT = 
        "dn: olcDatabase={0}config,cn=config\n"
        "changetype: modify\n"
        "replace: olcRootPW\n"
        "olcRootPW: ";

    const std::string ASTRA_ACCESS_LDIF_CONTENT = 
        "dn: olcDatabase={1}mdb,cn=config\n"
        "changetype: modify\n"
        "replace: olcAccess\n"
        "olcAccess: {0}to * by users write by * read";
    
    const std::string REDOS_ACCESS_LDIF_CONTENT = 
        "dn: olcDatabase={2}mdb,cn=config\n"
        "changetype: modify\n"
        "replace: olcAccess\n"
        "olcAccess: {0}to * by users write by * read";

    const std::string DB_LDIF_CONTENT = 
        "dn: olcDatabase={2}mdb,cn=config\n"
        "changetype: modify\n"
        "replace: olcSuffix\n"
        "olcSuffix: dc=maxcrc,dc=com\n"
        "\n"
        "dn: olcDatabase={2}mdb,cn=config\n"
        "changetype: modify\n"
        "replace: olcRootDN\n"
        "olcRootDN: cn=admin,dc=maxcrc,dc=com\n"
        "\n"
        "dn: olcDatabase={2}mdb,cn=config\n"
        "changetype: modify\n"
        "replace: olcRootPW\n"
        "olcRootPW: ";
        
    const std::string EMPTY_LDIF_CONTENT = 
        "dn: dc=maxcrc,dc=com\n"
        "objectClass: domain\n"
        "objectClass: top\n"
        "dc: maxcrc";

};



#endif