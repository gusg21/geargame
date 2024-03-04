#include "tiled.h"

#include <stdlib.h>
#include <string.h>

#include "external/cute_tiled.h"

#include "log.h"
#include "memory.h"

static void TiledMap_S_LoadLayer(gg_tiled_map_t* tmap, cute_tiled_map_t* map, cute_tiled_tileset_t* tileset,
                                 cute_tiled_layer_t* layer, const char* path) {
    if (TextIsEqual(layer->type.ptr, "tilelayer")) {
        Log_Info("TILED: Loading tile layer");

        // Load texture
        const char* full_path = TextFormat("%s/%s", GetDirectoryPath(path), tileset->image.ptr);
        Texture_LoadFromFile(&tmap->tex, full_path);

        // Load tileset
        Tileset_CreateFromTexture(&tmap->set, &tmap->tex, tileset->tilewidth, tileset->columns);

        // Load tilemap
        Grid_Create(&tmap->grid, map->width, map->height);

        // Copy in tile data
        memcpy(tmap->grid.data, layer->data, layer->width * layer->height * sizeof(uint32_t));
    } else if (TextIsEqual(layer->type.ptr, "objectgroup")) {
        Log_Info("TILED: Loading object layer");

        cute_tiled_object_t* tobject = layer->objects;
        gg_tiled_object_t* gobject = tmap->objs;
        while (tobject != NULL) {
            gobject->name = (char*)GG_CALLOC(256, sizeof(char));
            TextCopy(gobject->name, tobject->name.ptr);

            gobject->x = (int32_t)tobject->x;
            gobject->y = (int32_t)tobject->y;

            gobject->script_name = NULL;
            for (uint32_t prop_idx = 0; prop_idx < (uint32_t)tobject->property_count; prop_idx++) {
                cute_tiled_property_t* prop = &tobject->properties[prop_idx];   
                if (TextIsEqual(TextToLower(prop->name.ptr), "script")) {
                    gobject->script_name = (char*)GG_CALLOC(128, sizeof(char));
                    TextCopy(gobject->script_name, prop->data.string.ptr);
                }
            }

            if (tobject->next != NULL) {
                gobject->next = (gg_tiled_object_t*)GG_MALLOC(sizeof(gg_tiled_object_t));
            } else {
                gobject->next = NULL;
            }

            gobject = gobject->next;
            tobject = tobject->next;
        }
    }
}

void TiledMap_LoadFromTMJ(gg_tiled_map_t* tmap, const char* path) {
    tmap->objs = GG_MALLOC(sizeof(gg_tiled_object_t));

    cute_tiled_map_t* map = cute_tiled_load_map_from_file(path, NULL);
    cute_tiled_tileset_t* tileset = &map->tilesets[0];

    cute_tiled_layer_t* layer = map->layers;
    while (layer != NULL) {
        TiledMap_S_LoadLayer(tmap, map, tileset, layer, path);

        layer = layer->next;
    }

    cute_tiled_free_map(map);
}

int32_t TiledMap_GetWidth(gg_tiled_map_t* tmap) { return tmap->grid.width * tmap->set.tile_size; }

int32_t TiledMap_GetHeight(gg_tiled_map_t* tmap) { return tmap->grid.height * tmap->set.tile_size; }

gg_tiled_object_t* TiledMap_FindObject(gg_tiled_map_t* tmap, const char* name) {
    gg_tiled_object_t* obj = tmap->objs;
    while (obj != NULL) {
        if (TextIsEqual(obj->name, name)) {
            return obj;
        }
        obj = obj->next;
    }
    return NULL;
}

void TiledMap_Draw(gg_tiled_map_t* tmap, gg_window_t* window, int32_t x, int32_t y) {
    Grid_Draw(&tmap->grid, window, &tmap->set, x, y);
}

void TiledMap_DrawCentered(gg_tiled_map_t* tmap, gg_window_t* window, int32_t x, int32_t y) {
    uint32_t grid_w = TiledMap_GetWidth(tmap);
    uint32_t grid_h = TiledMap_GetHeight(tmap);
    TiledMap_Draw(tmap, window, x - grid_w / 2, y - grid_h / 2);
}

void TiledMap_Destroy(gg_tiled_map_t* tmap) {
    Grid_Destroy(&tmap->grid);
    Tileset_Destroy(&tmap->set);
    Texture_Destroy(&tmap->tex);
    
    gg_tiled_object_t* obj = tmap->objs;
    while (obj != NULL) {
        gg_tiled_object_t* next = obj->next;
        GG_FREE(obj->name);
        GG_FREE(obj->script_name);
        GG_FREE(obj);
        obj = next;
    }
    tmap->objs = NULL;
}
