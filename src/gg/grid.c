#include "grid.h"
#include "tileset.h"

#include <stdlib.h>
#include <string.h>

#define CUTE_TILED_IMPLEMENTATION
#include "external/cute_tiled.h"

void Grid_Create(gg_grid_t* grid, uint32_t width, uint32_t height) {
    grid->width = width;
    grid->height = height;
    grid->data = calloc(width * height, sizeof(uint32_t));
}

void Grid_Draw(gg_grid_t* grid, gg_window_t* window, gg_tileset_t* tileset, int32_t x, int32_t y) {
    uint32_t tile_size = tileset->tile_size;
    for (uint32_t xx = 0; xx < grid->width; xx++) {
        for (uint32_t yy = 0; yy < grid->height; yy++) {
            Tileset_DrawTile(tileset, window, x + xx * tile_size, y + yy * tile_size,
                             grid->data[xx + yy * grid->width]);
        }
    }
}

void Grid_Destroy(gg_grid_t* grid) { free(grid->data); }
