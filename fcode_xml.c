/**
 * section: InputOutput
 * synopsis: Output to char buffer
 * purpose: Demonstrate the use of xmlDocDumpMemory
 *          to output document to a character buffer
 * usage: io2
 * test: io2 > io2.tmp && diff io2.tmp $(srcdir)/io2.res
 * author: John Fleck
 * copy: see Copyright for the status of this software.
 */


#include "fcode_object.h"
//#include "fcode_list.h"

#include <libxml/parser.h>
#include <time.h>

int fcode_xml(void)
{
    xmlNodePtr mxGraphModel;
    xmlNodePtr root;
    xmlNodePtr mxCell0, mxCell1;
    xmlNodePtr mxCell, mxCellGeo;
    xmlDocPtr doc;
    xmlChar *xmlbuff;
    int buffersize;

    FILE *f;

    f = fopen("ouput.xml", "w");

    doc = xmlNewDoc(BAD_CAST "1.0");
    /* TODO: find another way to set the encodings */
    //xmlSetProp(doc, "encoding", "UTF-8");

    mxGraphModel = xmlNewNode(NULL, BAD_CAST "mxGraphModel");
    xmlSetProp(mxGraphModel, "dx", "1000");
    xmlSetProp(mxGraphModel, "dy", "1000");
    xmlSetProp(mxGraphModel, "grid", "0");
    xmlSetProp(mxGraphModel, "guides", "0");
    xmlSetProp(mxGraphModel, "tooltips", "0");
    xmlSetProp(mxGraphModel, "connect", "0");
    xmlSetProp(mxGraphModel, "arrows", "0");
    xmlSetProp(mxGraphModel, "fold", "0");
    xmlSetProp(mxGraphModel, "page", "0");
    xmlSetProp(mxGraphModel, "math", "0");
    xmlSetProp(mxGraphModel, "shadow", "0");

    xmlSetProp(mxGraphModel, "pageWidth", "500");
    xmlSetProp(mxGraphModel, "pageHeight", "500");

    root = xmlNewChild(mxGraphModel, NULL, "root", NULL);

    mxCell0 = xmlNewChild(root, NULL, "mxCell", NULL); 
    xmlSetProp(mxCell0, "id", "0");

    mxCell1 = xmlNewChild(root, NULL, "mxCell", NULL); 
    xmlSetProp(mxCell1, "id", "1");
    xmlSetProp(mxCell1, "parent", "0");

    mxCell = xmlNewChild(root, NULL, "mxCell", NULL); 
    xmlSetProp(mxCell, "id", "10");
    xmlSetProp(mxCell, "value", "TODO");
    xmlSetProp(mxCell, "style", "rounded=0;whiteSpace=wrap;html=1;align=left;verticalAlign=top;");
    xmlSetProp(mxCell, "vertex", "1");
    xmlSetProp(mxCell, "parent", "1");
    mxCellGeo = xmlNewChild(mxCell, NULL, "mxGeometry", NULL);
    xmlSetProp(mxCellGeo, "x", "10");
    xmlSetProp(mxCellGeo, "y", "10");
    xmlSetProp(mxCellGeo, "width", "10");
    xmlSetProp(mxCellGeo, "height", "10");
    xmlSetProp(mxCellGeo, "as", "geometry");

    //xmlNodeSetContent(mxGraphModel, BAD_CAST "content");
    xmlDocSetRootElement(doc, mxGraphModel);

    /*
     * Dump the document to a buffer and print it
     * for demonstration purposes.
     */
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    fprintf(f, "%s", (char *) xmlbuff);

    /*
     * Free associated memory.
     */
    xmlFree(xmlbuff);
    xmlFreeDoc(doc);

    fclose(f);

    return 0;

}

void fcode_xml_node(xmlNodePtr node, fcode_object *obj, int id)
{
    xmlNodePtr new_node = xmlNewChild(node, NULL, "mxCell", NULL);
    xmlNodePtr new_node_geo;
    char start_x[10] = {0};
    char start_y[10] = {0};
    char diff_x[10] = {0};
    char diff_y[10] = {0};
    char sid[10] = {0};
    struct node *tmp;

    sprintf(start_x, "%f", obj->sx);
    sprintf(start_y, "%f", obj->sy);
    sprintf(diff_x, "%f", obj->dx);
    sprintf(diff_y, "%f", obj->dy);

    time_t t;
    sprintf(sid, "%ld", random());

    xmlSetProp(new_node, "id", sid);
    xmlSetProp(new_node, "value", obj->name);
    xmlSetProp(new_node, "style", "rounded=0;whiteSpace=wrap;html=1;align=left;verticalAlign=top;");
    xmlSetProp(new_node, "vertex", "1");
    xmlSetProp(new_node, "parent", "1");
    new_node_geo = xmlNewChild(new_node, NULL, "mxGeometry", NULL);
    xmlSetProp(new_node_geo, "x", start_x);
    xmlSetProp(new_node_geo, "y", start_y);
    xmlSetProp(new_node_geo, "width", diff_x);
    xmlSetProp(new_node_geo, "height", diff_y);
    xmlSetProp(new_node_geo, "as", "geometry");

    tmp = obj->children;
    while(tmp) {
	id++;
	fcode_xml_node(node, (fcode_object *)tmp->value, id);
	tmp = tmp->next;
    }
}

int fcode_gen_xml(fcode_project *p)
{
    srandom(time(NULL));

    xmlNodePtr mxGraphModel;
    xmlNodePtr root;
    xmlNodePtr mxCell0, mxCell1;
    xmlNodePtr mxCell, mxCellGeo;
    xmlDocPtr doc;
    xmlChar *xmlbuff;
    int buffersize;

    FILE *f;

    f = fopen("ouput.xml", "w");

    doc = xmlNewDoc(BAD_CAST "1.0");
    /* TODO: find another way to set the encodings */
    //xmlSetProp(doc, "encoding", "UTF-8");

    mxGraphModel = xmlNewNode(NULL, BAD_CAST "mxGraphModel");
    xmlSetProp(mxGraphModel, "dx", "1000");
    xmlSetProp(mxGraphModel, "dy", "1000");
    xmlSetProp(mxGraphModel, "grid", "0");
    xmlSetProp(mxGraphModel, "guides", "0");
    xmlSetProp(mxGraphModel, "tooltips", "0");
    xmlSetProp(mxGraphModel, "connect", "0");
    xmlSetProp(mxGraphModel, "arrows", "0");
    xmlSetProp(mxGraphModel, "fold", "0");
    xmlSetProp(mxGraphModel, "page", "0");
    xmlSetProp(mxGraphModel, "math", "0");
    xmlSetProp(mxGraphModel, "shadow", "0");

    xmlSetProp(mxGraphModel, "pageWidth", "500");
    xmlSetProp(mxGraphModel, "pageHeight", "500");

    root = xmlNewChild(mxGraphModel, NULL, "root", NULL);

    mxCell0 = xmlNewChild(root, NULL, "mxCell", NULL); 
    xmlSetProp(mxCell0, "id", "0");

    mxCell1 = xmlNewChild(root, NULL, "mxCell", NULL); 
    xmlSetProp(mxCell1, "id", "1");
    xmlSetProp(mxCell1, "parent", "0");

    fcode_xml_node(root, p->objects, 2);

    //xmlNodeSetContent(mxGraphModel, BAD_CAST "content");
    xmlDocSetRootElement(doc, mxGraphModel);

    /*
     * Dump the document to a buffer and print it
     * for demonstration purposes.
     */
    xmlDocDumpFormatMemory(doc, &xmlbuff, &buffersize, 1);
    fprintf(f, "%s", (char *) xmlbuff);

    /*
     * Free associated memory.
     */
    xmlFree(xmlbuff);
    xmlFreeDoc(doc);

    fclose(f);

    return 0;
}


