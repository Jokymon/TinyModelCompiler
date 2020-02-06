#include "modelgenerator.h"
#include "opc_ua_nodeset2.h"
#include <array>
#include <iostream>

struct alias {
    std::string alias;
    int id;
};

const std::array<alias, 35> Aliases = {{
    { "Boolean", 1 },
    { "SByte", 2 },
    { "Byte", 3 },
    { "Int16", 4 },
    { "UInt16", 5 },
    { "Int32", 6 },
    { "UInt32", 7 },
    { "Int64", 8 },
    { "UInt64", 9 },
    { "Float", 10 },
    { "Double", 11 },
    { "DateTime", 13 },
    { "String", 12 },
    { "ByteString", 15 },
    { "Guid", 14 },
    { "XmlElement", 16 },
    { "NodeId", 17 },
    { "ExpandedNodeId", 18 },
    { "QualifiedName", 20 },
    { "LocalizedText", 21 },
    { "StatusCode", 19 },
    { "Structure", 22 },
    { "Number", 26 },
    { "Integer", 27 },
    { "UInteger", 28 },
    { "HasComponent", 47 },
    { "HasProperty", 46 },
    { "Organizes", 35 },
    { "HasEventSource", 36 },
    { "HasNotifier", 48 },
    { "HasSubtype", 45 },
    { "HasTypeDefinition", 40 },
    { "HasModellingRule", 37 },
    { "HasEncoding", 38 },
    { "HasDescription", 39 }
}};

const int DefaultPlaceHolderId = 1000;

//static bool ends_with(const std::string& str, const std::string& suffix)
//{
//    return str.size() >= suffix.size() && 0 == str.compare(str.size()-suffix.size(), suffix.size(), suffix);
//}

static bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

model_generator::model_generator()
{
    populate_node_list(m_ua_nodeset2);

    for (const auto &alias : Aliases)
    {
        m_ua_nodes.add_alias(alias.alias, ua_node_id(0, alias.id));
    }
}

void model_generator::load_model(const std::string &model_file)
{
    xml_document doc= xml_document::parse_file(model_file);

    auto root = doc.root();
    if (!root)
    {
        std::cerr << "Empty document\n";
        return;
    }

    m_target_namespace = root.attribute("TargetNamespace");
    const auto &namespace_prefixes{doc.namespaces()};
    for (const auto &definition : namespace_prefixes)
    {
        m_prefix_for_ns[definition.prefix] = definition.href;
    }

    for (auto child : root)
    {
    // TODO: Missing elements: "ReferenceType" "DataType" "Method" "Variable" "Property" "Dictionary" "View"
        if (child.name() == "Namespaces")
            parse_namespaces(child);
        else if (child.name() == "VariableType")
            parse_variable_type(child);
        else if (child.name() == "ObjectType")
            parse_object_type(child);
        else if (child.name() == "Object")
            parse_object(child);
    }
}

void model_generator::write_nodeset2(const std::string &output_file)
{
    resolve_references();
    m_ua_nodes.dump_to_file(output_file);
}

void model_generator::parse_namespaces(xml_node &namespaces_node)
{
    for (auto namespace_node : namespaces_node)
    {
        if (namespace_node.name() != "Namespace")
            continue;

        std::string url = namespace_node.data();
        m_ua_nodes.append_namespace(url);
    }
}

void model_generator::parse_variable_type(xml_node &variable_type_node)
{
    std::string symbolic_name = variable_type_node.attribute("SymbolicName");
    std::string base_type = variable_type_node.attribute("BaseType");

    auto new_node = std::make_shared<ua_variable_type>();
    new_node->node_id = ua_node_id(NamespaceIndex, DefaultPlaceHolderId);
    new_node->browse_name = qualified_name(NamespaceIndex, symbolic_name);
    new_node->references.emplace_back("HasSubtype", false, base_type);

    m_ua_nodes.push_back(new_node);
}

void model_generator::parse_object_type(xml_node &object_type_node)
{
    std::string symbolic_name = object_type_node.attribute("SymbolicName");
    std::string base_type = object_type_node.attribute("BaseType");

    ua_node_ptr new_node = std::make_shared<ua_object_type>();
    new_node->node_id = ua_node_id(NamespaceIndex, DefaultPlaceHolderId);
    new_node->browse_name = qualified_name(NamespaceIndex, symbolic_name);
    new_node->references.emplace_back("HasSubtype", false, base_type);
    m_ua_nodes.push_back(new_node);

    for (auto child : object_type_node)
    {
        if (child.name() == "Children")
        {
            for (auto grand_child : child)
            {
                if (grand_child.name() != "Property")
                    continue;

                auto new_property = parse_property(grand_child, new_node);
                new_node->references.emplace_back("HasProperty", true, new_property->node_id.to_string());
            }
        }
    }
}

ua_node_ptr model_generator::parse_property(xml_node &property_node, ua_node_ptr &parent)
{
    std::string symbolic_name = property_node.attribute("SymbolicName");

    auto new_node = std::make_shared<ua_variable>();
    new_node->node_id = ua_node_id(NamespaceIndex, DefaultPlaceHolderId);
    new_node->browse_name = qualified_name(NamespaceIndex, symbolic_name);
    new_node->parent = parent;
    new_node->data_type = property_node.attribute("DataType");
    new_node->data_type = new_node->data_type.substr(3);

    new_node->references.emplace_back("HasTypeDefinition", true, "ua:PropertyType");
    new_node->references.emplace_back("HasModellingRule", true, "ua:ModellingRule_Mandatory");
    new_node->references.emplace_back("HasProperty", false, parent->node_id.to_string());

    m_ua_nodes.push_back(new_node);

    return new_node;
}

void model_generator::parse_object(xml_node &object_node)
{
    std::string symbolic_name = object_node.attribute("SymbolicName");
    std::string type_definition = object_node.attribute("TypeDefinition");

    auto new_node = std::make_shared<ua_object>();
    new_node->node_id = ua_node_id(NamespaceIndex, DefaultPlaceHolderId);
    new_node->browse_name = qualified_name(NamespaceIndex, symbolic_name);

    for (auto child : object_node)
    {
        if (child.name() == "References")
        {
            for (auto grand_child : child)
            {
                if (grand_child.name() != "Reference")
                    continue;

                bool is_inverse = false;
                if (grand_child.has_attribute("IsInverse"))
                    if (grand_child.attribute("IsInverse")=="true")
                        is_inverse = true;

                std::string reference_type;
                std::string target_id;

                for (auto reference_property : grand_child)
                {
                    if (reference_property.name() == "ReferenceType")
                    {
                        reference_type = reference_property.data();
                        if (starts_with(reference_type, "ua:"))
                            reference_type = reference_type.substr(3);
                    }
                    else if (reference_property.name() == "TargetId")
                    {
                        target_id = reference_property.data();
                    }
                }

                new_node->references.emplace_back(reference_type, !is_inverse, target_id);
            }
        }
    }

    new_node->references.emplace_back("HasTypeDefinition", true, type_definition);

    m_ua_nodes.push_back(new_node);
}

void model_generator::resolve_references()
{
    for (auto &node : m_ua_nodes)
    {
        for (auto &reference : node->references)
        {
            qname reference_qname{reference.value};
            std::string prefix = reference_qname.prefix;
            if (m_prefix_for_ns.find(prefix) == m_prefix_for_ns.end())
            {
                std::cerr << "Cannot find namespace for prefix '" << prefix << "'\n";
                continue;
            }

            if (m_prefix_for_ns[prefix] == "http://opcfoundation.org/UA/")
            {
                if (m_ua_nodeset2.find(reference_qname.local_part) != m_ua_nodeset2.end())
                    reference.value = m_ua_nodeset2[reference_qname.local_part]->node_id.to_string();
            }
            else if (m_prefix_for_ns[prefix] == m_target_namespace)
            {
                if (m_ua_nodes.contains(reference_qname.local_part))
                    reference.value = m_ua_nodes.get_node(reference_qname.local_part)->node_id.to_string();
            }
            else
            {
                std::cerr << "Unknown namespace URI: " << m_prefix_for_ns[prefix] << std::endl;
            }
        }
    }
}
