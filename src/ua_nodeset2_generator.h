#ifndef UA_NODESET2_GENERATOR_H
#define UA_NODESET2_GENERATOR_H

#include "ua_model.h"
#include "xmlpp.h"
#include <memory>
#include <string>
#include <vector>

class ua_nodeset2_generator
{
public:
    ua_nodeset2_generator();

    void load_nodeset(const std::string &nodeset_file);

    void write_nodeset2_sources();

private:
    void parse_variable_type(xml_node &variable_type_node);
    void parse_object_type(xml_node &object_type_node);

private:
    std::vector<ua_node_ptr> _ua_nodes;
};

#endif
