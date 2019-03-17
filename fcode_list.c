#include <stdio.h>
#include <stdlib.h>

#include "fcode_list.h"

node *list_add(node *list, void *value) {
    node *n;
    node *idx;

    n = (node *)malloc(sizeof(node));
    n->value = value;
    n->next = NULL;

    if (list == NULL) {
        return n; 
    }

    idx = list;
    while (idx->next != NULL) {
        idx = idx->next;
    }

    idx->next = n;

    return list;
}

node *list_search(node *list, void *value) {
    node *idx;

    idx = list;
    while (idx) {
        if (idx->value == value)
            return idx;

        idx = idx->next;
    }

    return NULL;
}

node *list_del(node *list, node *n) {
    node *idx;

    if (list == n) {
        node *ret = list->next;
        free(list);

        return ret;
    }

    idx = list;
    while (idx->next) {
        if (idx->next == n) {
            idx->next = idx->next->next;
            free(n);
            return list;
        }

        idx = idx->next;
    }

    return list;
}

void list_print(node *list, void (*print)(void *a)) {
    node *idx;

    idx = list;

    while(idx) {
	print(idx->value);
        idx = idx->next;
    }
}

