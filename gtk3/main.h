#include <gtk/gtk.h>
GtkWidget *window, *grid, *drawing_area, *spn_percent, *chk_critical, *chk_charging;

gboolean toggle_callback (GtkWidget *widget, gpointer data);
gboolean update_spinner (GtkWidget *widget, gpointer data);
gboolean battery_get_critical ();
gboolean battery_get_charging ();
gdouble battery_get_percent ();
gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);
static void activate (GtkApplication *app, gpointer data);
int main (int argc, char **argv);
