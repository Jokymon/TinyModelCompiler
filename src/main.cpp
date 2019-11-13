#include "xmlpp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include <string>

void parse_story(xml_node node)
{
    auto cur = node.first_child();
    while (cur)
    {
        if (cur.name() == "keyword")
        {
            std::cout << "keyword: " << cur.data() << std::endl;
        }

        cur = cur.next_sibling();
    }
}

void parseStory (xmlDocPtr doc, xmlNodePtr cur)
{
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"keyword"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    printf("keyword: %s\n", key);
		    xmlFree(key);
 	    }
	    cur = cur->next;
	}
    return;
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

    auto cur = root.first_child();
    while (cur)
    {
        if (cur.name() == "storyinfo")
            parse_story(cur);

        cur = cur.next_sibling();
    }
}

void parseDoc(const char *docname)
{
	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);
	
	if (doc == NULL) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "story")) {
		fprintf(stderr,"document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return;
	}
	
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo"))){
			parseStory (doc, cur);
		}
		 
	    cur = cur->next;
	}
	
	xmlFreeDoc(doc);
}

void generate_nodeset2()
{
    xml_document doc = xml_document::create("http://opcfoundation.org/UA/2011/03/UANodeSet.xsd",
                                            "UaNodeSet");
    auto ns_uris = doc.root().create_child("NamespaceUris");

    doc.dump_file("MemoryBuffer.NodeSet2.xml");
}

int main(int argc, char **argv)
{
	if (argc <= 1) {
		printf("Usage: %s docname\n", argv[0]);
		return(0);
	}

    std::string docname = argv[1];
	parseDoc(docname.c_str());
    parse_doc(docname);

    generate_nodeset2();

	return 1;
}
