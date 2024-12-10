#include "ldap_setup.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <name>  <port>" << std::endl;
        return 1;
    }

    std::string name = argv[1];
    std::string port = argv[2];

    SecuritySetuper setuper;
#ifdef ASTRA
    setuper.SetupAstra(name, port);
#elif REDOS
    setuper.SetupRedos(name, port);
#endif

    return 0;
}
