#include "modelgenerator.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
    if (argc <= 2) {
        printf("Usage: %s docname outputname\n", argv[0]);
        return(0);
    }

    std::string docname = argv[1];
    std::string output_name = argv[2];

    model_generator mg;

    mg.load_model(docname);
    mg.write_nodeset2(output_name);

    return 0;
}
