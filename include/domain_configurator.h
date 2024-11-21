#ifndef domain_configurator.h
#define domain_configurator .h

#include <memory>
#include <pugixml.hpp>
#include <unordered_map>

#include "station.h"

class DomainConfigurator {
   public:
    DomainConfigurator(const MainStation& station);

    void CreateXML();

   private:

    std::unordered_map<std::shared_ptr<Station>, pugi::xml_document> xml_docs;
};

#endif