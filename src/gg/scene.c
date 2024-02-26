#include "scene.h"

#include <stdio.h>
#include <string.h>

#include "parson/parson.h"

#include "actor.h"
#include "assets.h"
#include "log.h"
#include "state.h"
#include "tiled.h"
#include "utils.h"

void Scene_Create(gg_scene_t* scene, gg_window_t* window, gg_state_t* state) {
    // Name
    scene->name = (char*)malloc(sizeof(char) * SCENE_DEFAULT_NAME_LEN);
    sprintf_s(scene->name, SCENE_DEFAULT_NAME_LEN, "%s", SCENE_DEFAULT_NAME);

    // Actors
    scene->actors = calloc(SCENE_MAX_ACTORS, sizeof(gg_actor_t));
    scene->actors_alive = 0;

    // Reset Actors
    for (int i = 0; i < SCENE_MAX_ACTORS; i++) {
        gg_actor_t* actor = &scene->actors[i];
        Actor_ResetActor(actor);
    }

    // Save the state pointer
    scene->state = state;

    // Make our camera
    Camera_Create(&scene->camera, Window_GetWidth(window), Window_GetHeight(window), 0, 0);

    // Initialize the Lua scripting state
    Scripting_Initialize(&scene->scripting, true);
}

void Scene_LoadFromJson(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_state_t* state,
                        const char* path) {
    // Create empty scene
    Scene_Create(scene, window, state);

    // Load JSON
    JSON_Value* root = json_parse_file_with_comments(path);
    if (root == NULL) {
        Log_Err(Log_TextFormat("SCENE JSON: Failed to load JSON from %s", path));
        return;
    }
    JSON_Object* root_obj = json_object(root);

    // Scene Name
    if (json_object_dothas_value_of_type(root_obj, "name", JSONString)) {
        const char* scene_name = json_object_dotget_string(root_obj, "name");
        size_t scene_name_len = json_object_dotget_string_len(root_obj, "name");

        if (scene->name != NULL) {
            free(scene->name);
        }
        scene->name = calloc(scene_name_len, sizeof(char));
        sprintf_s(scene->name, scene_name_len, "%s", scene_name);
    }

    // Scene Map
    if (json_object_dothas_value_of_type(root_obj, "map", JSONString)) {
        const char* map_name = json_object_dotget_string(root_obj, "map");

        gg_asset_t* map_asset;
        bool found = Assets_Get(assets, &map_asset, map_name);
        if (found) {
            Scene_CreateObjectsFromTiledMap(scene, window, assets, &map_asset->data.as_tiled_map);
        } else {
            Log_Err(Log_TextFormat("Unable to load tiled map asset %s", map_name));
        }
    }
}

uint32_t Scene_GetNewAndActivate(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_script_t* script) {
    for (uint32_t actor_id = 0; actor_id < SCENE_MAX_ACTORS; actor_id++) {
        gg_actor_t* actor = &scene->actors[actor_id];

        if (!actor->alive) {
            Actor_ResetActor(actor);

            actor->parent_id = ACTOR_ROOT;
            actor->actor_id = actor_id;
            actor->alive = true;
            actor->visible = true;

            scene->actors_alive++;

            if (script != NULL) {
                actor->script_handle = Scripting_LoadScript(&scene->scripting, script);
                Actor_CallScriptFunctionWithPointerBouquet(actor, &scene->scripting, "initialize", scene->state, window, assets);
            }

            return actor_id;
        }
    }

    return ACTOR_INVALID;
}

void Scene_CreateObjectsFromTiledMap(gg_scene_t* scene, gg_window_t* window, gg_assets_t* assets,
                                     gg_tiled_map_t* tmap) {
    if (tmap == NULL) {
        printf("ERROR: Invalid tiled map");
        return;
    }

    // Load map object
    gg_asset_t* asset;
    bool valid = Assets_Get(assets, &asset, SCENE_MAP_RENDERER_SCRIPT);
    if (valid) {
        gg_script_t* script = &asset->data.as_script;
        uint32_t id = Scene_GetNewAndActivate(scene, assets, window, script);
        gg_actor_t* actor = Scene_GetActorByID(scene, id);

        Actor_CallScriptFunctionWithPointer(actor, &scene->scripting, SCENE_MAP_RENDERER_SETUP_NAME,
                                                   tmap);

        actor->name = malloc(sizeof(char) * 64);
        TextCopy(actor->name, "map renderer");
    }

    // Load objects
    gg_tiled_object_t* object = tmap->objs;
    while (object != NULL) {
        gg_script_t* script = NULL;
        if (object->script_name != NULL) {
            gg_asset_t* asset;
            bool valid = Assets_Get(assets, &asset, object->script_name);
            if (valid) {
                script = &asset->data.as_script;
            }
        }

        uint32_t id = Scene_GetNewAndActivate(scene, assets, window, script);
        gg_actor_t* actor = Scene_GetActorByID(scene, id);

        actor->transform.pos.x = (float)object->x;
        actor->transform.pos.y = (float)object->y;

        actor->name = malloc(sizeof(char) * 64);
        TextCopy(actor->name, object->name);

        object = object->next;
    }
}

gg_actor_t* Scene_GetActorByID(gg_scene_t* scene, uint32_t id) {
    for (size_t i = 0; i < SCENE_MAX_ACTORS; i++) {
        gg_actor_t* actor = &scene->actors[i];
        if (actor->actor_id == id) {
            return actor;
        }
    }
    return NULL;
}

gg_actor_t* Scene_GetActorByName(gg_scene_t* scene, const char* name) {
    for (int i = 0; i < SCENE_MAX_ACTORS; i++) {
        gg_actor_t* actor = &scene->actors[i];
        if (TextIsEqual(actor->name, name)) {
            return actor;
        }
    }
    return NULL;
}

void Scene_Ready(gg_scene_t* scene) {
    for (int i = 0; i < SCENE_MAX_ACTORS; i++) {
        gg_actor_t* actor = &scene->actors[i];
        if (actor->alive) {
            Actor_CallScriptFunction(actor, &scene->scripting, "ready");
        }
    }
}

void Scene_Update(gg_scene_t* scene, float delta) {
    for (int i = 0; i < SCENE_MAX_ACTORS; i++) {
        gg_actor_t* actor = &scene->actors[i];
        if (actor->alive) {
            Actor_CallScriptFunctionWithFloat(actor, &scene->scripting, "update", delta);
        }
    }

    // TODO: Better way/place to GC?
    Scripting_DoGarbageCollection(&scene->scripting);
}

void Scene_Draw(gg_scene_t* scene, gg_window_t* window) {
    for (int i = 0; i < SCENE_MAX_ACTORS; i++) {
        gg_actor_t* actor = &scene->actors[i];
        if (actor->visible) {
            Actor_CallScriptFunction(actor, &scene->scripting, "draw");
        }
    }
}
