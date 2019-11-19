#ifndef UA_MODEL_H
#define UA_MODEL_H

#include <string>
#include <variant>

class ua_node_id
{
public:
    ua_node_id(int namespace_id, const std::string &name);
    ua_node_id(int namespace_id, int node_id);

    int namespace_id;
    std::variant<int, std::string> id;

    std::string to_string() const;
};

class ua_node;
class ua_variable_type;

class ua_node_visitor
{
public:
    virtual void visit(ua_node&) =0;
    virtual void visit(ua_variable_type&) =0;
};

class ua_node
{
public:
    ua_node(const ua_node_id& node_id, const std::string &display_name);

    ua_node_id node_id;
    std::string display_name;

    virtual void visit(ua_node_visitor&) =0;
};

template<class T>
class visitable_ua_node : public ua_node
{
public:
    visitable_ua_node(const ua_node_id& node_id, const std::string &display_name) :
        ua_node(node_id, display_name)
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
    ua_variable_type(const ua_node_id& node_id, const std::string &display_name);
};

#endif
