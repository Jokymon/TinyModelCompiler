#ifndef MODELGENERATOR_H
#define MODELGENERATOR_H

#include "ua_model.h"
#include "xmlpp.h"
#include <map>
#include <memory>
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
    void parse_variable_type(xml_node &variable_type_node);

    void generate_aliases(xml_node &aliases_node);
    void generate_nodes(xml_node &node_set);

private:
    std::vector<std::string> _namespaces;
    std::vector<std::unique_ptr<ua_node>> _ua_nodes;
    std::map<std::string, std::unique_ptr<ua_node>> _ua_nodeset2;

    const int NamespaceIndex = 1;
};

#endif
