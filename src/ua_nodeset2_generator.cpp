#include "ua_nodeset2_generator.h"
#include <array>
#include <fstream>
#include <iostream>

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
        std::string address_part_str;
        auto address_part = node.node_id.id;
        if (std::holds_alternative<int>(address_part))
            address_part_str = std::to_string(std::get<int>(address_part));
        else if (std::holds_alternative<std::string>(address_part))
            address_part_str = std::string("\"") + std::get<std::string>(address_part) + std::string("\"");
        
        out << std::string("    nodes[\"") << node.browse_name.to_string() << std::string("\"] = ")
                << std::string("std::make_unique<ua_variable_type>(ua_node_id(")
                << node.node_id.namespace_id << ", " << address_part_str << "), qualified_name(\""
                << node.browse_name.to_string() << std::string("\"), \"")
                << node.browse_name.to_string() << std::string("\");\n");
    }

private:
    std::ostream &out;
};

ua_nodeset2_generator::ua_nodeset2_generator() :
    _ua_nodes()
{
}

void ua_nodeset2_generator::load_nodeset(const std::string &nodeset_file)
{
    xml_document doc= xml_document::parse_file(nodeset_file);

    auto root = doc.root();
    if (!root)
    {
        std::cerr << "Empty document\n";
        return;
    }

    for (auto child : root)
    {
        if (child.name() == "UAVariableType")
            parse_variable_type(child);
    }
}

void ua_nodeset2_generator::write_nodeset2_sources()
{
    std::ofstream h_source("opc_ua_nodeset2.h");
    
    h_source << "#ifndef OPC_UA_NODESET2_H\n";
    h_source << "#define OPC_UA_NODESET2_H\n\n";
    h_source << "#include \"ua_model.h\"\n";
    h_source << "#include <map>\n";
    h_source << "#include <memory>\n";
    h_source << "#include <string>\n";

    h_source << "void populate_node_list(std::map<std::string, std::unique_ptr<ua_node>> &nodes);\n\n";

    h_source << "#endif\n";

    // -----------------------------------------------------

    std::ofstream cpp_source("opc_ua_nodeset2.cpp");

    cpp_source << "#include \"opc_ua_nodeset2.h\"\n";
    cpp_source << "#include \"ua_model.h\"\n\n";
    cpp_source << "void populate_node_list(std::map<std::string, std::unique_ptr<ua_node>> &nodes)\n";
    cpp_source << "{\n";

    node_code_generator generator(cpp_source);
    for (const auto &ua_node : _ua_nodes)
    {
        generator.visit(*ua_node.get());
    }
    
    cpp_source << "}\n";
}

void ua_nodeset2_generator::parse_variable_type(xml_node &variable_type_node)
{
    std::string node_id_str = variable_type_node.attribute("NodeId");
    auto node_id = ua_node_id::from_string(node_id_str);
    qualified_name browse_name{0, variable_type_node.attribute("BrowseName")};
    int value_rank = -1;
    if (variable_type_node.has_attribute("ValueRank"))
    {
        std::string rank = variable_type_node.attribute("ValueRank");
        // TODO: convert to int
    }
    bool is_abstract = false;
    if (variable_type_node.has_attribute("IsAbstract"))
    {
        std::string abstract = variable_type_node.attribute("IsAbstract");
        if (abstract=="true")
            is_abstract = true;
    }
    std::string display_name;

    _ua_nodes.emplace_back(std::make_unique<ua_variable_type>(node_id, browse_name, display_name));
}
