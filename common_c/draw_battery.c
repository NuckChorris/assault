#include "draw_battery.h"
#include "utils.h"

cairo_path_t *battery_bolt_generate
(int width, int height)
{
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, width, height);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	cairo_move_to(cr, width * ( 0.0/19), height * ( 3.0/11));
	cairo_line_to(cr, width * (11.0/19), height * (11.0/11));
	cairo_line_to(cr, width * (11.0/19), height * ( 5.5/11));
	cairo_line_to(cr, width * (19.0/19), height * ( 8.0/11));
	cairo_line_to(cr, width * ( 8.0/19), height * ( 0.0/11));
	cairo_line_to(cr, width * ( 8.0/19), height * ( 5.5/11));

	cairo_close_path(cr);
	return cairo_copy_path(cr);
}

gboolean battery_bolt_draw
(cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h)
{
	cairo_path_t *bolt = battery_bolt_generate(w, h);

	cairo_translate(cr, x, y);
	cairo_append_path(cr, bolt);
	cairo_set_source(cr, c);
	cairo_fill(cr);
	cairo_translate(cr, -x, -y);
	return TRUE;
}

cairo_path_t *battery_border_generate
(int width, int height, int stroke_width, int peg_width, int peg_height)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	int outside_width, outside_height, peg_top;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8,
	                                     width + stroke_width * 2 + peg_width,
	                                     height + stroke_width * 2);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	outside_width = width + stroke_width * 2;
	outside_height = height + stroke_width * 2;
	peg_top = CENTERED(height + stroke_width * 2, peg_height);

	// Outside of border
//	cairo_rectangle(cr, 0, 0, outside_width, outside_height);
	cairo_move_to(cr, 0                        , 0                   );
	cairo_line_to(cr, outside_width            , 0                   );
	cairo_line_to(cr, outside_width            , peg_top             );
	cairo_line_to(cr, outside_width + peg_width, peg_top             );
	cairo_line_to(cr, outside_width + peg_width, peg_top + peg_height);
	cairo_line_to(cr, outside_width            , peg_top + peg_height);
	cairo_line_to(cr, outside_width            , outside_height      );
	cairo_line_to(cr, 0                        , outside_height      );
	// Inside of border
	cairo_rectangle(cr, stroke_width, stroke_width, width, height);

	// Peg on positive end
//	cairo_rectangle(cr, width + stroke_width * 2,
//	                    (int)CENTERED(height + stroke_width * 2, peg_height),
//	                    peg_width,
//	                    peg_height);
	
	cairo_close_path(cr);
	return cairo_copy_path(cr);
}

gboolean battery_border_draw
(cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h, int sw, int pw, int ph)
{
	cairo_path_t *border = battery_border_generate(w, h, sw, pw, ph);

	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);

	cairo_translate(cr, x - sw, y -sw);
	cairo_append_path(cr, border);
	cairo_set_source(cr, c);
	cairo_fill(cr);
	cairo_translate(cr, -(x-sw), -(y-sw));

	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);

	return TRUE;
}

cairo_path_t *battery_fill_generate
(int width, int height, double percent)
{
	cairo_surface_t *surface;
	cairo_t *cr;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, width, height);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	cairo_rectangle(cr, 0, 0, (int)(width * percent), height);

	cairo_close_path(cr);
	return cairo_copy_path(cr);
}

gboolean battery_fill_draw
(cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h, double p)
{
	cairo_path_t *fill = battery_fill_generate(w, h, p);

	cairo_translate(cr, x, y);
	cairo_append_path(cr, fill);
	cairo_set_source(cr, c);
	cairo_fill(cr);
	cairo_translate(cr, -x, -y);
	return TRUE;
}

cairo_path_t *battery_text_generate
(char text[5], char font_face[], int font_size)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_text_extents_t extents;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 100, 100);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	cairo_select_font_face(cr, font_face, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, font_size);

	cairo_text_extents(cr, text, &extents);

	cairo_move_to(cr, 0, 0);
	cairo_text_path(cr, text);

	cairo_close_path(cr);
	return cairo_copy_path(cr);
}

gboolean battery_text_draw
(cairo_t *cr, cairo_pattern_t *c, int center_x, int center_y, char text[5], char font_face[], int font_size)
{
	cairo_path_t *text_path = battery_text_generate(text, font_face, font_size);

	cairo_text_extents_t extents;

	cairo_select_font_face(cr, font_face, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, font_size);

	cairo_text_extents(cr, text, &extents);

	int text_x = center_x - (extents.width / 2);
	int text_y = center_y - (extents.height / 2);

	cairo_translate(cr, text_x, text_y);
	cairo_append_path(cr, text_path);
	cairo_set_source(cr, c);
	cairo_fill(cr);
	cairo_translate(cr, -text_x, -text_y);
	return TRUE;
}
