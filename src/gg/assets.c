#include "assets.h"
#include "log.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void Assets_Create(gg_assets_t* assets) { assets->asset_list = NULL; }

void Assets_LoadInternals(gg_assets_t* assets, gg_window_t* window, gg_state_t* state) {
    Assets_Load(assets, window, state, ASSET_SCRIPT, "gg/map");
}

void Assets_Load(gg_assets_t* assets, gg_window_t* window, gg_state_t* state, gg_asset_type_e type, const char* name) {
    gg_asset_pair_t* pair = (gg_asset_pair_t*)malloc(sizeof(gg_asset_pair_t));

    pair->name = (char*)calloc(256, sizeof(char));
    TextCopy(pair->name, name);
    pair->asset.type = type;
    pair->next = NULL;

    // Load actual data
    char formatted_path[ASSETS_MAX_PATH_LENGTH] = {0};
    switch (type) {
        case ASSET_TEXTURE:
            sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, "assets/%s.png", name);
            Texture_LoadFromFile(&pair->asset.data.as_tex, formatted_path);
            break;
        case ASSET_TILED_MAP:
            sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, "assets/%s.tmj", name);
            TiledMap_LoadFromTMJ(&pair->asset.data.as_tiled_map, formatted_path);
            break;
        case ASSET_SCRIPT:
            sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, "assets/%s.lua", name);
            Script_LoadFromLua(&pair->asset.data.as_script, formatted_path);
            break;
        case ASSET_SCENE:
            // This asset is FANCY (read: painful) and needs the asset manager itself!!!
            // Yippee!!!
            sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, "assets/%s.json", name);
            Scene_LoadFromJson(&pair->asset.data.as_scene, assets, window, state, formatted_path);
            break;
        case ASSET_ACTOR_SPEC:
            sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, "assets/%s.json", name);
            ActorSpec_LoadFromJSON(&pair->asset.data.as_actor_spec, formatted_path);
            break;
        default:
            printf("ASSET LOAD: Unsupported asset type number %d...\n", type);
            break;
    }

    // Easy out if first pair
    if (assets->asset_list == NULL) {
        assets->asset_list = pair;
        return;
    }

    // Append to linked list
    // TODO: Might be broken???? I was working on it when it broke (2/16/24) and
    // it started working suddenly. not sure what's up, good luck soldier
    gg_asset_pair_t* last_pair = assets->asset_list;
    while (last_pair->next != NULL) {
        last_pair = last_pair->next;
    }
    last_pair->next = pair;
}

// gg_asset_t* Assets_Get(gg_assets_t* assets, const char* name) {
//     gg_assets_t* asset = NULL;
//     Assets_SafeGet(assets, &asset, name);
//     return asset;
// }

bool Assets_Has(gg_assets_t* assets, const char* name) {
    gg_asset_pair_t* pair = assets->asset_list;

    if (pair == NULL) return false;

    while (pair != NULL) {
        if (TextIsEqual(pair->name, name)) {
            return true;
        }

        pair = pair->next;
    }

    Log_Warn(Log_TextFormat("ASSETS: Unable to HAS() asset name %s!", name));
    return false;
}

bool Assets_Get(gg_assets_t* assets, gg_asset_t** asset_ptr, const char* name) {
    gg_asset_pair_t* pair = assets->asset_list;

    if (pair == NULL) return false;

    while (pair != NULL) {
        if (TextIsEqual(pair->name, name)) {
            (*asset_ptr) = &pair->asset;
            return true;
        }

        pair = pair->next;
    }

    Log_Warn(Log_TextFormat("ASSETS: Unable to GET() asset name %s!", name));
    return false;
}

const char* Assets_GetTypeName(gg_asset_type_e type) {
    switch (type) {
        case ASSET_NONE:
            return "None";

        case ASSET_SCENE:
            return "Scene";

        case ASSET_SCRIPT:
            return "Script";

        case ASSET_TEXTURE:
            return "Texture";

        case ASSET_TILED_MAP:
            return "Tiled Map";

        case ASSET_ACTOR_SPEC:
            return "Actor Spec";

        default:
            return "Hello";
    }

    return "??";
}

void Assets_FindAssetsByType(gg_assets_t* assets, char** string_list, gg_asset_type_e type) {
    gg_asset_pair_t* pair = assets->asset_list;
    size_t i = 0;
    while (pair != NULL) {
        if (pair->asset.type == type) {
            string_list[i] = pair->name;
            i++;
        }

        pair = pair->next;
    }
}

void Assets_Destroy(gg_assets_t* assets) {
    // TODO: Cleanup
}