#if !defined(GG_ACTOR_H)
#define GG_ACTOR_H

#include <stdbool.h>

#include "assets.h"
#include "scene.h"
#include "scripting.h"
#include "transform.h"
#include "window.h"

#define ACTOR_INVALID 0xFFFFFFFF
#define ACTOR_ROOT 0xFFFFFFFE

// An actor in game. Has a transform and a script handle
typedef struct gg_actor {
    char _internal[4];  // Makes it easier to see unnamed actors in the memory viewer. Probably no longer needed at some
                        // point.
    char name[32];      // Name of the actor
    bool alive;         // Should we get updates? (NOTE: Don't modify directly!)
    bool visible;       // Should we get drawn?
    gg_transform_t transform;  // Our local transform
    uint32_t parent_id;        // The parent ID within the scene
    uint32_t actor_id;         // Our ID within the scene
    uint32_t script_handle;    // The handle to our table of data 'n' functions
} gg_actor_t;

void Actor_ResetActor(gg_actor_t* actor);
void Actor_CallScriptFunction(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name);
void Actor_CallScriptFunctionWithFloat(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name, float f);
void Actor_CallScriptFunctionWithPointer(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name,
                                         void* pointer);
void Actor_CallScriptFunctionWithTwoPointers(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name,
                                             void* pointer1, void* pointer2);
void Actor_CallScriptFunctionWithPointerBouquet(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name,
                                                gg_state_t* state, gg_window_t* window, gg_assets_t* assets);

#endif  // GG_ACTOR_H
