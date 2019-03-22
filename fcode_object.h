#include "fcode_list.h"

#ifndef FCODE_OBJECT_H
#define FCODE_OBJECT_H

#define MAXNAME 512

typedef enum obj_type {
    FCODE_ROOT = 0,
    FCODE_DIR,
    FCODE_FILE
} fcode_objtype;

typedef struct object {
    fcode_objtype type;       
    char name[MAXNAME];

    struct object *parent;
    node *children; 

    /* drawing */
    int sx;
    int sy;
    int dx;
    int dy;
    int exp; /* horizontal/vertical expansion */

} fcode_object;



typedef struct project {
    fcode_object *objects; 
} fcode_project;

void print_object(fcode_object *obj);
void print_project(fcode_project *proj);

#endif /* FCODE_OBJECT_H */
