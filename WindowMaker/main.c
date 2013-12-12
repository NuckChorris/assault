#include "main.h"
#include "variables.h"
#include "draw_battery.h"
#include "utils.h"
#include "acpi.h"

GtkWidget *dockwin, *iconwin, *drawing_area;

gboolean do_redraw
(UNUSED gpointer user_data)
{
	gtk_widget_queue_draw(drawing_area);
	return TRUE;
}

gboolean battery_get_critical
()
{
	return (battery_get_percent() <= CRITICAL_LEVEL);
}

gboolean battery_get_charging
()
{
	return acpi_get_charging(BATTERY_ID);
}

gdouble battery_get_percent
()
{
	return acpi_get_percent(BATTERY_ID);
}

gboolean draw_callback
(GtkWidget *widget, cairo_t *cr, UNUSED gpointer data)
{
	guint width, height;
	cairo_pattern_t *color;
	cairo_path_t *bolt, *fill, *text;

	// Grab our widget size
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

	// Clear canvas
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_set_source_rgba(cr, COLOR_BACKGROUND, 1.0);
	cairo_fill(cr);

	// Decide on a color
	color = cairo_pattern_create_rgb(COLOR_DEFAULT);
	if (battery_get_charging())
		color = cairo_pattern_create_rgb(COLOR_CHARGING);
	if (battery_get_critical())
		color = cairo_pattern_create_rgb(COLOR_CRITICAL);

	battery_border_draw(cr, color, (int)CENTERED(width, BATTERY_WIDTH),
	                               (int)CENTERED(height, BATTERY_HEIGHT),
	                               BATTERY_WIDTH,
	                               BATTERY_HEIGHT,
	                               STROKE_WIDTH,
	                               PEG_WIDTH,
	                               PEG_HEIGHT);


	int fill_x = CENTERED(width, BATTERY_WIDTH);
	int fill_y = CENTERED(height, BATTERY_HEIGHT);

	// Set fill style
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);

	// Generate battery fill
	fill = battery_fill_generate(BATTERY_WIDTH, BATTERY_HEIGHT, battery_get_percent());
	cairo_translate(cr, fill_x, fill_y);
	cairo_append_path(cr, fill);

	if (battery_get_charging()) {
		int bolt_x = CENTERED(BATTERY_WIDTH, BOLT_WIDTH);
		int bolt_y = CENTERED(BATTERY_HEIGHT, BOLT_HEIGHT);

		cairo_translate(cr, bolt_x, bolt_y);
		bolt = battery_bolt_generate(BOLT_WIDTH, BOLT_HEIGHT);
		cairo_append_path(cr, bolt);
		cairo_translate(cr, bolt_x, bolt_y);
	} else {
		cairo_text_extents_t extents;
		char strpercent[5];
		snprintf(strpercent, 5, "%i%%", (int)(battery_get_percent() * 100));

		cairo_select_font_face(cr, FONT_FACE, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, FONT_SIZE);
		cairo_text_extents(cr, strpercent, &extents);

		int text_x = CENTERED(BATTERY_WIDTH, extents.width);
		int text_y = CENTERED(BATTERY_HEIGHT,  extents.height + extents.y_bearing*2);

		text = battery_text_generate(strpercent, FONT_FACE, FONT_SIZE);

		cairo_translate(cr, text_x, text_y);
		cairo_append_path(cr, text);
		cairo_translate(cr, -text_x, -text_y);
	}
	cairo_translate(cr, -fill_x, -fill_y);

	cairo_set_source(cr, color);
	cairo_fill(cr);

	return FALSE;
}

static void activate
(GtkApplication *app, UNUSED gpointer user_data)
{
	// Window
	dockwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	iconwin = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_wmclass(GTK_WINDOW(dockwin), "Assault", "DockApp");
	gtk_window_set_wmclass(GTK_WINDOW(iconwin), "Assault", "DockApp");
	gtk_widget_set_size_request(dockwin, 64, 64);
	gtk_widget_set_size_request(iconwin, 64, 64);

	gtk_widget_realize(dockwin);
	gtk_widget_realize(iconwin);

	GList *list = NULL;
	list = g_list_prepend(list, iconwin->window);
	gdk_window_set_icon_list(dockwin->window, list);

	// Set up Drawing Area
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, 64, 64);
	g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), NULL);
	// Add a timer for redraw
	g_timeout_add_seconds(UPDATE_SECONDS, &do_redraw, NULL);
	gtk_container_add(GTK_CONTAINER(iconwin), drawing_area);

	// And display it.
	gtk_widget_show(iconwin);
	gtk_widget_show(dockwin);
	gdk_window_withdraw(dockwin->window);
}

int main
(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("org.assault.DockApp", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref (app);
	return status;
}
