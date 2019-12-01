#include "ua_model.h"
#include "xmlpp.h"
#include <exception>
#include <iostream>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

ua_node_id::ua_node_id() :
    namespace_id(0),
    id()
{
}

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

qualified_name::qualified_name(const std::string &name) :
    namespace_index(0),
    name(name)
{
}

qualified_name::qualified_name(int namespace_index, const std::string &name) :
    namespace_index(namespace_index),
    name(name)
{
}

std::string qualified_name::to_string() const
{
    if (namespace_index==0)
    {
        return name;
    }
    return std::to_string(namespace_index) + std::string(":") + name;
}

reference::reference(const std::string &reference_type, bool is_forward, const std::string &value) :
    reference_type(reference_type),
    is_forward(is_forward),
    value(value)
{
}

ua_node::ua_node(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &symbolic_name) :
    node_id(node_id),
    browse_name(browse_name),
    symbolic_name(symbolic_name)
{
}

ua_variable_type::ua_variable_type(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &symbolic_name) :
    visitable_ua_node(node_id, browse_name, symbolic_name)
{
}

ua_object_type::ua_object_type(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &symbolic_name) :
    visitable_ua_node(node_id, browse_name, symbolic_name)
{
}

ua_variable::ua_variable(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &symbolic_name) :
    visitable_ua_node(node_id, browse_name, symbolic_name)
{
}

ua_object::ua_object(const ua_node_id& node_id, const qualified_name &browse_name, const std::string &symbolic_name) :
    visitable_ua_node(node_id, browse_name, symbolic_name)
{
}

// -------- UA node model -----------------------

class node_set_generator : public ua_node_visitor
{
public:
    explicit node_set_generator(xml_node &node_set) :
        node_set(node_set)
    {}

    void visit(ua_node &node) override
    {
        node.visit(*this);
    }

    void visit(ua_variable_type &node) override
    {
        auto variable_node = node_set.create_child("UAVariableType");
        generate_base_attributes(node, variable_node);

        variable_node.set_attribute("ValueRank", "-2");
    }

    void visit(ua_object_type &node) override
    {
        auto object_node = node_set.create_child("UAObjectType");
        generate_base_attributes(node, object_node);
    }

    void visit(ua_variable &node) override
    {
        auto variable_node = node_set.create_child("UAVariable");
        generate_base_attributes(node, variable_node);

        if (node.parent)
        {
            variable_node.set_attribute("ParentNodeId", node.parent->node_id.to_string());
            variable_node.set_attribute("DataType", node.data_type);
        }
    }

    void visit(ua_object &node) override
    {
        auto object_node = node_set.create_child("UAObject");
        generate_base_attributes(node, object_node);
    }

private:
    void generate_base_attributes(ua_node &u_node, xml_node &x_node)
    {
        x_node.set_attribute("NodeId", u_node.node_id.to_string());
        x_node.set_attribute("BrowseName", u_node.browse_name.to_string());
        if (!u_node.symbolic_name.empty())
            x_node.set_attribute("SymbolicName", u_node.symbolic_name);

        auto display_name = x_node.create_child("DisplayName");
        display_name.set_data(u_node.browse_name.name);

        if (u_node.references.size()>0)
        {
            auto references_node = x_node.create_child("References");
            for (auto &reference : u_node.references)
            {
                auto reference_node = references_node.create_child("Reference");
                reference_node.set_attribute("ReferenceType", reference.reference_type);
                if (!reference.is_forward)
                    reference_node.set_attribute("IsForward", "false");
                reference_node.set_data(reference.value);
            }
        }
    }

private:
    xml_node &node_set;
};

ua_model::ua_model()
{
}

void ua_model::populate_from_file(const std::string &filename)
{
    xml_document doc = xml_document::parse_file(filename);

    auto root = doc.root();
    if (!root)
    {
        std::cerr << "Empty document\n";
        return;
    }

    for (auto child : root)
    {
        if (child.name() == "Aliases")
        {
            for (auto grand_child : child)
            {
                if (grand_child.name() == "Alias")
                    parse_alias(grand_child);
            }
        }
        else if (child.name() == "UAVariableType")
            parse_variable_type(child);
        else if (child.name() == "UAObjectType")
            parse_object_type(child);
        else if (child.name() == "UAVariable")
            parse_variable(child);
        else if (child.name() == "UAObject")
            parse_object(child);
    }
}

void ua_model::dump_to_file(const std::string &filename)
{
   xml_document doc = xml_document::create("http://opcfoundation.org/UA/2011/03/UANodeSet.xsd",
                                            "UaNodeSet");
    auto root = doc.root();

    generate_namespaces(root);
    generate_aliases(root);
    generate_nodes(root);

    doc.dump_file(filename);
}

void ua_model::push_back(ua_node_ptr node)
{
    _nodeset.push_back(node);
}

void ua_model::append_namespace(const std::string &namespace_uri)
{
    _namespaces.push_back(namespace_uri);
}

void ua_model::add_alias(const std::string &alias, const ua_node_id &node_id)
{
    _aliases[alias] = node_id;
}

ua_model::iterator ua_model::begin()
{
    return _nodeset.begin();
}

ua_model::iterator ua_model::end()
{
    return _nodeset.end();
}

void ua_model::parse_alias(xml_node &alias_node)
{
    std::string alias = alias_node.attribute("Alias");
    ua_node_id node_id = ua_node_id::from_string(alias_node.data());
    add_alias(alias, node_id);
}

void ua_model::parse_variable_type(xml_node &variable_type_node)
{
    std::string node_id_str = variable_type_node.attribute("NodeId");
    auto node_id = ua_node_id::from_string(node_id_str);
    qualified_name browse_name{0, variable_type_node.attribute("BrowseName")};
    std::string symbolic_name = variable_type_node.attribute("SymbolicName");

    int value_rank = -1;
    if (variable_type_node.has_attribute("ValueRank"))
    {
        std::string rank = variable_type_node.attribute("ValueRank");
        // TODO: convert to int
    }
    bool is_abstract = false;
    if (variable_type_node.has_attribute("IsAbstract"))
    {
        std::string abstract = variable_type_node.attribute("IsAbstract");
        if (abstract=="true")
            is_abstract = true;
    }

    _nodeset.emplace_back(std::make_shared<ua_variable_type>(node_id, browse_name, symbolic_name));
}

void ua_model::parse_object_type(xml_node &object_type_node)
{
    std::string node_id_str = object_type_node.attribute("NodeId");
    auto node_id = ua_node_id::from_string(node_id_str);
    qualified_name browse_name{0, object_type_node.attribute("BrowseName")};
    std::string symbolic_name = object_type_node.attribute("SymbolicName");

    bool is_abstract = false;
    if (object_type_node.has_attribute("IsAbstract"))
    {
        std::string abstract = object_type_node.attribute("IsAbstract");
        if (abstract=="true")
            is_abstract = true;
    }

    _nodeset.emplace_back(std::make_shared<ua_object_type>(node_id, browse_name, symbolic_name));
}

void ua_model::parse_variable(xml_node &variable_node)
{
    std::string node_id_str = variable_node.attribute("NodeId");
    auto node_id = ua_node_id::from_string(node_id_str);
    qualified_name browse_name{0, variable_node.attribute("BrowseName")};
    std::string symbolic_name = variable_node.attribute("SymbolicName");

    bool is_abstract = false;
    if (variable_node.has_attribute("IsAbstract"))
    {
        std::string abstract = variable_node.attribute("IsAbstract");
        if (abstract=="true")
            is_abstract = true;
    }

    _nodeset.emplace_back(std::make_shared<ua_variable>(node_id, browse_name, symbolic_name));
}

void ua_model::parse_object(xml_node &object_node)
{
    std::string node_id_str = object_node.attribute("NodeId");
    auto node_id = ua_node_id::from_string(node_id_str);
    qualified_name browse_name{0, object_node.attribute("BrowseName")};
    std::string symbolic_name = object_node.attribute("SymbolicName");

    bool is_abstract = false;
    if (object_node.has_attribute("IsAbstract"))
    {
        std::string abstract = object_node.attribute("IsAbstract");
        if (abstract=="true")
            is_abstract = true;
    }

    _nodeset.emplace_back(std::make_shared<ua_object>(node_id, browse_name, symbolic_name));
}

void ua_model::generate_namespaces(xml_node &root)
{
    auto ns_uris = root.create_child("NamespaceUris");
    for (const auto &url : _namespaces)
    {
        auto ns_uri = ns_uris.create_child("Uri");
        ns_uri.set_data(url);
    }
}

void ua_model::generate_aliases(xml_node &root)
{
    if (!_aliases.empty())
    {
        auto aliases_node = root.create_child("Aliases");
        for (auto alias : _aliases)
        {
            auto alias_node = aliases_node.create_child("Alias");
            alias_node.set_data(alias.second.to_string());
            alias_node.set_attribute("Alias", alias.first);
        }
    }
}

void ua_model::generate_nodes(xml_node &root)
{
    node_set_generator generator(root);
    for (const auto &ua_node : _nodeset)
    {
        generator.visit(*ua_node.get());
    }
}
