#ifndef MODELGENERATOR_H
#define MODELGENERATOR_H

#include "xmlpp.h"
#include <string>
#include <vector>

class model_generator
{
public:
    model_generator();

    void load_model(const std::string &model_file);

    void write_nodeset2();

private:
    void parse_namespaces(xml_node &namespaces_node);
    void generate_aliases(xml_node &aliases_node);

private:
    std::vector<std::string> _namespaces;
};

#endif
