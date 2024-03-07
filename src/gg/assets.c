#include "assets.h"
#include "log.h"

#include <raylib.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "utils.h"

static const char* asset_file_format_strings[] = {
    "", "assets/%s.png", "assets/%s.tmj", "assets/%s.lua", "assets/%s.json", "assets/%s.json"};

void Assets_Create(gg_assets_t* assets) { assets->asset_list = NULL; }

void Assets_LoadInternals(gg_assets_t* assets, gg_window_t* window, gg_state_t* state) {
    Assets_Load(assets, window, state, ASSET_SCRIPT, "gg/map");
}

void Assets_Load(gg_assets_t* assets, gg_window_t* window, gg_state_t* state, gg_asset_type_e type, const char* name) {
    gg_asset_pair_t* pair = Assets_CreateNew(assets, type, name);

    // Generate file name
    char formatted_path[ASSETS_MAX_PATH_LENGTH] = {0};
    sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, asset_file_format_strings[type], name);

    // Load actual data
    switch (type) {
        case ASSET_TEXTURE:
            Texture_LoadFromFile(&pair->asset.data.as_tex, formatted_path);
            break;
        case ASSET_TILED_MAP:
            TiledMap_LoadFromTMJ(&pair->asset.data.as_tiled_map, formatted_path);
            break;
        case ASSET_SCRIPT:
            Script_LoadFromLua(&pair->asset.data.as_script, formatted_path);
            break;
        case ASSET_SCENE_SPEC:
            SceneSpec_LoadFromJSON(&pair->asset.data.as_scene_spec, assets, formatted_path);
            break;
        case ASSET_ACTOR_SPEC:
            ActorSpec_LoadFromJSON(&pair->asset.data.as_actor_spec, formatted_path);
            break;
        default:
            printf("ASSET LOAD: Unsupported asset type number %d...\n", type);
            break;
    }
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

gg_asset_pair_t* Assets_CreateNew(gg_assets_t* assets, gg_asset_type_e type, const char* name) {
    gg_asset_pair_t* pair = (gg_asset_pair_t*)GG_CALLOC(1, sizeof(gg_asset_pair_t));

    pair->name = (char*)GG_CALLOC(256, sizeof(char));
    TextCopy(pair->name, name);
    pair->asset.type = type;
    pair->next = NULL;

    // Easy out if first pair
    if (assets->asset_list == NULL) {
        assets->asset_list = pair;
        return pair;
    }

    // Append to linked list
    gg_asset_pair_t* last_pair = assets->asset_list;
    while (last_pair->next != NULL) {
        last_pair = last_pair->next;
    }
    last_pair->next = pair;

    return pair;
}

void Assets_SaveAssetPair(gg_assets_t* assets, gg_asset_pair_t* pair) {
    // Generate file name
    char formatted_path[ASSETS_MAX_PATH_LENGTH] = {0};
    sprintf_s(formatted_path, ASSETS_MAX_PATH_LENGTH, asset_file_format_strings[pair->asset.type], pair->name);

    FILE* asset_file = fopen(formatted_path, "w");
    if (asset_file == NULL) {
        Log_Warn(Log_TextFormat("ASSETS: Unable to write %s asset to file %s", Assets_GetTypeName(pair->asset.type),
                                formatted_path));
        return;
    }

    switch (pair->asset.type) {
        case ASSET_SCRIPT:
            fwrite(pair->asset.data.as_script.text, sizeof(char), strlen(pair->asset.data.as_script.text), asset_file);
            break;
        default:
            Log_Warn(Log_TextFormat("ASSETS: Unsupported save type %s", Assets_GetTypeName(pair->asset.type)));
            break;
    }
    fclose(asset_file);
}

const char* Assets_GetTypeName(gg_asset_type_e type) {
    switch (type) {
        case ASSET_NONE:
            return "None";

        case ASSET_SCENE_SPEC:
            return "Scene Spec";

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
    // TODO(2): Please Gus add the cleanup code :pray: :pray: :pray:
    gg_asset_pair_t* pair = assets->asset_list;
    while (pair != NULL) {
        gg_asset_pair_t* next_pair = pair->next;
        Assets_DestroyPair(assets, pair);
        pair = next_pair;
    }
}

void Assets_DestroyPair(gg_assets_t* assets, gg_asset_pair_t* pair) {
    switch (pair->asset.type) {
        case ASSET_ACTOR_SPEC:
            ActorSpec_Destroy(&pair->asset.data.as_actor_spec);
            break;
        case ASSET_SCENE_SPEC:
            SceneSpec_Destroy(&pair->asset.data.as_scene_spec);
            break;
        case ASSET_SCRIPT:
            Script_Destroy(&pair->asset.data.as_script);
            break;
        case ASSET_TEXTURE:
            Texture_Destroy(&pair->asset.data.as_tex);
            break;
        case ASSET_TILED_MAP:
            TiledMap_Destroy(&pair->asset.data.as_tiled_map);
            break;
        default:
            Log_Err(Log_TextFormat("ASSETS: Unable to destroy type %s", Assets_GetTypeName(pair->asset.type)));
            break;
    }

    GG_FREE(pair->name);
    pair->name = NULL;
    GG_FREE(pair);
    pair = NULL;
}
