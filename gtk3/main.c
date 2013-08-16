#include <string.h>
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
	battery_state *state;
	battery_options *opts;

	// Grab our widget size
	width = gtk_widget_get_allocated_width(widget);
	height = gtk_widget_get_allocated_height(widget);

	// Figure out options
	opts = &(battery_options){
		.stroke_width = STROKE_WIDTH,
		.battery_width = BATTERY_WIDTH,
		.battery_height = BATTERY_HEIGHT,
		.peg_width = PEG_WIDTH,
		.peg_height = PEG_HEIGHT,
		.bolt_width = BOLT_WIDTH,
		.bolt_height = BOLT_HEIGHT,
		.font_face = FONT_FACE,
		.font_size = FONT_SIZE,
		.font_slant = CAIRO_FONT_SLANT_NORMAL,
		.font_weight = CAIRO_FONT_WEIGHT_NORMAL,
		.pat_background = cairo_pattern_create_rgba(COLOR_BACKGROUND),
		.pat_default = cairo_pattern_create_rgba(COLOR_DEFAULT),
		.pat_charging = cairo_pattern_create_rgba(COLOR_CHARGING),
		.pat_critical = cairo_pattern_create_rgba(COLOR_CRITICAL),
		.x = (int)CENTERED(width, BATTERY_WIDTH + STROKE_WIDTH),
		.y = (int)CENTERED(height, BATTERY_HEIGHT + STROKE_WIDTH)
	};

	state = &(battery_state){
		.charging = battery_get_charging(),
		.critical = battery_get_critical(),
		.percent = battery_get_percent()
	};
	snprintf(state->text, 5, "%i%%", (int)(state->percent * 100));

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

	// Clear canvas
	#ifdef BACKGROUND_FILL_ENTIRE_WIDGET
	cairo_rectangle(cr, 0, 0, width, height);
	cairo_set_source_rgba(cr, COLOR_BACKGROUND);
	cairo_fill(cr);
	#endif

	battery_draw(cr, opts, state);

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
	gtk_widget_set_size_request(drawing_area, BATTERY_WIDTH + 8, BATTERY_HEIGHT + 8);
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
