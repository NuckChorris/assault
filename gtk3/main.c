#include "main.h"
#include "variables.h"
#include "draw_battery.h"
#include "utils.h"
#include "acpi.h"

GtkWidget *window, *grid, *drawing_area, *spn_percent, *chk_charging, *chk_real_battery;

gboolean do_redraw
(UNUSED gpointer user_data)
{
	gtk_widget_queue_draw(drawing_area);
	return TRUE;
}

gboolean toggle_callback
(UNUSED GtkWidget *widget, UNUSED gpointer data)
{
	gtk_widget_queue_draw(drawing_area);
	return FALSE;
}

gboolean toggle_real_callback
(UNUSED GtkWidget *widget, UNUSED gpointer data)
{
	gtk_widget_set_sensitive(GTK_WIDGET(spn_percent), !battery_real());
	gtk_widget_set_sensitive(GTK_WIDGET(chk_charging), !battery_real());
	gtk_widget_queue_draw(drawing_area);
	return FALSE;
}

gboolean update_spinner
(UNUSED GtkWidget *widget, UNUSED gpointer data)
{
	gtk_widget_queue_draw(drawing_area);
	return FALSE;
}

gboolean battery_real
()
{
	return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_real_battery));
}

gboolean battery_get_critical
()
{
	return (battery_get_percent() <= CRITICAL_LEVEL);
}

gboolean battery_get_charging
()
{
	if (battery_real()) {
		return acpi_get_charging(BATTERY_ID);
	} else {
		return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chk_charging));
	}
}

gdouble battery_get_percent
()
{
	if (battery_real()) {
		return acpi_get_percent(BATTERY_ID);
    } else {
		return gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spn_percent)) / 100.0;
	}
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

static void activate
(GtkApplication *app, UNUSED gpointer user_data)
{
	GtkAdjustment *stepper_adjustment;

	// Window
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Battery Indicator");
	gtk_window_set_modal(GTK_WINDOW(window), TRUE);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	// Grid Container
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	// Battery Indicator
	// Set up Drawing Area
	drawing_area = gtk_drawing_area_new();
	gtk_grid_attach(GTK_GRID(grid), drawing_area, 0, 0, 1, 1);
	gtk_widget_set_size_request(drawing_area, BATTERY_WIDTH + MARGIN, BATTERY_HEIGHT + MARGIN);
	g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), NULL);
	// Add a timer for redraw
	g_timeout_add_seconds(UPDATE_SECONDS, (GSourceFunc)do_redraw, NULL);
	
	// Percentage spinner
	stepper_adjustment = gtk_adjustment_new(50, 0, 100, 1, 0, 0);
	spn_percent = gtk_spin_button_new(stepper_adjustment, 1.0, 0);
	gtk_grid_attach(GTK_GRID(grid), spn_percent, 0, 1, 1, 1);
	g_signal_connect(G_OBJECT(spn_percent), "value-changed", G_CALLBACK(update_spinner), NULL);

	// Real battery level checkbox
	chk_real_battery = gtk_check_button_new_with_label("Real level");
	gtk_grid_attach(GTK_GRID(grid), chk_real_battery, 0, 2, 1, 1);
	g_signal_connect(G_OBJECT(chk_real_battery), "toggled", G_CALLBACK(toggle_real_callback), NULL);

	// Charging checkbox
	chk_charging = gtk_check_button_new_with_label("Charging");
	gtk_grid_attach(GTK_GRID(grid), chk_charging, 0, 3, 1, 1);
	g_signal_connect(G_OBJECT(chk_charging), "toggled", G_CALLBACK(toggle_callback), NULL);
	
	// Display it
	gtk_widget_show_all(window);
}

int main
(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new("org.assault.gtk3", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref (app);
	return status;
}

// vim: set noexpandtab
