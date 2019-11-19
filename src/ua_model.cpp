#include "ua_model.h"

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

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

std::string ua_node_id::to_string() const
{
    std::string str;
    if (namespace_id != 0)
    {
        str += "ns=" + std::to_string(namespace_id) + ";";
    }
    std::visit([&str](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, int>)
            str += "i=" + std::to_string(arg);
        else if constexpr (std::is_same_v<T, std::string>)
            str += "s=" + arg;
    }, id);

    return str;
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
