#if !defined(GG_TILEMAP_H)
#define GG_TILEMAP_H

#include "tileset.h"

typedef struct gg_tilemap {
    uint32_t* data;
    uint32_t width;
    uint32_t height;
} gg_grid_t;

void Grid_Create(gg_grid_t* grid, uint32_t width, uint32_t height);
void Grid_Draw(gg_grid_t* grid, gg_window_t* window, gg_tileset_t* tileset, int32_t x, int32_t y);
void Grid_Destroy(gg_grid_t* grid);

#endif  // GG_TILEMAP_H
