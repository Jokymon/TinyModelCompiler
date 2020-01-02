#include "ua_nodeset2_generator.h"
#include <CLI/CLI.hpp>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    CLI::App app;

    std::string docname;

    app.add_option("docname", docname, "Path to the Opc.Ua.NodeSet2.xml input file")
        ->required();

    CLI11_PARSE(app, argc, argv);

    ua_nodeset2_generator ns2gen;
    ns2gen.load_nodeset(docname);
    ns2gen.write_nodeset2_sources();

	return 0;
}
