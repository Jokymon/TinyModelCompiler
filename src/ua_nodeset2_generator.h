#ifndef UA_NODESET2_GENERATOR_H
#define UA_NODESET2_GENERATOR_H

#include "ua_model.h"
#include "xmlpp.h"
#include <memory>
#include <string>
#include <vector>

class node_code_generator : public ua_node_visitor
{
public:
    explicit node_code_generator(std::ostream &out) :
        out(out)
    {}

    void visit(ua_node &node) override
    {
        node.visit(*this);
    }

    void visit(ua_variable_type &node) override
    {
        out << std::string("    _ua_nodes.emplace_back(std::make_unique<ua_variable_type>(ua_node_id(0, 1000), \"") << node.browse_name << std::string("\"));\n");
    }

private:
    std::ostream &out;
};

class ua_nodeset2_generator
{
public:
    ua_nodeset2_generator();

    void load_nodeset(const std::string &nodeset_file);

    void write_nodeset2_sources();

private:
    void parse_variable_type(xml_node &variable_type_node);

private:
    std::vector<std::unique_ptr<ua_node>> _ua_nodes;
};

#endif
