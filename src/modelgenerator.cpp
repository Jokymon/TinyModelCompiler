#include "modelgenerator.h"
#include <iostream>

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
    auto alias1 = aliases.create_child("Alias");
    alias1.set_data("i=1");
    alias1.set_attribute("Alias", "Boolean");

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
