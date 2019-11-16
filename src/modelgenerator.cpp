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
    }
}

void model_generator::write_nodeset2()
{
    xml_document doc = xml_document::create("http://opcfoundation.org/UA/2011/03/UANodeSet.xsd",
                                            "UaNodeSet");
    auto ns_uris = doc.root().create_child("NamespaceUris");
    for (const auto &url : _namespaces)
    {
        auto ns_uri = ns_uris.create_child("Uri");
        ns_uri.set_data(url);
    }

    auto aliases = doc.root().create_child("Aliases");
    generate_aliases(aliases);

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

void model_generator::generate_aliases(xml_node &aliases_node)
{
    for (const auto &alias : Aliases)
    {
        auto alias_node = aliases_node.create_child("Alias");
        alias_node.set_data("i="+std::to_string(alias.id));
        alias_node.set_attribute("Alias", alias.alias);
    }
}
