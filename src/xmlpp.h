#ifndef XMLPP_H
#define XMLPP_H

#include <libxml/xmlmemory.h>
#include <string>

class xml_node {
public:
    xml_node() =delete;

    std::string name() const;
    std::string data() const;

    xml_node first_child() const;
    xml_node next_sibling() const;

    xml_node create_child(const std::string &tag_name);

    operator bool() const;

private:
    xml_node(xmlNodePtr node);

private:
    xmlNodePtr _node;

    friend class xml_document;
};

class xml_document {
public:
    ~xml_document();

    bool dump_file(const std::string &filename);

    xml_node root() const;

    static xml_document parse_file(const std::string &filename);
    static xml_document create(const std::string &namespaceUri, const std::string &qualifiedName/*, doctype*/);

private:
    xml_document();

private:
    xmlDocPtr _doc;
};

#endif
