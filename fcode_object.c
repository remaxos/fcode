#include "fcode_list.h"
#include "fcode_parser.h"
#include "fcode_object.h"

#define MAXNAME 512

void fcode_objprint(void *elem) {
    print_object((fcode_object *)elem);
}

void print_object(fcode_object *obj) {
    printf("%d %s (%d, %d, %d, %d)\n", obj->type, obj->name,
	  obj->sx, obj->sy, obj->dx, obj->dy); 
    list_print(obj->children, fcode_objprint);
}

void print_project(fcode_project *proj) {
    printf("----project-----\n");
    print_object(proj->objects);
    printf("----------------\n");
}
