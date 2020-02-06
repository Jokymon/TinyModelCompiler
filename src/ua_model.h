#ifndef UA_MODEL_H
#define UA_MODEL_H

#include "fifo_map.hpp"
#include <memory>
#include <string>
#include <variant>
#include <vector>

class xml_node;

class ua_node_id
{
public:
    ua_node_id()=default;
    ua_node_id(int namespace_id, const std::string &name);
    ua_node_id(int namespace_id, int node_id);

    int namespace_id=0;
    std::variant<int, std::string> id;

    std::string to_string() const;
    static ua_node_id from_string(const std::string &s);
};

class qualified_name
{
public:
    qualified_name();
    explicit qualified_name(const std::string &name);
    explicit qualified_name(int namespace_index, const std::string &name);

    std::string to_string() const;

    int namespace_index;
    std::string name;
};

class reference
{
public:
    explicit reference(const std::string &reference_type, bool is_forward, const std::string &value);

    std::string reference_type;
    bool is_forward;
    std::string value;
};

class ua_node;
class ua_variable_type;
class ua_object_type;
class ua_variable;
class ua_object;

class ua_node_visitor
{
public:
    virtual ~ua_node_visitor()=default;
    virtual void visit(ua_node&) =0;
    virtual void visit(ua_variable_type&) =0;
    virtual void visit(ua_object_type&) =0;
    virtual void visit(ua_variable&) =0;
    virtual void visit(ua_object&) =0;
};

class ua_node
{
public:
    virtual ~ua_node()=default;
    ua_node_id node_id;
    qualified_name browse_name;
    std::string symbolic_name;

    std::vector<reference> references;

    virtual void visit(ua_node_visitor&) =0;
};

typedef std::shared_ptr<ua_node> ua_node_ptr;

class ua_type : public ua_node
{
public:
    bool is_abstract;
};

class ua_variable_type : public ua_type
{
public:
    void visit(ua_node_visitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class ua_object_type : public ua_type
{
public:
    void visit(ua_node_visitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class ua_instance : public ua_node
{
public:
    ua_node_ptr parent;
};

class ua_variable : public ua_instance
{
public:
    std::string data_type;

    void visit(ua_node_visitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class ua_object : public ua_instance
{
public:
    std::string data_type;

    void visit(ua_node_visitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class ua_model
{
    using nodeset_type = std::vector<ua_node_ptr>;
public:
    using iterator = nodeset_type::iterator;

    ua_model()=default;
    virtual ~ua_model()=default;

    void populate_from_file(const std::string &filename);
    void dump_to_file(const std::string &filename);

    void push_back(ua_node_ptr node);
    bool contains(const std::string &name);
    ua_node_ptr get_node(const std::string &name);

    void append_namespace(const std::string &namespace_uri);
    void add_alias(const std::string &alias, const ua_node_id &node_id);

    iterator begin();
    iterator end();

private:
    void parse_alias(xml_node &alias_node);
    void parse_variable_type(xml_node &variable_type_node);
    void parse_object_type(xml_node &object_type_node);
    void parse_variable(xml_node &variable_node);
    void parse_object(xml_node &object_node);

    void load_ua_node(xml_node &x_node, ua_node &u_node);
    void load_ua_type(xml_node &x_node, ua_type &u_node);

    void generate_namespaces(xml_node &root);
    void generate_aliases(xml_node &root);
    void generate_nodes(xml_node &root);

private:
    std::vector<std::string> m_namespaces;
    nlohmann::fifo_map<std::string, ua_node_id> m_aliases;
    std::map<std::string, ua_node_ptr> m_nodes;
    nodeset_type m_nodeset;
};

#endif
