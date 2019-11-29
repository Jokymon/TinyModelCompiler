#ifndef UA_MODEL_H
#define UA_MODEL_H

#include <memory>
#include <string>
#include <variant>
#include <vector>

class xml_node;

class ua_node_id
{
public:
    ua_node_id(int namespace_id, const std::string &name);
    ua_node_id(int namespace_id, int node_id);

    int namespace_id;
    std::variant<int, std::string> id;

    std::string to_string() const;
    static ua_node_id from_string(const std::string &s);
};

class qualified_name
{
public:
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
    virtual void visit(ua_node&) =0;
    virtual void visit(ua_variable_type&) =0;
    virtual void visit(ua_object_type&) =0;
    virtual void visit(ua_variable&) =0;
    virtual void visit(ua_object&) =0;
};

class ua_node
{
public:
    ua_node(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &display_name);

    ua_node_id node_id;
    qualified_name browse_name;
    std::string display_name;

    std::vector<reference> references;

    virtual void visit(ua_node_visitor&) =0;
};

typedef std::shared_ptr<ua_node> ua_node_ptr;

template<class T>
class visitable_ua_node : public ua_node
{
public:
    visitable_ua_node(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &display_name) :
        ua_node(node_id, browse_name, display_name)
    {
    }

    void visit(ua_node_visitor& visitor) override
    {
        visitor.visit(static_cast<T&>(*this));
    }
};

class ua_variable_type : public visitable_ua_node<ua_variable_type>
{
public:
    ua_variable_type(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &display_name);
};

class ua_object_type : public visitable_ua_node<ua_object_type>
{
public:
    ua_object_type(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &display_name);
};

class ua_variable : public visitable_ua_node<ua_variable>
{
public:
    ua_variable(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &display_name);

    ua_node_ptr parent;
    std::string data_type;
};

class ua_object : public visitable_ua_node<ua_object>
{
public:
    ua_object(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &display_name);

    ua_node_ptr parent;
    std::string data_type;
};

class ua_model
{
    using nodeset_type = std::vector<ua_node_ptr>;
public:
    using iterator = nodeset_type::iterator;

    ua_model();

    void populate_from_file(const std::string &filename);

    void push_back(ua_node_ptr node);

    iterator begin();
    iterator end();

private:
    void parse_variable_type(xml_node &variable_type_node);
    void parse_object_type(xml_node &object_type_node);
    void parse_variable(xml_node &variable_node);
    void parse_object(xml_node &object_node);

private:
    nodeset_type _nodeset;
};

#endif
