#include "ldap_setup.h"


std::string SecuritySetuper::GetEncryptedPassword(const std::string& password) {
    std::string command = "echo " + password + " | /opt/AstraRegul/Astra.Security/Utils/astra.security.crypter";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to get encrypted password." << std::endl;
        return "";
    }

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    pclose(pipe);
    std::istringstream iss(result);
    std::string word;
    while (iss >> word) {
        result = word;
    }
    return result;
}

void SecuritySetuper::BackupFile(const std::string& original_path){
    std::string backup_dir = "/etc/AstraRegul/Backups";
    std::filesystem::path orig_path =  std::filesystem::absolute(original_path);
    try {
        std::filesystem::create_directories(backup_dir);
        std::string backup_path = backup_dir + "/" + orig_path.filename().string();
        std::filesystem::copy_file(orig_path, backup_path, std::filesystem::copy_options::overwrite_existing);
    } catch (std::exception& e) {
        std::cerr << "Error creating backup: " << e.what() << std::endl;
        exit(1);
    }
}

pugi::xml_document SecuritySetuper::ParseXml(const std::string& filepath){
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str(), pugi::parse_default | pugi::parse_comments);
    if (!result) {
        std::cerr << "Error parsing XML file: " << result.description() << std::endl;
        exit(1);
    } else {
        std::cout << filepath << " parsed successfully." << std::endl;
    }
    return doc;
}


pugi::xml_node SecuritySetuper::FindTag(pugi::xml_document& doc, const std::string& tag){
    pugi::xml_node node = doc.find_node([&](pugi::xml_node& n) { return std::string(n.name()) == tag; });
    if (!node) {
        std::cerr << "Error: <" << tag << "> element not found in XML." << std::endl;
        exit(1);
    }
    return node;
}

void SecuritySetuper::UpdateAttribute(pugi::xml_node& element, const std::string& attr_name, const std::string& new_value){
    pugi::xml_attribute attr = element.attribute(attr_name.c_str());
    if (attr) {
        std::string original_value = attr.value();
        attr.set_value(new_value.c_str());
        std::cout << "Updated '" << attr_name << "' from '" << original_value << "' to '" << new_value << "'." << std::endl;
    } else {
        std::cerr << "Error: Attribute '" << attr_name << "' not found in element <" << element.name() << ">." << std::endl;
        exit(1);
    }
}

void SecuritySetuper::WriteXml(pugi::xml_document& doc, const std::string& file_path) {
    unsigned int save_flags = pugi::format_default | pugi::format_indent;

    if (!doc.save_file(file_path.c_str(), "  ", save_flags, pugi::encoding_utf8)) {
        std::cerr << "Error writing to file: " << file_path << std::endl;
        exit(1);
    }
    std::cout << "Updated '" << file_path << "' successfully." << std::endl;
}


void SecuritySetuper::RestartService(const std::string& service_name){
    std::string command = "systemctl restart " + service_name;
    int result = system(command.c_str());
    if (WIFEXITED(result) && WEXITSTATUS(result) == 0) {
        std::cout << "Service " << service_name << " restarted successfully." << std::endl;
    } else {
        std::cerr << "Failed to restart the service " << service_name << std::endl;
    }
}

void SecuritySetuper::WriteConfig(const std::string& filepath, const std::string& content){
    std::ofstream file_(filepath);
    if (!file_) {
        std::cerr << "Could not create or open the file" << std::endl;
        return;
    }
    file_ << content;
}

void SecuritySetuper::ConfigureAstraSecurityAgent(const std::string& password, const std::string& port) {
    std::string file_path = "/opt/AstraRegul/Astra.Security/astra.security.agent.xml";
    BackupFile(file_path);
    pugi::xml_document doc = ParseXml(file_path);

    pugi::xml_node entry_point_tag = FindTag(doc, "EntryPointNetAgent");
    UpdateAttribute(entry_point_tag, "Address", "127.0.0.1");
    UpdateAttribute(entry_point_tag, "Port", port);

    pugi::xml_node ldap_password_tag = FindTag(doc, "LdapPassword");

    UpdateAttribute(ldap_password_tag, "value", GetEncryptedPassword(password));

    WriteXml(doc, file_path);
}

void SecuritySetuper::ConfigureAstraSecurityUseractivity() {
    const char* sudo_user = getenv("SUDO_USER");
    if (!sudo_user) {
        std::cerr << "Error: SUDO_USER environment variable not set." << std::endl;
        exit(1);
    }
    std::string current_user = sudo_user;

    const char* display_env = getenv("DISPLAY");
    if (!display_env) {
        std::cerr << "No DISPLAY value found." << std::endl;
        exit(1);
    }
    std::string display = display_env;

    std::string file_path = "/opt/AstraRegul/Astra.Security/astra.security.useractivity.sh";
    BackupFile(file_path);

    std::ifstream infile(file_path);
    if (!infile.is_open()) {
        std::cerr << "Error opening file " << file_path << std::endl;
        exit(1);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find("export DISPLAY=") != std::string::npos) {
            line = "export DISPLAY=\"" + display + "\"";
        }
        if (line.find("user1") != std::string::npos) {
            size_t pos = line.find("user1");
            if (pos != std::string::npos) {
                line.replace(pos, 5, current_user);
            }
        }
        lines.push_back(line);
    }
    infile.close();

    std::ofstream outfile(file_path);
    if (!outfile.is_open()) {
        std::cerr << "Error writing to file " << file_path << std::endl;
        exit(1);
    }
    for (const auto& l : lines) {
        outfile << l << std::endl;
    }
    outfile.close();

    RestartService("astra.security.useractivity");
    std::cout << "Configured astra.security.useractivity" << std::endl;
}

void SecuritySetuper::ConfigureMonitorServices() {
    std::string command = "cd /opt/AstraRegul/Astra.Security/ && sh ./monitor.export.sh && sh ./addLockTime.sh";
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error while configuring monitor services." << std::endl;
    }
}

void SecuritySetuper::RedosConfigureLDAP(){
    std::string command = "slappasswd -s " + PASSWORD;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to get encrypted password." << std::endl;
    }

    char buffer[128];
    std::string crypted_pass;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        crypted_pass += buffer;
    }
    pclose(pipe);
    std::cout << "Configuring config.ldif" << std::endl;
    WriteConfig("/opt/AstraRegul/Astra.Security/config.ldif", REDOS_CONFIG_LDIF_CONTENT + crypted_pass);
    system("cd /opt/AstraRegul/Astra.Security/ && sudo ldapadd -Y EXTERNAL -H ldapi:/// -f config.ldif");
    std::cout << "Configuring access.ldif" << std::endl;
    WriteConfig("/opt/AstraRegul/Astra.Security/access.ldif", REDOS_ACCESS_LDIF_CONTENT);
    system("cd /opt/AstraRegul/Astra.Security/ && sudo ldapadd -Y EXTERNAL -H ldapi:/// -f access.ldif");
    std::cout << "Configuring db.ldif" << std::endl;
    WriteConfig("/opt/AstraRegul/Astra.Security/db.ldif", DB_LDIF_CONTENT + crypted_pass);
    system("cd /opt/AstraRegul/Astra.Security/ && sudo ldapadd -Y EXTERNAL -H ldapi:/// -f db.ldif");
    std::cout << "Configuring schemas" << std::endl;
    system("cd /etc/openldap/schema/ &&"
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f collective.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f corba.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f cosine.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f duaconf.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f dyngroup.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f inetorgperson.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f java.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f misc.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f nis.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f openldap.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f pmi.ldif && "
           "sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f ppolicy.ldif");
    std::cout << "Configuring astra.security.ldif" << std::endl;
    system("cd /opt/AstraRegul/Astra.Security/ && sudo ldapadd -Q -Y EXTERNAL -H ldapi:/// -f astra.security.ldif");
    std::cout << "Configuring empty.ldif" << std::endl;
    WriteConfig("/opt/AstraRegul/Astra.Security/empty.ldif", EMPTY_LDIF_CONTENT);
    system("cd /opt/AstraRegul/Astra.Security/ && sudo ldapadd -Y EXTERNAL -H ldapi:/// -f empty.ldif");

}

void SecuritySetuper::AstraConfigureLDAP(){
    GetEncryptedPassword("secret");
    setenv("DEBIAN_FRONTEND", "noninteractive", 1);

    
    int result = system(SLAPD_RECONFIGURE_COMMAND.c_str());
    if (WIFEXITED(result) && WEXITSTATUS(result) == 0) {
        RestartService("slapd");
        system("ufw allow ldap");
    } else {
        std::cerr << "Error while configuring slapd." << std::endl;
    }

    std::string command = "cd /opt/AstraRegul/Astra.Security && sudo sh ./astra.security.schema.export.sh";
    result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error while exporting schema." << std::endl;
    }
    std::cout << "Schema exported" << std::endl;
    RestartService("slapd");

    WriteConfig("/opt/AstraRegul/Astra.Security/access.ldif",ASTRA_ACCESS_LDIF_CONTENT);
    command = "cd /opt/AstraRegul/Astra.Security/ && ldapadd -Y EXTERNAL -H ldapi:/// -f access.ldif";
    result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Error while adding access.ldif." << std::endl;
    }
    result = system("systemctl enable slapd");
    if (WIFEXITED(result) && WEXITSTATUS(result) == 0) {
        std::cout << "Service slapd enabled successfully." << std::endl;
    } else {
        std::cerr << "Failed to enable the service slapd." << std::endl;
    }
    RestartService("slapd");
    std::cout << "Configured slapd" << std::endl;
} 

void SecuritySetuper::ConfigureAstraDomainAgent(const std::string& name, const std::string& port) {
    std::string file_path = "/opt/AstraRegul/Astra.Domain/astra.domain.agent.xml";
    BackupFile(file_path);
    pugi::xml_document doc = ParseXml(file_path);

    pugi::xml_node entry_point_tag = FindTag(doc, "EntryPointNetAgent");
    UpdateAttribute(entry_point_tag, "Name", name);
    UpdateAttribute(entry_point_tag, "Address", "127.0.0.1");
    UpdateAttribute(entry_point_tag, "Port", port);

    WriteXml(doc, file_path);
    RestartService("astra.domain");

    std::cout << "Configured astra.domain" << std::endl;
}

void SecuritySetuper::ConfigureAstraNetAgent(const std::string& name, const std::string& port){
    std::string file_path = "/opt/AstraRegul/Astra.Domain/astra.net.agent.xml";
    BackupFile(file_path);
    pugi::xml_document doc = ParseXml(file_path);

    pugi::xml_node net_tag = FindTag(doc, "Astra.Net.Agent");
    UpdateAttribute(net_tag, "Name", name);
    UpdateAttribute(net_tag, "NetEnterPort", port);
    UpdateAttribute(net_tag, "ParentAgentPort", "1010");

    WriteXml(doc, file_path);
    RestartService("astra.net");

    std::cout << "Configured astra.net" << std::endl;
}

void SecuritySetuper::SetupRedos(const std::string& name, const std::string& port){
    ConfigureAstraSecurityAgent(PASSWORD, port);
    ConfigureAstraSecurityUseractivity();
    ConfigureMonitorServices();

    ConfigureAstraDomainAgent(name, port);
    ConfigureAstraNetAgent(name, port);

    RedosConfigureLDAP();
    RestartService("astra.security.service");
}

void SecuritySetuper::SetupAstra(const std::string& name, const std::string& port){
    ConfigureAstraSecurityAgent(PASSWORD, port);
    ConfigureAstraSecurityUseractivity();
    ConfigureMonitorServices();
    ConfigureAstraDomainAgent(name, port);
    ConfigureAstraNetAgent(name, port);
    AstraConfigureLDAP();
    RestartService("astra.security.service");
    
}