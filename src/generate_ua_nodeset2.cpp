#include "ua_nodeset2_generator.h"
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    if (argc <= 1) {
        printf("Usage: %s <path to Opc.Ua.NodeSet2.xml>\n", argv[0]);
        return(0);
    }

    std::string docname = argv[1];

    ua_nodeset2_generator ns2gen;
    ns2gen.load_nodeset(docname);
    ns2gen.write_nodeset2_sources();

	return 0;
}
