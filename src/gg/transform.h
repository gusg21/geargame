#if !defined(GG_TRANSFORM_H)
#define GG_TRANSFORM_H

#include "vector.h"

typedef struct gg_transform {
    gg_v2_t pos;
    float rot;
    gg_v2_t scale;
} gg_transform_t;

#endif  // GG_TRANSFORM_H
