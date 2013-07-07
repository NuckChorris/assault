#pragma once

#include <stdbool.h>
#include <cairo/cairo.h>

cairo_path_t *battery_bolt_generate (int width, int height);
bool battery_bolt_draw (cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h);

cairo_path_t *battery_border_generate (int width, int height, int stroke_width, int peg_width, int peg_height);
bool battery_border_draw (cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h, int sw, int pw, int ph);

cairo_path_t *battery_fill_generate (int width, int height, double percent);
bool battery_fill_draw (cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h, double p);

cairo_path_t *battery_text_generate (char *text, char *font_face, int font_size);
bool battery_text_draw (cairo_t *cr, cairo_pattern_t *c, int center_x, int center_y, char *text, char *font_face, int font_size);
