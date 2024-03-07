#include "scenespec.h"

#include "parson/parson.h"
#include "assets.h"
#include "log.h"
#include "utils.h"

#include <string.h>

void SceneSpec_Create(gg_scene_spec_t* scene_spec) {
    // I'm a fraud
}

void SceneSpec_LoadFromJSON(gg_scene_spec_t* scene_spec, gg_assets_t* assets, const char* path) {
    // Load JSON
    JSON_Value* json_root = json_parse_file_with_comments(path);
    if (json_root == NULL) {
        Log_Err(Log_TextFormat("SCENE JSON: Failed to load JSON from %s", path));
        return;
    }
    JSON_Object* root_obj = json_object(json_root);

    // Scene Name
    if (json_object_dothas_value_of_type(root_obj, "name", JSONString)) {
        const char* scene_name = json_object_dotget_string(root_obj, "name");
        size_t scene_name_len = json_object_dotget_string_len(root_obj, "name");
        
        sprintf_s(scene_spec->name, SCENE_SPEC_MAX_NAME_LEN, "%s", scene_name);
    }

    // Scene Map
    if (json_object_dothas_value_of_type(root_obj, "map", JSONString)) {
        const char* map_name = json_object_dotget_string(root_obj, "map");

        gg_asset_t* map_asset;
        bool found = Assets_Get(assets, &map_asset, map_name);
        if (found) {
            scene_spec->tiled_map = &map_asset->data.as_tiled_map;
        } else {
            Log_Err(Log_TextFormat("Unable to load tiled map asset %s", map_name));
        }
    }
}

void SceneSpec_Destroy(gg_scene_spec_t* scene_spec) {
    // I'm a fraud
}
