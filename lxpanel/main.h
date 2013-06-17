#include <gtk/gtk.h>
#include <lxpanel/plugin.h>
GtkWidget *window, *grid, *drawing_area;

gboolean do_redraw ();
gboolean battery_get_critical ();
gboolean battery_get_charging ();
gdouble battery_get_percent ();
gboolean draw_callback (GtkWidget *widget, GdkEventExpose *event, gpointer data);
static int lxpanel_constructor (Plugin *p, char **fp);
static void lxpanel_destructor (Plugin *p);

