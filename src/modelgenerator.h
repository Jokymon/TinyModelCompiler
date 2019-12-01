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
    void parse_object_type(xml_node &object_type_node);
    ua_node_ptr parse_property(xml_node &property_node, ua_node_ptr &parent);
    void parse_object(xml_node &object_node);

    ua_node_ptr& get_node(const std::string &browse_name);

private:
    ua_model _ua_nodes;
    std::map<std::string, ua_node_ptr> _ua_nodeset2;

    const int NamespaceIndex = 1;
};

#endif
