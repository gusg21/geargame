#include "actor.h"

#include <string.h>

#include "log.h"
#include "scripting.h"

void Actor_ResetActor(gg_actor_t* actor) {
    actor->_internal[0] = 'A';
    actor->_internal[1] = 'C';
    actor->_internal[2] = 'T';
    actor->_internal[3] = 'O';

    memset(actor->name, 0, 32);
    actor->alive = false;
    actor->visible = false;
    actor->transform = (gg_transform_t){
        .pos = V2ZERO(),
        .rot = 0.f,
        .scale = V2ONE(),
    };
    actor->parent_id = ACTOR_ROOT;
    actor->actor_id = ACTOR_INVALID;
    actor->script_handle = SCRIPTING_BAD_HANDLE;
};

// TODO: This is the worst naming convention ever. Absolute affront to god

void Actor_CallScriptFunction(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name) {
    if (actor->script_handle != SCRIPTING_BAD_HANDLE) {
        Scripting_Call(scripting, func_name, actor->script_handle);
    }
}

void Actor_CallScriptFunctionWithFloat(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name, float f) {
    if (actor->script_handle != SCRIPTING_BAD_HANDLE) {
        Scripting_CallWithFloat(scripting, func_name, actor->script_handle, f);
    }
}

void Actor_CallScriptFunctionWithPointer(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name,
                                         void* pointer) {
    if (actor->script_handle != SCRIPTING_BAD_HANDLE) {
        Scripting_CallWithPointer(scripting, func_name, actor->script_handle, pointer);
    }
}

void Actor_CallScriptFunctionWithTwoPointers(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name,
                                             void* pointer1, void* pointer2) {
    if (actor->script_handle != SCRIPTING_BAD_HANDLE) {
        Scripting_CallWithTwoPointers(scripting, func_name, actor->script_handle, pointer1, pointer2);
    }
}

void Actor_CallScriptFunctionWithPointerBouquet(gg_actor_t* actor, gg_scripting_t* scripting, const char* func_name,
                                                gg_state_t* state, gg_window_t* window, gg_assets_t* assets) {
    if (actor->script_handle != SCRIPTING_BAD_HANDLE) {
        Scripting_CallWithPointerBouquet(scripting, func_name, actor->script_handle, actor, state, window, assets);
    }
}

void Actor_SetPointerBouquet(gg_actor_t* actor, gg_scripting_t* scripting, gg_state_t* state, gg_window_t* window,
                             gg_assets_t* assets) {
    if (actor->script_handle != SCRIPTING_BAD_HANDLE) {
        Scripting_SetPointerBouquet(scripting, actor->script_handle, actor, state, window, assets);
    }
}