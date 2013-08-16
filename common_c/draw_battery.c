#include <stdio.h>
#include <stdlib.h>
#include "draw_battery.h"
#include "utils.h"

static inline bool battery_draw_path
(cairo_t *cr, cairo_path_t *path, cairo_pattern_t *pattern, int x, int y)
{
	cairo_translate(cr, x, y);
	cairo_append_path(cr, path);
	cairo_set_source(cr, pattern);
	cairo_fill(cr);
	cairo_translate(cr, -x, -y);
	return true;
}

bool battery_draw
(cairo_t *cr, battery_options *opts, battery_state *state)
{
	cairo_path_t *bolt, *border, *fill, *text;
	cairo_pattern_t *pat;

	bolt = battery_bolt_generate(opts->bolt_width, opts->bolt_height);
	border = battery_border_generate(opts->battery_width, opts->battery_height, opts->stroke_width,
			opts->peg_width, opts->peg_height);
	fill = battery_fill_generate(opts->battery_width, opts->battery_height, state->percent);
	text = battery_text_generate(state->text, opts->font_face, opts->font_size, opts->font_slant,
			opts->font_weight);

	cairo_set_antialias(cr, CAIRO_ANTIALIAS_BEST);

	// Fill behind battery
	cairo_rectangle(cr, opts->x, opts->y, opts->battery_width, opts->battery_height);
	cairo_set_source(cr, opts->pat_background);
	cairo_fill(cr);

	// Decide on a pattern
	pat = opts->pat_default;
	if (state->charging)
		pat = opts->pat_charging;
	if (state->critical)
		pat = opts->pat_critical;

	/***  DRAW BORDERS ***/
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_EVEN_ODD);
	battery_draw_path(cr, border, pat, opts->x, opts->y);

	/*** APPEND FILL ***/
	cairo_translate(cr, opts->x + opts->stroke_width, opts->y + opts->stroke_width);
	cairo_append_path(cr, fill);

	/*** APPEND TEXT OR BOLT ***/
	if (state->charging && state->text == NULL) {
		int bolt_x = CENTERED(opts->bolt_width,  opts->battery_width);
		int bolt_y = CENTERED(opts->bolt_height, opts->battery_height);

		cairo_translate(cr, bolt_x, bolt_y);
		cairo_append_path(cr, bolt);
		cairo_translate(cr, -bolt_x, -bolt_y);
	} else {
		cairo_text_extents_t *extents;
		extents = battery_text_extents(state->text, opts->font_face, opts->font_size,
				opts->font_slant, opts->font_weight);

		int text_x = CENTERED(opts->battery_width, extents->width);
		int text_y = CENTERED(opts->battery_height, extents->height + extents->y_bearing*2);

		text = battery_text_generate(state->text, opts->font_face, opts->font_size, opts->font_slant,
				opts->font_weight);

		cairo_translate(cr, text_x, text_y);
		cairo_append_path(cr, text);
		cairo_translate(cr, -text_x, -text_y);
	}
	cairo_translate(cr, -(opts->x), -(opts->y));

	cairo_set_source(cr, pat);
	cairo_fill(cr);
	cairo_set_fill_rule(cr, CAIRO_FILL_RULE_WINDING);

	return true;
}

cairo_path_t *battery_bolt_generate
(int width, int height)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_path_t *path;

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
	path = cairo_copy_path(cr);
	cairo_destroy(cr);
	return path;
}

cairo_path_t *battery_border_generate
(int width, int height, int stroke_width, int peg_width, int peg_height)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	int outside_width, outside_height, peg_top;
	cairo_path_t *path;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, width + stroke_width * 2 + peg_width,
			height + stroke_width * 2);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	outside_width = width + stroke_width * 2;
	outside_height = height + stroke_width * 2;
	peg_top = CENTERED(height + stroke_width * 2, peg_height);

	// Outside of border
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
	
	cairo_close_path(cr);
	path = cairo_copy_path(cr);
	cairo_destroy(cr);
	return path;
}

cairo_path_t *battery_fill_generate
(int width, int height, double percent)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_path_t *path;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, width, height);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	cairo_rectangle(cr, 0, 0, (int)(width * percent), height);

	cairo_close_path(cr);
	path = cairo_copy_path(cr);
	cairo_destroy(cr);
	return path;
}

cairo_path_t *battery_text_generate
(char *text, char *font_face, int font_size, cairo_font_slant_t font_slant,
		cairo_font_weight_t font_weight)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_path_t *path;

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 100, 100);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);
	cairo_new_path(cr);

	cairo_select_font_face(cr, font_face, font_slant, font_weight);
	cairo_set_font_size(cr, font_size);

	cairo_move_to(cr, 0, 0);
	cairo_text_path(cr, text);

	cairo_close_path(cr);
	path = cairo_copy_path(cr);
	cairo_destroy(cr);
	return path;
}

cairo_text_extents_t *battery_text_extents
(char *text, char *font_face, int font_size, cairo_font_slant_t font_slant,
		cairo_font_weight_t font_weight)
{
	cairo_surface_t *surface;
	cairo_t *cr;
	cairo_text_extents_t *extents = malloc(sizeof *extents);

	surface = cairo_image_surface_create(CAIRO_FORMAT_A8, 0, 0);
	cr = cairo_create(surface);
	cairo_surface_destroy(surface);

	cairo_select_font_face(cr, font_face, font_slant, font_weight);
	cairo_set_font_size(cr, font_size);

	cairo_text_extents(cr, text, extents);
	cairo_destroy(cr);
	return extents;
}
