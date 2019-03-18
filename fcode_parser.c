#include "fcode_parser.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME 512

void fcode_parse_dir(char *location, fcode_object *parent)
{
    DIR *d;
    struct dirent *dir;

    d = opendir(location);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s %d -> %d\n", dir->d_name, dir->d_type, strcmp(dir->d_name, ".."));

	    /* TODO: a sort of a dirignore (similar to gitignore) should be created */
	    if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".git") != 0) {
		fcode_object *fobj = (fcode_object *)malloc(sizeof(fcode_object));
		/* TODO: fobj == NULL */
		
		if (dir->d_type == 8) {
		    fobj->type = FCODE_FILE;
		} else {
		    fobj->type = FCODE_DIR;
		}
		strcpy(fobj->name, dir->d_name);
		fobj->children = NULL;
		fobj->parent = parent;
		fobj->sx = fobj->sy = fobj->dx = fobj->dy = 0;

		parent->children = list_add(parent->children, fobj);

		/* the recursive step */
		if (dir->d_type == 4) {
		    char *next_dir = malloc(sizeof(char) * MAX_NAME); 
		    memset(next_dir, 0, sizeof(char) * MAX_NAME);
		    sprintf(next_dir, "%s/%s", location, dir->d_name);

		    //printf("[%s]\n", next_dir);

		    fcode_parse_dir(next_dir, fobj);
		    free(next_dir);
		}
	    }
        }

        closedir(d);
    }

    return /* TODO */;
}
