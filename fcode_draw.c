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

#define BORDER 20

#define BORDER_X 20
#define BORDER_Y 20

#define MIN_DX 200
#define MIN_DY 50

typedef struct fcode_objdraw {
    /* father tells children at what (sx, sy) they should start */
    int sx;
    int sy;

    /* child should tell his father his sizes */
    int dx;
    int dy;

    /* horizontal or vertical expansion told by father */
    int exp;
} fcode_objdraw;


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

fcode_objdraw *draw_object(fcode_object *obj, fcode_objdraw *down_obj) {
    fcode_objdraw *up_obj;
    int tmp_sx, tmp_sy;

    printf("%s down:%d %d %d %d\n", obj->name , down_obj->sx, down_obj->sy, down_obj->dx, down_obj->dy);

    /* use the pass down parameters */
    obj->sx = down_obj->sx;
    obj->sy = down_obj->sy; /* needs to be computed */
    obj->dy = BORDER;
    obj->dx = 0;
    obj->exp = down_obj->exp;

    free(down_obj);

    tmp_sx = obj->sx + BORDER;
    tmp_sy = obj->sy + BORDER;
    node *tmp = obj->children;
    if (tmp == NULL) {
	obj->dx = MIN_DX;
    }
    while(tmp) {
	down_obj = (fcode_objdraw *)malloc(sizeof(fcode_objdraw));

	if (obj->exp == 1) {
	    down_obj->sx = obj->sx + BORDER;
	    down_obj->sy = tmp_sy; /* automatically computed based on child size */
	} else {
	    down_obj->sx = tmp_sx;
	    down_obj->sy = obj->sy + BORDER;
	}
	down_obj->dx = 0;
	down_obj->dy = 0;

	if (obj->exp == 0)
	    down_obj->exp = 1;
	else
	    down_obj->exp = 0;

	up_obj = draw_object((fcode_object *)tmp->value, down_obj);

	tmp_sx += up_obj->dx + BORDER;
	tmp_sy += up_obj->dy + BORDER;

	if (obj->exp == 1) {
	    obj->dy += up_obj->dy + BORDER;
	    down_obj->sy += up_obj->sy + BORDER;
	    if (up_obj->dx + 2 * BORDER > obj->dx)
		obj->dx = up_obj->dx + 2 * BORDER;
	} else {
	    obj->dx += up_obj->dx + BORDER;
	    down_obj->sx += up_obj->sx + BORDER;
	    if (up_obj->dy + 2 * BORDER > obj->dy)
		obj->dy = up_obj->dy + 2 * BORDER;
	}

	free(up_obj);
	tmp = tmp->next;
    }

    up_obj = (fcode_objdraw *)malloc(sizeof(fcode_objdraw));

    up_obj->sx = 0;
    up_obj->sy = obj->sy;
    up_obj->dx = 0;
    up_obj->dy = 0;

    /* prepare to give the parameters up */
    up_obj->dy = obj->dy;
    up_obj->dx = obj->dx;
    //up_obj->sy = obj->sy;

    return up_obj;
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
    while(tmp) {
	set_sy((fcode_object *)tmp->value, child_sy);
	child_sy += get_dy((fcode_object *)tmp->value) + BORDER;

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
    fcode_objdraw *pass_down, *pass_up;

    pass_down = (fcode_objdraw *)malloc(sizeof(fcode_objdraw));
    pass_up = (fcode_objdraw *)malloc(sizeof(fcode_objdraw));

    memset(pass_down, 0, sizeof(fcode_objdraw));
    memset(pass_up, 0, sizeof(fcode_objdraw));

    //init_draw_coord(p->objects);
    draw_object(p->objects, pass_down);
}
