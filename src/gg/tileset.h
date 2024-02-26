#if !defined(GG_TILESET_H)
#define GG_TILESET_H

#include <stdint.h>

#include "external/cute_tiled.h"

#include "texture.h"
#include "window.h"

typedef struct gg_tileset {
    gg_texture_t* texture;
    uint32_t tile_size;
    uint32_t tiles_wide;
} gg_tileset_t;

void Tileset_CreateFromTexture(gg_tileset_t* tileset, gg_texture_t* texture, uint32_t tile_size, uint32_t tiles_wide);
void Tileset_CreateFromTMX(gg_tileset_t* tileset, cute_tiled_tileset_t* tiled_set);
void Tileset_DrawTile(gg_tileset_t* tileset, gg_window_t* window, int32_t x, int32_t y, uint32_t tile_id);
void Tileset_Destroy(gg_tileset_t* tileset);

#endif  // GG_TILESET_H
