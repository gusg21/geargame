#if !defined(GG_TILED_OBJECT_H)
#define GG_TILED_OBJECT_H

#include <stdint.h>

typedef struct gg_tiled_object gg_tiled_object_t;

struct gg_tiled_object {
    char* name;
    int32_t x, y;
    char* script_name;

    gg_tiled_object_t* next;
};

#endif  // GG_TILED_OBJECT_H
