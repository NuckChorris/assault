#pragma once

// Time between updates (in seconds)
#define UPDATE_SECONDS 5

// Battery ID
#define BATTERY_ID "BAT0"

// Critical battery level
#define CRITICAL_LEVEL .15

// Battery size
#define BATTERY_HEIGHT 16
#define BATTERY_WIDTH 32

// Peg size
#define PEG_HEIGHT (BATTERY_HEIGHT / 3)
#define PEG_WIDTH 2

// Bolt size
#define BOLT_WIDTH 24
#define BOLT_HEIGHT 12

// Font settings
//#define FONT_FACE "M+ 1p Light"
#define FONT_FACE "Sans"
#define FONT_SIZE 12.0

// Margin
#define MARGIN 2 * 2

// Stroke Width
#define STROKE_WIDTH 2.0

// COLORS
#define COLOR_DEFAULT 65/255.0, 84/255.0, 127/255.0
#define COLOR_CRITICAL 172/255.0, 57/255.0, 57/255.0
#define COLOR_CHARGING COLOR_DEFAULT
//#define COLOR_CHARGING 65/255.0, 127/255.0, 84/255.0
#define COLOR_BACKGROUND 196/255.0, 202/255.0, 202/255.0
// Set this to 0.0 to have a transparent background
#define OPACITY_BACKGROUND 1.0
// Uncomment to make the background fill the entire widget.
// By default, the background will only fill the background
// of the battery bar itself.
#define BACKGROUND_FILL_ENTIRE_WIDGET
