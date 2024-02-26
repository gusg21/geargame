#if !defined(GG_VECTOR_H)
#define GG_VECTOR_H

#include <stdint.h>

typedef struct gg_v2 {
    float x, y;
} gg_v2_t;  // Top 10 most readable type names
#define V2(_x, _y) ((gg_v2_t){.x = (_x), .y = (_y)})
#define V2ZERO() ((gg_v2_t){.x = 0.f, .y = 0.f})
#define V2ONE() ((gg_v2_t){.x = 1.f, .y = 1.f})

typedef struct gg_v2i {
    int32_t x, y;
} gg_v2i_t;  // Top 10 most readable type names
#define V2I(_x, _y) ((gg_v2_t){.x = (_x), .y = (_y)})
#define V2IZERO() ((gg_v2_t){.x = (_x), .y = (_y)})

gg_v2_t V2_Add(gg_v2_t a, gg_v2_t b);
gg_v2_t V2_Subtract(gg_v2_t a, gg_v2_t b);
float V2_Dot(gg_v2_t a, gg_v2_t b);

// TODO: v2i functions as needed

#endif  // GG_VECTOR_H
