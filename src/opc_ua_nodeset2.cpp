#include "opc_ua_nodeset2.h"
#include "ua_model.h"

void populate_node_list(std::map<std::string, std::unique_ptr<ua_node>> &nodes)
{
    nodes["BaseVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 62), qualified_name("BaseVariableType"), "BaseVariableType");
    nodes["BaseDataVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 63), qualified_name("BaseDataVariableType"), "BaseDataVariableType");
    nodes["PropertyType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 68), qualified_name("PropertyType"), "PropertyType");
    nodes["DataTypeDescriptionType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 69), qualified_name("DataTypeDescriptionType"), "DataTypeDescriptionType");
    nodes["DataTypeDictionaryType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 72), qualified_name("DataTypeDictionaryType"), "DataTypeDictionaryType");
    nodes["ServerVendorCapabilityType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2137), qualified_name("ServerVendorCapabilityType"), "ServerVendorCapabilityType");
    nodes["ServerStatusType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2138), qualified_name("ServerStatusType"), "ServerStatusType");
    nodes["BuildInfoType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 3051), qualified_name("BuildInfoType"), "BuildInfoType");
    nodes["ServerDiagnosticsSummaryType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2150), qualified_name("ServerDiagnosticsSummaryType"), "ServerDiagnosticsSummaryType");
    nodes["SamplingIntervalDiagnosticsArrayType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2164), qualified_name("SamplingIntervalDiagnosticsArrayType"), "SamplingIntervalDiagnosticsArrayType");
    nodes["SamplingIntervalDiagnosticsType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2165), qualified_name("SamplingIntervalDiagnosticsType"), "SamplingIntervalDiagnosticsType");
    nodes["SubscriptionDiagnosticsArrayType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2171), qualified_name("SubscriptionDiagnosticsArrayType"), "SubscriptionDiagnosticsArrayType");
    nodes["SubscriptionDiagnosticsType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2172), qualified_name("SubscriptionDiagnosticsType"), "SubscriptionDiagnosticsType");
    nodes["SessionDiagnosticsArrayType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2196), qualified_name("SessionDiagnosticsArrayType"), "SessionDiagnosticsArrayType");
    nodes["SessionDiagnosticsVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2197), qualified_name("SessionDiagnosticsVariableType"), "SessionDiagnosticsVariableType");
    nodes["SessionSecurityDiagnosticsArrayType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2243), qualified_name("SessionSecurityDiagnosticsArrayType"), "SessionSecurityDiagnosticsArrayType");
    nodes["SessionSecurityDiagnosticsType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2244), qualified_name("SessionSecurityDiagnosticsType"), "SessionSecurityDiagnosticsType");
    nodes["OptionSetType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 11487), qualified_name("OptionSetType"), "OptionSetType");
    nodes["StateVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2755), qualified_name("StateVariableType"), "StateVariableType");
    nodes["TransitionVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2762), qualified_name("TransitionVariableType"), "TransitionVariableType");
    nodes["FiniteStateVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2760), qualified_name("FiniteStateVariableType"), "FiniteStateVariableType");
    nodes["FiniteTransitionVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2767), qualified_name("FiniteTransitionVariableType"), "FiniteTransitionVariableType");
    nodes["DataItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2365), qualified_name("DataItemType"), "DataItemType");
    nodes["AnalogItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2368), qualified_name("AnalogItemType"), "AnalogItemType");
    nodes["DiscreteItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2372), qualified_name("DiscreteItemType"), "DiscreteItemType");
    nodes["TwoStateDiscreteType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2373), qualified_name("TwoStateDiscreteType"), "TwoStateDiscreteType");
    nodes["MultiStateDiscreteType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2376), qualified_name("MultiStateDiscreteType"), "MultiStateDiscreteType");
    nodes["MultiStateValueDiscreteType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 11238), qualified_name("MultiStateValueDiscreteType"), "MultiStateValueDiscreteType");
    nodes["ArrayItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 12021), qualified_name("ArrayItemType"), "ArrayItemType");
    nodes["YArrayItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 12029), qualified_name("YArrayItemType"), "YArrayItemType");
    nodes["XYArrayItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 12038), qualified_name("XYArrayItemType"), "XYArrayItemType");
    nodes["ImageItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 12047), qualified_name("ImageItemType"), "ImageItemType");
    nodes["CubeItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 12057), qualified_name("CubeItemType"), "CubeItemType");
    nodes["NDimensionArrayItemType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 12068), qualified_name("NDimensionArrayItemType"), "NDimensionArrayItemType");
    nodes["TwoStateVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 8995), qualified_name("TwoStateVariableType"), "TwoStateVariableType");
    nodes["ConditionVariableType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 9002), qualified_name("ConditionVariableType"), "ConditionVariableType");
    nodes["ProgramDiagnosticType"] = std::make_unique<ua_variable_type>(ua_node_id(0, 2380), qualified_name("ProgramDiagnosticType"), "ProgramDiagnosticType");
}
