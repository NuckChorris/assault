#pragma once
#include <gtk/gtk.h>

cairo_path_t *battery_bolt_generate (int width, int height);
gboolean battery_bolt_draw (cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h);

cairo_path_t *battery_border_generate (int width, int height, int stroke_width, int peg_width, int peg_height);
gboolean battery_border_draw (cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h, int sw, int pw, int ph);

cairo_path_t *battery_fill_generate (int width, int height, double percent);
gboolean battery_fill_draw (cairo_t *cr, cairo_pattern_t *c, int x, int y, int w, int h, double p);

cairo_path_t *battery_text_generate (char text[5], char font_face[], int font_size);
gboolean battery_text_draw (cairo_t *cr, cairo_pattern_t *c, int center_x, int center_y, char text[5], char font_face[], int font_size);
