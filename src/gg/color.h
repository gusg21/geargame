#if !defined(GG_COLOR_H)
#define GG_COLOR_H

#include <stdint.h>

typedef struct gg_color {
    uint8_t r, g, b, a;
} gg_color_t;

// Helper macros
#define COL(_r, _g, _b) \
    (gg_color_t) { .r = (_r), .g = (_g), .b = (_b), .a = 255 }
#define COLA(_r, _g, _b, _a) \
    (gg_color_t) { .r = (_r), .g = (_g), .b = (_b), .a = (_a) }

#endif  // GG_COLOR_H