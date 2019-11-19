#include "modelgenerator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

    std::string docname = argv[1];
    //parseDoc(docname.c_str());
    //parse_doc(docname);

    model_generator mg;

    mg.load_model(docname);
    mg.write_nodeset2();

	return 0;
}
