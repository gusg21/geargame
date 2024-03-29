#if !defined(GG_SCENE_H)
#define GG_SCENE_H

#include <stdint.h>

#include "camera.h"
#include "scripting.h"
#include "transform.h"

#define SCENE_MAX_ACTORS 64
#define SCENE_DEFAULT_NAME "NEW_SCENE"
#define SCENE_MAX_NAME_LEN 64
#define SCENE_MAP_RENDERER_SCRIPT "gg/map"
#define SCENE_MAP_RENDERER_SETUP_NAME "set_tiled_map"

// Forward declarations for pointers
typedef struct gg_actor gg_actor_t;
typedef struct gg_actor_spec gg_actor_spec_t;
typedef struct gg_assets gg_assets_t;
typedef struct gg_scene_spec gg_scene_spec_t;
typedef struct gg_state gg_state_t;
typedef struct gg_tiled_map gg_tiled_map_t;
typedef struct gg_window gg_window_t;

// Represents a set of actors, a scripting state, and a camera. Can be loaded as an asset and swapped in and out of
// gg_state_t's ad nauseum.
typedef struct gg_scene {
    bool ok;  // Is this scene actually being used/initialized?

    char name[SCENE_MAX_NAME_LEN];  // The name of the scene
    bool paused;                    // Is this scene paused?

    gg_actor_t* actors;     // The block of actors
    uint32_t actors_alive;  // The number of actors currently activated

    gg_camera_t camera;        // This scene's camera
    gg_scripting_t scripting;  // This scene's Lua scripting state

    gg_state_t* state;  // A reference to the state that contains this scene
} gg_scene_t;

// Create an empty scene
void Scene_Create(gg_scene_t* scene, gg_window_t* window, gg_state_t* state);
// Load a scene from a .JSON spec
// void Scene_LoadFromJson(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_state_t* state,
//                         const char* path);
// Load a scene from a gg_scene_spec_t
void Scene_CreateFromSpec(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_state_t* state, gg_scene_spec_t* spec);
// Activate a non-active actor and set it up
// "script" is an optional paramter. Pass "NULL" to load no script
uint32_t Scene_NewActor(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_script_t* script);
// Create a new actor from an actor spec.
uint32_t Scene_NewActorFromSpec(gg_scene_t* scene, gg_assets_t* assets, gg_window_t* window, gg_actor_spec_t* spec);
// Create objects based on a Tiled Map object (also creates "internal" objects like the map renderer as needed)
void Scene_CreateObjectsFromTiledMap(gg_scene_t* scene, gg_window_t* window, gg_assets_t* assets, gg_tiled_map_t* tmap);
// Get a pointer to an actor by its ID (NULL if not found)
gg_actor_t* Scene_GetActorByID(gg_scene_t* scene, uint32_t id);
// Get a pointer to an actor by its name (NULL if not found)
gg_actor_t* Scene_GetActorByName(gg_scene_t* scene, const char* name);
// Tell all scripts that the scene tree has been set up and we are ready to go.
void Scene_Ready(gg_scene_t* scene);
// Tell all scripts that a new frame is happening.
void Scene_Update(gg_scene_t* scene, float delta);
// Tell all scripts to put themselves on the screen.
void Scene_Draw(gg_scene_t* scene, gg_window_t* window);
// Clean up the scene
void Scene_Destroy(gg_scene_t* scene);

#endif  // GG_SCENE_H
