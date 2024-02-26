#if !defined(GG_TILED_H)
#define GG_TILED_H

#include "grid.h"
#include "texture.h"
#include "tiledobject.h"
#include "tileset.h"


typedef struct gg_tiled_map {
    gg_grid_t grid;
    gg_tileset_t set;
    gg_texture_t tex;
    gg_tiled_object_t* objs;
} gg_tiled_map_t;

void TiledMap_LoadFromTMJ(gg_tiled_map_t* tmap, const char* path);
int32_t TiledMap_GetWidth(gg_tiled_map_t* tmap);
int32_t TiledMap_GetHeight(gg_tiled_map_t* tmap);
gg_tiled_object_t* TiledMap_FindObject(gg_tiled_map_t* tmap, const char* name);
void TiledMap_Draw(gg_tiled_map_t* tmap, gg_window_t* window, int32_t x, int32_t y);
void TiledMap_DrawCentered(gg_tiled_map_t* tmap, gg_window_t* window, int32_t x, int32_t y);
void TiledMap_Destroy(gg_tiled_map_t* tmap);

#endif  // GG_TILED_H
