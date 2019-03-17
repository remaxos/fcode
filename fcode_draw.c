#include <gtk/gtk.h>

#include "fcode_object.h"


void draw_project(cairo_t *cr, fcode_project *p)
{
    GdkRGBA color;
    cairo_set_source_rgb(cr, 10, 10, 10); 

    cairo_rectangle(cr, 0, 0, 70, 70);
    cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    cairo_set_font_size(cr, 100);

    gdk_rgba_parse(&color, "red");
    gdk_cairo_set_source_rgba (cr, &color);
    cairo_move_to(cr, 20, 60);
    cairo_show_text(cr, "test");

    gdk_rgba_parse(&color, "white");
    gdk_cairo_set_source_rgba (cr, &color);
    cairo_move_to(cr, 20, 100);
    cairo_show_text(cr, "test");
}

#define BORDER 10
#define MIN_DX 100
#define MIN_DY 20


void set_sx(fcode_object *obj) {
    if (obj->parent == NULL) {
	obj->sx = 0;
    } else {
	obj->sx = obj->parent->sx + BORDER;
    }

    node *tmp = obj->children;
    while(tmp) {
	set_sx((fcode_object *)tmp->value);
	tmp = tmp->next;
    }
}



int get_dy(fcode_object *obj) {
    if (obj->dy != 0)
	return obj->dy;

    node *tmp = obj->children;
    int child_dy = 0;
    int dy = BORDER;
    while(tmp) {
	child_dy = get_dy((fcode_object *)tmp->value);
	dy += child_dy + BORDER;	
	tmp = tmp->next;
    }

    /* no child */
    if (dy == BORDER) {
	obj->dy = MIN_DY;
    } else {
	obj->dy = dy;
    }

    return obj->dy;
}

int get_dx(fcode_object *obj) {
    if (obj->dx != 0)
	return obj->dx;

    node *tmp = obj->children;
    int child_dx = 0;
    int max_child_dx = 0;
    while(tmp) {
	child_dx = get_dx((fcode_object *)tmp->value);
	if (child_dx > max_child_dx) {
	    max_child_dx = child_dx;
	}
	tmp = tmp->next;
    }

    /* no child */
    if (max_child_dx == 0) {
	obj->dx = MIN_DX;
    } else {
	obj->dx = max_child_dx + 2 * BORDER;
    }

    return obj->dx;
}

void set_sy(fcode_object *obj, int sy) {
    obj->sy = sy;

    int child_sy = obj->sy + BORDER;
    node *tmp = obj->children;
    int cnt = 0;
    while(tmp) {
	set_sy((fcode_object *)tmp->value, child_sy);
	child_sy += get_dy((fcode_object *)tmp->value);

	tmp = tmp->next;
    }
}

void init_draw_coord(fcode_object *obj) {
    set_sx(obj);
    set_sy(obj, 0);
    obj->dx = get_dx(obj);
    obj->dy = get_dy(obj);
}


void prepare_drawing(fcode_project *p)
{
    init_draw_coord(p->objects);
}
