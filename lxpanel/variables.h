#pragma once

// Time between updates (in seconds)
#define UPDATE_SECONDS 5

// Critical battery level
#define CRITICAL_LEVEL .15

// Battery size
#define BATTERY_HEIGHT 16
#define BATTERY_WIDTH 32

// Peg size
#define PEG_HEIGHT (BATTERY_HEIGHT / 3)
#define PEG_WIDTH 2

// Bolt size
#define BOLT_WIDTH 40
#define BOLT_HEIGHT 22

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
