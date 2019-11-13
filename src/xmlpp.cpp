#include "xmlpp.h"

xml_node::xml_node(xmlNodePtr node) :
    _node(node)
{
}

std::string xml_node::name() const
{
    return std::string(reinterpret_cast<const char*>(_node->name));
}

std::string xml_node::data() const
{
    return std::string(reinterpret_cast<const char*>(
                xmlNodeGetContent(_node))
                );
}

xml_node xml_node::first_child() const
{
    return xml_node(_node->children);
}

xml_node xml_node::next_sibling() const
{
    return xml_node(_node->next);
}

xml_node xml_node::create_child(const std::string &tag_name)
{
    xml_node new_node(
            xmlNewChild(
                _node,
                nullptr,
                (xmlChar*)tag_name.c_str(),
                nullptr)
            );
    return new_node;
}

xml_node::operator bool() const
{
    if (!_node)
        return false;
    return true;
}

xml_document::~xml_document()
{
    if (_doc)
    {
        xmlFreeDoc(_doc);
        _doc = nullptr;
    }
}

bool xml_document::dump_file(const std::string &filename)
{
    int result = xmlSaveFormatFileEnc(filename.c_str(),
                                      _doc,
                                      "utf-8",
                                      1);
    if (result > 0)
        return true;
    return false;
}

xml_node xml_document::root() const
{
    return xml_node(xmlDocGetRootElement(_doc));
}

xml_document xml_document::parse_file(const std::string &filename)
{
    xml_document new_doc;
    new_doc._doc = xmlParseFile(filename.c_str());
    // TODO: handle case when _doc is now NULL
    return new_doc;
}

xml_document xml_document::create(const std::string &namespaceUri, const std::string &qualifiedName)
{
    xml_document new_doc;
    new_doc._doc = xmlNewDoc((xmlChar*)"1.0");

    auto root_node = xmlNewNode(nullptr, (xmlChar*)qualifiedName.c_str());
    xmlDocSetRootElement(new_doc._doc, root_node);

    if (!namespaceUri.empty())
    {
        xmlNsPtr ns = xmlNewNs(root_node, (xmlChar*)namespaceUri.c_str(), nullptr);
        xmlSetNs(root_node, ns);
    }

    return new_doc;
}

xml_document::xml_document() :
    _doc(nullptr)
{
}

