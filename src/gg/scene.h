#if !defined(GG_SCENE_H)
#define GG_SCENE_H

#define SCENE_MAX_ACTORS 1024
#define SCENE_DEFAULT_NAME "NEW_SCENE"
#define SCENE_DEFAULT_NAME_LEN sizeof(SCENE_DEFAULT_NAME)
#define SCENE_MAP_RENDERER_SCRIPT "gg/map"
#define SCENE_MAP_RENDERER_SETUP_NAME "set_tiled_map"

#include <stdint.h>

#include "camera.h"
#include "scripting.h"
#include "transform.h"

typedef struct gg_assets gg_assets_t;
typedef struct gg_tiled_map gg_tiled_map_t;
typedef struct gg_state gg_state_t;
typedef struct gg_actor gg_actor_t;
typedef struct gg_window gg_window_t;

typedef struct gg_scene {
    // TODO: please don't let this struct get bloated :(
    char* name;

    gg_actor_t* actors;
    uint32_t actors_alive;

    gg_camera_t camera;
    gg_scripting_t scripting;

    gg_state_t* state;
} gg_scene_t;

void Scene_Create(gg_scene_t* scene, gg_window_t* window, gg_state_t* state);
void Scene_LoadFromJson(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_state_t* state,
                        const char* path);
uint32_t Scene_GetNewAndActivate(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_script_t* script);
void Scene_CreateObjectsFromTiledMap(gg_scene_t* scene, gg_window_t* window, gg_assets_t* assets, gg_tiled_map_t* tmap);
gg_actor_t* Scene_GetActorByID(gg_scene_t* scene, uint32_t id);
gg_actor_t* Scene_GetActorByName(gg_scene_t* scene, const char* name);
void Scene_Ready(gg_scene_t* scene);
void Scene_Update(gg_scene_t* scene, float delta);
void Scene_Draw(gg_scene_t* scene, gg_window_t* window);

#endif  // GG_SCENE_H
