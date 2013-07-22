#include "main.h"
#include "variables.h"
#include "draw_battery.h"
#include "utils.h"
#include "acpi.h"

GtkWidget *drawing_area;

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
(GtkWidget *widget, GdkEventExpose *event, UNUSED gpointer data)
{
	guint width, height;
	cairo_pattern_t *color;
	cairo_path_t *bolt, *fill, *text;

	// Grab our widget size
	width = widget->allocation.width;
	height = widget->allocation.height;

	// Set up Cairo
	cairo_t *cr = gdk_cairo_create(widget->window);

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

	// Clear canvas
	#ifdef BACKGROUND_FILL_ENTIRE_WIDGET
	cairo_rectangle(cr, 0, 0, width, height);
	#else
	cairo_rectangle(cr, (MARGIN / 2) + STROKE_WIDTH, (MARGIN / 2) + STROKE_WIDTH, BATTERY_WIDTH, BATTERY_HEIGHT);
	#endif
	cairo_set_source_rgba(cr, COLOR_BACKGROUND, OPACITY_BACKGROUND);
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

static int lxpanel_constructor
(Plugin *p, char **fp)
{
	(void)fp;
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, BATTERY_WIDTH + PEG_WIDTH + MARGIN, BATTERY_HEIGHT + MARGIN);
	g_signal_connect(G_OBJECT(drawing_area), "expose_event", G_CALLBACK(draw_callback), NULL);
	// Add a timer for redraw
	g_timeout_add_seconds(UPDATE_SECONDS, &do_redraw, NULL);

	p->pwid = gtk_event_box_new();
	gtk_container_add(GTK_CONTAINER(p->pwid), GTK_WIDGET(drawing_area));
	gtk_widget_set_size_request(p->pwid, BATTERY_WIDTH + PEG_WIDTH + MARGIN, BATTERY_HEIGHT + MARGIN);
	gtk_widget_set_has_window(p->pwid, FALSE);
	gtk_widget_show_all(p->pwid);

	return TRUE;
}

static void lxpanel_destructor (UNUSED Plugin *p) {}

PluginClass assault_plugin_class = {
	PLUGINCLASS_VERSIONING,
	type : "assault",
	name : "Assault",
	version: "1.0",
	description : "A battery indicator.",
	one_per_system : FALSE,
	expand_available : FALSE,
	constructor : lxpanel_constructor,
	destructor : lxpanel_destructor,
};
