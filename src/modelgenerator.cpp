#include "modelgenerator.h"
#include <array>
#include <iostream>

struct alias {
    std::string alias;
    int id;
};

const std::array<alias, 35> Aliases = {{
    { "Boolean", 1 },
    { "SByte", 2 },
    { "Byte", 3 },
    { "Int16", 4 },
    { "UInt16", 5 },
    { "Int32", 6 },
    { "UInt32", 7 },
    { "Int64", 8 },
    { "UInt64", 9 },
    { "Float", 10 },
    { "Double", 11 },
    { "DateTime", 13 },
    { "String", 12 },
    { "ByteString", 15 },
    { "Guid", 14 },
    { "XmlElement", 16 },
    { "NodeId", 17 },
    { "ExpandedNodeId", 18 },
    { "QualifiedName", 20 },
    { "LocalizedText", 21 },
    { "StatusCode", 19 },
    { "Structure", 22 },
    { "Number", 26 },
    { "Integer", 27 },
    { "UInteger", 28 },
    { "HasComponent", 47 },
    { "HasProperty", 46 },
    { "Organizes", 35 },
    { "HasEventSource", 36 },
    { "HasNotifier", 48 },
    { "HasSubtype", 45 },
    { "HasTypeDefinition", 40 },
    { "HasModellingRule", 37 },
    { "HasEncoding", 38 },
    { "HasDescription", 39 }
}};

class node_set_generator : public ua_node_visitor
{
public:
    explicit node_set_generator(xml_node &node_set) :
        node_set(node_set)
    {}

    void visit(ua_node &node) override
    {
        node.visit(*this);
    }

    void visit(ua_variable_type &node) override
    {
        auto variable_node = node_set.create_child("UAVariableType");
        variable_node.set_attribute("ValueRank", "-2");

        auto display_name = variable_node.create_child("DisplayName");
        display_name.set_data(node.display_name);
    }

private:
    xml_node &node_set;
};

model_generator::model_generator() :
    _namespaces()
{
}

void model_generator::load_model(const std::string &model_file)
{
    xml_document doc= xml_document::parse_file(model_file);

    auto root = doc.root();
    if (!root)
    {
        std::cerr << "Empty document\n";
        return;
    }

    for (auto child : root)
    {
        if (child.name() == "Namespaces")
            parse_namespaces(child);
        else if (child.name() == "VariableType")
            parse_variable_type(child);
    }
}

void model_generator::write_nodeset2()
{
    xml_document doc = xml_document::create("http://opcfoundation.org/UA/2011/03/UANodeSet.xsd",
                                            "UaNodeSet");
    auto root = doc.root();
    auto ns_uris = root.create_child("NamespaceUris");
    for (const auto &url : _namespaces)
    {
        auto ns_uri = ns_uris.create_child("Uri");
        ns_uri.set_data(url);
    }

    auto aliases = doc.root().create_child("Aliases");
    generate_aliases(aliases);
    generate_nodes(root);

    doc.dump_file("MemoryBuffer.NodeSet2.xml");
}

void model_generator::parse_namespaces(xml_node &namespaces_node)
{
    for (auto namespace_node : namespaces_node)
    {
        if (namespace_node.name() != "Namespace")
            continue;

        std::string url = namespace_node.data();
        _namespaces.push_back(url);
    }
}

void model_generator::parse_variable_type(xml_node &variable_type_node)
{
    std::string symbolic_name = variable_type_node.attribute("SymbolicName");
    std::string base_type = variable_type_node.attribute("BaseType");

    if (base_type != "ua:BaseDataVariableType")
    {
        std::cerr << "Variable type definition with unknown BaseType '" << base_type << "'\n";
        return;
    }

    _ua_nodes.emplace_back(std::make_unique<ua_variable_type>(ua_node_id(1, 1000), symbolic_name));

    std::cout << "Name=" << symbolic_name << std::endl;
    std::cout << "BaseType=" << base_type << std::endl;
}

void model_generator::generate_aliases(xml_node &aliases_node)
{
    for (const auto &alias : Aliases)
    {
        auto alias_node = aliases_node.create_child("Alias");
        alias_node.set_data("i="+std::to_string(alias.id));
        alias_node.set_attribute("Alias", alias.alias);
    }
}

void model_generator::generate_nodes(xml_node &node_set)
{
    node_set_generator generator(node_set);
    for (const auto &ua_node : _ua_nodes)
    {
        generator.visit(*ua_node.get());
    }
}
