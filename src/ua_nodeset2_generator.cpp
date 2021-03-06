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
        generate_code(node, "ua_variable_type");
    }

    void visit(ua_object_type &node) override
    {
        generate_code(node, "ua_object_type");
    }

    void visit(ua_variable &node) override
    {
    }

    void visit(ua_object &node) override
    {
        generate_code(node, "ua_object");
    }

private:
    std::string key_name(const ua_node &node)
    {
        if (!node.symbolic_name.empty())
            return node.symbolic_name;
        return node.browse_name.to_string();
    }

    void generate_code(ua_node &node, const std::string &node_type)
    {
        std::string address_part_str;
        auto address_part = node.node_id.id;
        if (std::holds_alternative<int>(address_part))
            address_part_str = std::to_string(std::get<int>(address_part));
        else if (std::holds_alternative<std::string>(address_part))
            address_part_str = std::string("\"") + std::get<std::string>(address_part) + std::string("\"");
        
        out << std::string("    nodes[\"") << key_name(node) << std::string("\"] = ")
                << std::string("std::make_shared<") << node_type << std::string(">();\n");
        out << std::string("    nodes[\"") << key_name(node) << std::string("\"]->node_id = ")
                << std::string("ua_node_id(") << node.node_id.namespace_id << ", " << address_part_str << ");\n";
        out << std::string("    nodes[\"") << key_name(node) << std::string("\"]->browse_name = ")
                << std::string("qualified_name(\"") << node.browse_name.to_string() << std::string("\");\n");
        out << std::string("    nodes[\"") << key_name(node) << std::string("\"]->symbolic_name = ")
                << std::string("\"") << node.symbolic_name << std::string("\";\n");
    }

private:
    std::ostream &out;
};

void ua_nodeset2_generator::load_nodeset(const std::string &nodeset_file)
{
    _nodeset2.populate_from_file(nodeset_file);
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

    h_source << "void populate_node_list(std::map<std::string, ua_node_ptr> &nodes);\n\n";

    h_source << "#endif\n";

    // -----------------------------------------------------

    std::ofstream cpp_source("opc_ua_nodeset2.cpp");

    cpp_source << "#include \"opc_ua_nodeset2.h\"\n";
    cpp_source << "#include \"ua_model.h\"\n\n";
    cpp_source << "void populate_node_list(std::map<std::string, ua_node_ptr> &nodes)\n";
    cpp_source << "{\n";

    node_code_generator generator(cpp_source);
    for (const auto &ua_node : _nodeset2)
    {
        generator.visit(*ua_node.get());
    }
    
    cpp_source << "}\n";
}

