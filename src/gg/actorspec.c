#include "actorspec.h"

#include <stdint.h>
#include <string.h>

#include "parson/parson.h"

#include "actor.h"
#include "log.h"

void ActorSpec_LoadFromJSON(gg_actor_spec_t* spec, const char* path) {
    // Empty the spec
    memset(spec, 0, sizeof(gg_actor_spec_t));

    // Load the JSON
    JSON_Value* json_root = json_parse_file_with_comments(path);
    if (json_root == NULL) {
        Log_Err(Log_TextFormat("ACTOR JSON: Failed to load JSON from %s", path));
    }
    JSON_Object* root_object = json_value_get_object(json_root);

    // Gather data from the JSON
    if (json_object_dothas_value_of_type(root_object, "script", JSONString)) {
        const char* script_name = json_object_get_string(root_object, "script");
        memcpy(spec->script_asset_name, script_name, ACTOR_SPEC_MAX_ASSET_NAME_LEN - 1);
    }

    if (json_object_dothas_value_of_type(root_object, "name", JSONString)) {
        const char* name = json_object_get_string(root_object, "name");
        memcpy(spec->name, name, ACTOR_MAX_NAME_LEN - 1);
    }

    if (json_object_dothas_value_of_type(root_object, "position", JSONObject)) {
        JSON_Object* pos_obj = json_object_dotget_object(root_object, "position");
        if (json_object_dothas_value_of_type(pos_obj, "x", JSONNumber) &&
            json_object_dothas_value_of_type(pos_obj, "y", JSONNumber)) {
            spec->initial_pos.x = (float)json_object_dotget_number(pos_obj, "x");
            spec->initial_pos.y = (float)json_object_dotget_number(pos_obj, "y");
        } else {
            Log_Warn(Log_TextFormat("Malformed vector in Actor Spec JSON %s", path));
        }
    }
}