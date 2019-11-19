#include "ua_model.h"

ua_node_id::ua_node_id(int namespace_id, const std::string &name) :
    namespace_id(namespace_id),
    id(name)
{
}

ua_node_id::ua_node_id(int namespace_id, int node_id) :
    namespace_id(namespace_id),
    id(node_id)
{
}

ua_node::ua_node(const ua_node_id& node_id, const std::string &display_name) :
    node_id(node_id),
    display_name(display_name)
{
}

ua_variable_type::ua_variable_type(const ua_node_id& node_id, const std::string &display_name) :
    visitable_ua_node(node_id, display_name)
{
}
