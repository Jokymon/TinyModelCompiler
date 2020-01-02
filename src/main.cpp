#include "modelgenerator.h"
#include <CLI/CLI.hpp>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    CLI::App app;

    std::string docname;
    std::string output_name;

    app.add_option("docname", docname, "ModelDesign input file")
        ->required();
    app.add_option("-o", output_name, "NodeSet2 output file")
        ->required();

    CLI11_PARSE(app, argc, argv);

    model_generator mg;

    mg.load_model(docname);
    mg.write_nodeset2(output_name);

    return 0;
}
