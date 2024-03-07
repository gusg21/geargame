#if !defined(GG_SCENE_SPEC_H)
#define GG_SCENE_SPEC_H

#include "actorspec.h"
#include "tiled.h"

#define SCENE_SPEC_MAX_NAME_LEN 64
#define SCENE_SPEC_MAX_ACTOR_SPECS 64

typedef struct gg_assets gg_assets_t;

typedef struct gg_scene_spec {
    char name[SCENE_SPEC_MAX_NAME_LEN];  // Name of the scene

    gg_actor_spec_t*
        actor_specs[SCENE_SPEC_MAX_ACTOR_SPECS];  // Array of actor spec assets to instantiate for this scene
    gg_tiled_map_t* tiled_map;                    // The tile map to create extra from
} gg_scene_spec_t;

void SceneSpec_Create(gg_scene_spec_t* scene_spec);
void SceneSpec_LoadFromJSON(gg_scene_spec_t* scene_spec, gg_assets_t* assets, const char* path);
void SceneSpec_Destroy(gg_scene_spec_t* scene_spec);

#endif  // GG_SCENE_SPEC_H
