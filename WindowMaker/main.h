#pragma once

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <cairo/cairo.h>

GtkWidget *window, *grid, *drawing_area;

gboolean do_redraw ();
gboolean battery_get_critical ();
gboolean battery_get_charging ();
gdouble battery_get_percent ();
gboolean draw_callback (GtkWidget *widget, cairo_t *cr, gpointer data);
static void activate (GtkApplication *app, gpointer data);
int main (int argc, char **argv);
