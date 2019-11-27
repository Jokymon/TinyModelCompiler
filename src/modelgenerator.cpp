#include "modelgenerator.h"
#include "opc_ua_nodeset2.h"
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

static bool ends_with(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
}

static bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

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
        generate_base_attributes(node, variable_node);

        variable_node.set_attribute("ValueRank", "-2");
    }

    void visit(ua_object_type &node) override
    {
        auto object_node = node_set.create_child("UAObjectType");
        generate_base_attributes(node, object_node);
    }

    void visit(ua_variable &node) override
    {
        auto variable_node = node_set.create_child("UAVariable");
        generate_base_attributes(node, variable_node);

        if (node.parent)
        {
            variable_node.set_attribute("ParentNodeId", node.parent->node_id.to_string());
            variable_node.set_attribute("DataType", node.data_type);
        }
    }

    void visit(ua_object &node) override
    {
    }

private:
    void generate_base_attributes(ua_node &u_node, xml_node &x_node)
    {
        x_node.set_attribute("NodeId", u_node.node_id.to_string());
        x_node.set_attribute("BrowseName", u_node.browse_name.to_string());

        auto display_name = x_node.create_child("DisplayName");
        display_name.set_data(u_node.display_name);

        if (u_node.references.size()>0)
        {
            auto references_node = x_node.create_child("References");
            for (auto &reference : u_node.references)
            {
                auto reference_node = references_node.create_child("Reference");
                reference_node.set_attribute("ReferenceType", reference.reference_type);
                if (!reference.is_forward)
                    reference_node.set_attribute("IsForward", "false");
                reference_node.set_data(reference.value);
            }
        }
    }

private:
    xml_node &node_set;
};

model_generator::model_generator() :
    _namespaces(),
    _ua_nodes(),
    _ua_nodeset2()
{
    populate_node_list(_ua_nodeset2);
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
        else if (child.name() == "ObjectType")
            parse_object_type(child);
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

    auto new_node = std::make_shared<ua_variable_type>(ua_node_id(NamespaceIndex, 1000), qualified_name(NamespaceIndex, symbolic_name), symbolic_name);
    auto &reference_node = get_node(base_type);
    new_node->references.emplace_back("HasSubtype", false, reference_node->node_id.to_string());

    _ua_nodes.emplace_back(new_node);
}

void model_generator::parse_object_type(xml_node &object_type_node)
{
    std::string symbolic_name = object_type_node.attribute("SymbolicName");
    std::string base_type = object_type_node.attribute("BaseType");

    ua_node_ptr new_node = std::make_shared<ua_object_type>(ua_node_id(NamespaceIndex, 1000), qualified_name(NamespaceIndex, symbolic_name), symbolic_name);
    auto &reference_node = get_node(base_type);
    new_node->references.emplace_back("HasSubtype", false, reference_node->node_id.to_string());
    _ua_nodes.emplace_back(new_node);

    for (auto child : object_type_node)
    {
        if (child.name() == "Children")
        {
            for (auto grand_child : child)
            {
                if (grand_child.name() != "Property")
                    continue;

                auto new_property = parse_property(grand_child, new_node);
                new_node->references.emplace_back("HasProperty", true, new_property->node_id.to_string());
            }
        }
    }
}

ua_node_ptr model_generator::parse_property(xml_node &property_node, ua_node_ptr &parent)
{
    std::string symbolic_name = property_node.attribute("SymbolicName");

    auto new_node = std::make_shared<ua_variable>(ua_node_id(NamespaceIndex, 1000), qualified_name(NamespaceIndex, symbolic_name), symbolic_name);
    new_node->parent = parent;
    new_node->data_type = property_node.attribute("DataType");
    new_node->data_type = new_node->data_type.substr(3);

    new_node->references.emplace_back("HasTypeDefinition", true, get_node("PropertyType")->node_id.to_string());
    new_node->references.emplace_back("HasModellingRule", true, get_node("Mandatory")->node_id.to_string());
    new_node->references.emplace_back("HasProperty", false, parent->node_id.to_string());

    _ua_nodes.emplace_back(new_node);

    return new_node;
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

ua_node_ptr& model_generator::get_node(const std::string &browse_name)
{
    std::string find_name = browse_name;
    if (starts_with(find_name, "ua:"))
    {
        find_name = find_name.substr(3);
    }

    if (_ua_nodeset2.find(find_name) == _ua_nodeset2.end())
    {
        throw std::invalid_argument("Unknown reference");
    }
    return _ua_nodeset2[find_name];
}

