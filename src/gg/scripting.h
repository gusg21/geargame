#if !defined(GG_SCRIPTING_H)
#define GG_SCRIPTING_H

#include <stdbool.h>
#include <stdint.h>

#include "lua54/lua.h"

#define SCRIPTING_HANDLE_FORMAT "hnd__%d"
#define SCRIPTING_BAD_HANDLE 0

#define SCRIPT_MAX_LENGTH 4096

typedef struct gg_actor gg_actor_t;
typedef struct gg_state gg_state_t;
typedef struct gg_window gg_window_t;
typedef struct gg_assets gg_assets_t;

typedef struct gg_scripting {
    lua_State* state;
    uint32_t next_handle;
} gg_scripting_t;

typedef struct gg_script {
    char* text;
    bool ok;
} gg_script_t;

void Script_LoadFromLua(gg_script_t* code, const char* path);

void Scripting_Initialize(gg_scripting_t* script, bool use_auto_gc);
uint32_t Scripting_LoadScript(gg_scripting_t* script, gg_script_t* code);
uint32_t Scripting_ReloadScript(gg_scripting_t* script, gg_script_t* code, uint32_t handle);
bool Scripting_IsOk(gg_scripting_t* script);
void Scripting_DoGarbageCollection(gg_scripting_t* script);
void Scripting_GetHandleString(gg_scripting_t* script, char* string, size_t str_len, uint32_t handle);
int32_t Scripting_RunCode(gg_scripting_t* script, const char* code);
const char* Scripting_GetError(gg_scripting_t* script);
void Scripting_Call(gg_scripting_t* script, const char* func_name, uint32_t handle);
void Scripting_CallWithFloat(gg_scripting_t* script, const char* func_name, uint32_t handle, float f);
void Scripting_CallWithPointer(gg_scripting_t* script, const char* func_name, uint32_t handle, void* pointer);
void Scripting_CallWithTwoPointers(gg_scripting_t* script, const char* func_name, uint32_t handle, void* pointer1,
                                   void* pointer2);
void Scripting_CallWithPointerBouquet(gg_scripting_t* script, const char* func_name, uint32_t handle, gg_actor_t* actor,
                                      gg_state_t* state, gg_window_t* window, gg_assets_t* assets);
void Scripting_SetPointerBouquet(gg_scripting_t* script, uint32_t handle, gg_actor_t* actor, gg_state_t* state,
                                 gg_window_t* window, gg_assets_t* assets);
#endif  // GG_SCRIPTING_H
