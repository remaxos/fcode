#include <stdio.h>
#include <stdlib.h>

#ifndef FCODE_LIST_H
#define FCODE_LIST_H

/* just a basic int list (better than nothing) */
typedef struct node {
    void *value;
    struct node *next;
} node;

node *list_add(node *list, void *value);
node *list_search(node *list, void *value);
node *list_del(node *list, node *n);
void list_print(node *list, void (*print)(void *));

#endif /* FCODE_LIST_H */
