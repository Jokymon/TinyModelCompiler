#ifndef XMLPP_H
#define XMLPP_H

#include <libxml/xmlmemory.h>
#include <string>

class xml_node {
public:
    xml_node() =delete;

    std::string name() const;
    std::string data() const;
    void set_data(const std::string &data);

    bool has_attribute(const std::string &name) const;
    std::string attribute(const std::string &name) const;
    void set_attribute(const std::string &name,
                        const std::string &value);

    class node_iterator {
    public:
        friend bool operator==(const node_iterator &lhs,
                               const node_iterator &rhs);
        friend bool operator!=(const node_iterator &lhs,
                               const node_iterator &rhs);
        node_iterator& operator++();
        xml_node operator*();
    
    private:
        node_iterator(xmlNodePtr node);
    
        xmlNodePtr _node;
    
    friend class xml_node;
    };

    node_iterator begin();
    node_iterator end();

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
