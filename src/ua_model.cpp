#include "ua_model.h"
#include <exception>
#include <iostream>

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

// --------------------------------------------------------------
// parsing

char look_ahead(const std::string &s, int &pos)
{
    if (pos < s.length())
        return s[pos];
    return 255;
}

bool is_digit(char ch)
{
    return ((ch >= '0') && (ch <= '9'));
}

void match(const std::string &s, int &pos, char expected)
{
    if (s[pos] == expected)
    {
        pos++;
        return;
    }

    std::string exception_text = std::string("Expected character '")+expected+"' at position "+std::to_string(pos);
    throw std::invalid_argument(exception_text);
}

void skip(const std::string &s, int &pos)
{
    if (pos < s.length())
    {
        pos++;
        return;
    }

    throw std::invalid_argument("Character missing at the end of the string");
}

int parse_namespace(const std::string &s, int &pos)
{
    int namespace_id = 0;

    match(s, pos, 'n');
    match(s, pos, 's');
    match(s, pos, '=');

    char next_ch = look_ahead(s, pos);
    while (is_digit(next_ch))
    {
        skip(s, pos);
        namespace_id = (10 * namespace_id) + (next_ch - '0');
        next_ch = look_ahead(s, pos);
    }

    match(s, pos, ';');

    return namespace_id;
}

std::variant<int, std::string> parse_address(const std::string &s, int &pos)
{
    if (look_ahead(s, pos) == 'i')
    {
        match(s, pos, 'i');
        match(s, pos, '=');

        int address_id = 0;
        char next_ch = look_ahead(s, pos);
        while (is_digit(next_ch))
        {
            skip(s, pos);
            address_id = (10 * address_id) + (next_ch - '0');
            next_ch = look_ahead(s, pos);
        }
        return address_id;
    }
    else if (look_ahead(s, pos) == 's')
    {
        match(s, pos, 's');
        match(s, pos, '=');

        return std::string();
    }
    else
        throw std::invalid_argument("Address part startnot supported: "+look_ahead(s, pos));
}

ua_node_id parse_node_id(const std::string &s)
{
    int namespace_id = 0;

    int pos=0;
    if (look_ahead(s, pos) == 'n')
    {
        namespace_id = parse_namespace(s, pos);
    }
    auto address = parse_address(s, pos);

    if (std::holds_alternative<int>(address))
        return ua_node_id(namespace_id, std::get<int>(address));
    else if (std::holds_alternative<std::string>(address))
        return ua_node_id(namespace_id, std::get<std::string>(address));
}

// --------------------------------------------------------------

ua_node_id ua_node_id::from_string(const std::string &s)
{
    return parse_node_id(s);
}

ua_node::ua_node(const ua_node_id& node_id, const std::string &browse_name, const std::string &display_name) :
    node_id(node_id),
    browse_name(browse_name),
    display_name(display_name)
{
}

ua_variable_type::ua_variable_type(const ua_node_id& node_id, const std::string &browse_name, const std::string &display_name) :
    visitable_ua_node(node_id, browse_name, display_name)
{
}
