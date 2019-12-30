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

    void write_nodeset2(const std::string &output_file);

private:
    void parse_namespaces(xml_node &namespaces_node);
    void parse_variable_type(xml_node &variable_type_node);
    void parse_object_type(xml_node &object_type_node);
    ua_node_ptr parse_property(xml_node &property_node, ua_node_ptr &parent);
    void parse_object(xml_node &object_node);

    void resolve_references();

private:
    std::string _target_namespace;
    std::map<std::string, std::string> _prefix_for_ns;
    ua_model _ua_nodes;
    std::map<std::string, ua_node_ptr> _ua_nodeset2;

    const int NamespaceIndex = 1;
};

#endif
