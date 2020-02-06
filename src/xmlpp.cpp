#include "xmlpp.h"

qname::qname(const std::string &qname_string)
{
    xmlChar *local_part_xc = nullptr;
    xmlChar *prefix_xc = nullptr;

    local_part_xc = xmlSplitQName2(reinterpret_cast<const xmlChar*>(qname_string.c_str()), &prefix_xc);

    if (local_part_xc != nullptr)
    {
        local_part = std::string(reinterpret_cast<char*>(local_part_xc));
        xmlFree(local_part_xc);
    }

    if (prefix_xc != nullptr)
    {
        prefix = std::string(reinterpret_cast<char*>(prefix_xc));
        xmlFree(prefix_xc);
    }
}

std::string qname::to_string() const
{
    if (prefix.empty())
        return local_part;

    return prefix+":"+local_part;
}

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
    m_node(node)
{
}

std::string xml_node::name() const
{
    return std::string(reinterpret_cast<const char*>(m_node->name));
}

std::string xml_node::data() const
{
    return std::string(reinterpret_cast<const char*>(
                xmlNodeGetContent(m_node))
                );
}

void xml_node::set_data(const std::string &data)
{
    xmlNodeSetContent(m_node, reinterpret_cast<const xmlChar*>(
                data.c_str()));
}

bool xml_node::has_attribute(const std::string &/*name*/) const
{
    return true;
}

std::string xml_node::attribute(const std::string &name) const
{
    char* attribute = (char*)xmlGetProp(m_node, (xmlChar*)name.c_str());
    if (attribute)
        return std::string(attribute);
    return std::string();
}

void xml_node::set_attribute(const std::string &name,
                            const std::string &value)
{
    xmlSetProp(m_node,
        reinterpret_cast<const xmlChar*>(name.c_str()),
        reinterpret_cast<const xmlChar*>(value.c_str())
    );
}

xml_node::node_iterator xml_node::begin()
{
    return node_iterator(m_node->children);
}

xml_node::node_iterator xml_node::end()
{
    return node_iterator(nullptr);
}

xml_node xml_node::create_child(const std::string &tag_name)
{
    xml_node new_node(
            xmlNewChild(
                m_node,
                nullptr,
                (xmlChar*)tag_name.c_str(),
                nullptr)
            );
    return new_node;
}

xml_node::operator bool() const
{
    return m_node != nullptr;
}

xml_document::~xml_document()
{
    if (m_doc)
    {
        xmlFreeDoc(m_doc);
        m_doc = nullptr;
    }
}

bool xml_document::dump_file(const std::string &filename)
{
    int result = xmlSaveFormatFileEnc(filename.c_str(),
                                      m_doc,
                                      "utf-8",
                                      1);
    return (result > 0);
}

xml_node xml_document::root() const
{
    return xml_node(xmlDocGetRootElement(m_doc));
}

const std::vector<ns_definition>& xml_document::namespaces() const
{
    return m_namespaces;
}

xml_document xml_document::parse_file(const std::string &filename)
{
    xml_document new_doc;
    new_doc.m_doc = xmlParseFile(filename.c_str());
    // TODO: handle case when _doc is now NULL

    auto root = new_doc.root();
    auto ns_ptr = root.m_node->nsDef;
    while (ns_ptr)
    {
        std::string prefix;
        if (ns_ptr->prefix)
            prefix = std::string(reinterpret_cast<const char*>(ns_ptr->prefix));
        std::string href;
        if (ns_ptr->href)
            href = std::string(reinterpret_cast<const char*>(ns_ptr->href));

        new_doc.m_namespaces.emplace_back(prefix, href);
        ns_ptr = ns_ptr->next;
    }

    return new_doc;
}

xml_document xml_document::create(const std::string &namespaceUri, const std::string &qualifiedName)
{
    xml_document new_doc;
    new_doc.m_doc = xmlNewDoc((xmlChar*)"1.0");

    auto root_node = xmlNewNode(nullptr, (xmlChar*)qualifiedName.c_str());
    xmlDocSetRootElement(new_doc.m_doc, root_node);

    if (!namespaceUri.empty())
    {
        xmlNsPtr ns = xmlNewNs(root_node, (xmlChar*)namespaceUri.c_str(), nullptr);
        xmlSetNs(root_node, ns);
    }

    return new_doc;
}

xml_document::xml_document() :
    m_doc(nullptr)
{
}

