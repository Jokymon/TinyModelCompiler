#include "xmlpp.h"
#include "modelgenerator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include <string>

void parse_story(xml_node node)
{
    for (auto cur : node)
    {
        if (cur.name() == "keyword")
        {
            std::cout << "keyword: " << cur.data() << std::endl;
        }
    }
}

void parse_doc(const std::string &docname)
{
    xml_document doc= xml_document::parse_file(docname);
    //if (!doc.parse_file(docname))
    //{
    //    std::cerr << "Document not parsed successfully.\n";
    //    return;
    //}

    auto root = doc.root();
    if (!root)
    {
        std::cerr << "Empty document\n";
        return;
    }

    if (root.name() != "story")
    {
        std::cerr << "document of the wrong type, root node != story\n";
        return;
    }

    for (auto cur : root)
    {
        if (cur.name() == "storyinfo")
            parse_story(cur);
    }
}

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
