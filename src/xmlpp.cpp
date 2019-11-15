#include "xmlpp.h"

bool operator==(const xml_node::node_iterator &lhs, const xml_node::node_iterator &rhs)
{
    return lhs._node == rhs._node;
}

bool operator!=(const xml_node::node_iterator &lhs, const xml_node::node_iterator &rhs)
{
    return lhs._node != rhs._node;
}

xml_node::node_iterator& xml_node::node_iterator::operator++()
{
    if (_node)
        _node = _node->next;
    return *this;
}

xml_node xml_node::node_iterator::operator*()
{
    return xml_node(_node);
}

xml_node::node_iterator::node_iterator(xmlNodePtr node) :
    _node(node)
{
}

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

void xml_node::set_data(const std::string &data)
{
    xmlNodeSetContent(_node, reinterpret_cast<const xmlChar*>(
                data.c_str()));
}

bool xml_node::has_attribute(const std::string &name) const
{
    return true;
}

std::string xml_node::attribute(const std::string &name) const
{
    char* attribute = (char*)xmlGetProp(_node, (xmlChar*)name.c_str());
    if (attribute)
        return std::string(attribute);
    return std::string();
}

void xml_node::set_attribute(const std::string &name,
                            const std::string &value)
{
    xmlSetProp(_node,
        reinterpret_cast<const xmlChar*>(name.c_str()),
        reinterpret_cast<const xmlChar*>(value.c_str())
    );
}

xml_node::node_iterator xml_node::begin()
{
    return node_iterator(_node->children);
}

xml_node::node_iterator xml_node::end()
{
    return node_iterator(nullptr);
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

