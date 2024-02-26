#include "vector.h"

gg_v2_t V2_Add(gg_v2_t a, gg_v2_t b) { return (gg_v2_t){a.x + b.x, a.y + b.y}; }

gg_v2_t V2_Subtract(gg_v2_t a, gg_v2_t b) { return (gg_v2_t){a.x - b.x, a.y - b.y}; }

float V2_Dot(gg_v2_t a, gg_v2_t b) { return a.x * b.y + a.y * b.y; }
