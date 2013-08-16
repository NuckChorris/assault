#pragma once

#include <stdbool.h>
#include <cairo/cairo.h>

typedef struct {
	double percent;
	bool charging;
	bool critical;
	char text[8];
} battery_state;

typedef struct {
	int stroke_width;
	int x;              int y;
	int battery_width;  int battery_height;
	int peg_width;      int peg_height;
	int bolt_width;     int bolt_height;
	char font_face[64];
	int font_size;
	cairo_font_slant_t font_slant;
	cairo_font_weight_t font_weight;
	cairo_pattern_t *pat_background;
	cairo_pattern_t *pat_default;
	cairo_pattern_t *pat_charging;
	cairo_pattern_t *pat_critical;
} battery_options;

static inline bool battery_draw_path (cairo_t *cr, cairo_path_t *path, cairo_pattern_t *pattern, int x, int y);
bool battery_draw (cairo_t *cr, battery_options *opts, battery_state *state);

cairo_path_t *battery_bolt_generate (int width, int height);
cairo_path_t *battery_border_generate (int width, int height, int stroke_width, int peg_width, int peg_height);
cairo_path_t *battery_fill_generate (int width, int height, double percent);
cairo_path_t *battery_text_generate (char *text, char *font_face, int font_size, cairo_font_slant_t font_slant, cairo_font_weight_t font_weight);
cairo_text_extents_t *battery_text_extents (char *text, char *font_face, int font_size, cairo_font_slant_t font_slant, cairo_font_weight_t font_weight);
