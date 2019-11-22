#ifndef OPC_UA_NODESET2_H
#define OPC_UA_NODESET2_H

#include "ua_model.h"
#include <map>
#include <memory>
#include <string>
void populate_node_list(std::map<std::string, std::unique_ptr<ua_node>> &nodes);

#endif
