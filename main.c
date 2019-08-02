#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>

#include "fcode_parser.h"
#include "fcode_draw.h"
#include "fcode_xml.h"

/* Create a new hbox with an image and a label packed into it
 * and return the box. */

static GtkWidget *xpm_label_box( gchar     *xpm_filename,
        gchar     *label_text )
{
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *image;

    /* Create box for image and label */
    box = gtk_box_new(FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (box), 2);

    /* Now on to the image stuff */
    image = gtk_image_new_from_file (xpm_filename);

    /* Create a label for the button */
    label = gtk_label_new (label_text);

    /* Pack the image and label into the box */
    gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 3);
    gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 3);

    gtk_widget_show (image);
    gtk_widget_show (label);

    return box;
}


gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    guint width, height;
    GtkStyleContext *context;

    fcode_project *proj = (fcode_project *)data;

    context = gtk_widget_get_style_context(widget);

    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    //printf("%d %d\n", width, height);

    gtk_render_background(context, cr, 0, 0, width, height);
    cairo_scale(cr, proj->scale, proj->scale);
    draw_project(cr, proj);

    //cairo_rectangle(cr, 10, 10, 100, 100);

/* 
    gtk_style_context_get_color (context,
	    gtk_style_context_get_state (context),
	    &color);
*/
    //gdk_rgba_parse(&color, "white");
    //gdk_cairo_set_source_rgba (cr, &color);

    //cairo_fill(cr);

    return FALSE;
}


/* Our usual callback function */
static void callback(GtkWidget *widget, gpointer data)
{
    GtkWidget *main_window = (GtkWindow *)data;

    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
            GTK_WINDOW(main_window),
            action,
            "_Cancel",
            GTK_RESPONSE_CANCEL,
            "_Open",
            GTK_RESPONSE_ACCEPT,
            NULL);

    res = gtk_dialog_run (GTK_DIALOG (dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
        filename = gtk_file_chooser_get_filename (chooser);
        
        g_print("%s\n", filename);
        //open_file (filename);
        //g_free (filename);
    }

    gtk_widget_destroy (dialog);
}


gboolean button_press_event(GtkWidget *widget, GdkEventKey *event, gpointer data)
{
    fcode_project *proj = (fcode_project *)data;

    switch (event->keyval)
    {
        case GDK_KEY_minus:
	    printf("zoom-out\n");
	    fflush(stdout);

	    proj->scale -= 0.1;
	    prepare_drawing(proj);
	    gtk_widget_queue_draw(proj->drawing_area);

	    break;
	case GDK_KEY_plus:
	    printf("zoom-in\n");
	    fflush(stdout);

            proj->scale += 0.1;	
	    prepare_drawing(proj);
	    gtk_widget_queue_draw(proj->drawing_area);

	    break;
    };

    return FALSE;
} 

int main(int argc, char **argv)
{
    fcode_project proj;

    if (argc == 2) {
	printf("%s %s\n", argv[0], argv[1]);
    }

    /* parse the location and construct the structure */
    printf("parsing project...\n");
    proj.objects = malloc(sizeof(fcode_object));
    proj.objects->type = FCODE_ROOT;
    strcpy(proj.objects->name, "root");
    proj.objects->sx = proj.objects->sy = proj.objects->dx = proj.objects->dy = 0;
    proj.objects->children = NULL;
    proj.scale = 1;

    fcode_parse_dir(argv[1], proj.objects);

    printf("initialize drawings...\n");
    prepare_drawing(&proj);
    
    print_project(&proj);

    /* generate draw.io xml diagram */
    fcode_gen_xml(&proj);

    /* GtkWidget is the storage type for widgets */
    GtkWidget *main_window;
    GtkWidget *scroll_window;
    GtkWidget *button;
    GtkWidget *drawing_area;
    GtkWidget *box;

    gtk_init(&argc, &argv);

    /* Create a new window */
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "FCode");
    /* It's a good idea to do this for all windows. */
    g_signal_connect(main_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect(main_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    /* Sets the border width of the window. */
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 5);
    //gtk_window_fullscreen(GTK_WINDOW(main_window)); 

    /* Creating the 2D playing ground */
    drawing_area = gtk_drawing_area_new();
    proj.drawing_area = drawing_area;

    /* TODO: this looks shitty, maybe rewrite it */
    gtk_widget_set_size_request(drawing_area, proj.objects->dx, proj.objects->dy);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), (gpointer)&proj); 
    g_signal_connect(G_OBJECT(main_window), "key_press_event", G_CALLBACK(button_press_event),
		    (gpointer)&proj);

    /* scrolled window */
    scroll_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window), GTK_POLICY_AUTOMATIC,
		    GTK_POLICY_ALWAYS);

    /* Create a new button */
    button = gtk_button_new();

    /* Connect the "clicked" signal of the button to our callback */
    g_signal_connect(button, "clicked", G_CALLBACK(callback), (gpointer)main_window);

    /* This calls our box creating function */
    box = xpm_label_box("info.xpm", "Open directory");

    /* Pack and show all our widgets */
    gtk_widget_show(box);

    //gtk_container_add(GTK_CONTAINER(button), box);

    //gtk_widget_show(button);
    //gtk_container_add(GTK_CONTAINER(window), button);
    gtk_container_add(GTK_CONTAINER(main_window), scroll_window);
    gtk_container_add(GTK_CONTAINER(scroll_window), drawing_area);

    gtk_widget_show(drawing_area);
    gtk_widget_show(scroll_window);
    gtk_widget_show(main_window);

    /* Rest in gtk_main and wait for the fun to begin! */
    gtk_main();

    return 0;
}
