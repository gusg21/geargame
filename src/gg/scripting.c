#include "scripting.h"

#include <stdio.h>
#include <stdlib.h>

#include "ldefs.h"
#include "log.h"
#include "lua54/lauxlib.h"
#include "lua54/lualib.h"

void Script_LoadFromLua(gg_script_t* code, const char* path) {
    code->text = (char*)calloc(sizeof(char), SCRIPT_MAX_LENGTH);

    FILE* code_file = fopen(path, "r");
    if (code_file == NULL) {
        printf("ERROR: Error opening script from %s\n", path);
        code->ok = false;
        return;
    }

    size_t amt_read = fread(code->text, sizeof(char), SCRIPT_MAX_LENGTH - 1, code_file);
    if (amt_read > SCRIPT_MAX_LENGTH - 1) {
        printf("ERROR: Error reading script from %s\n", path);
        code->ok = false;
        fclose(code_file);
        return;
    }

    fclose(code_file);
    code->ok = true;
}

void Scripting_Initialize(gg_scripting_t* script, bool use_auto_gc) {
    script->next_handle = 1;

    script->state = luaL_newstate();
    if (use_auto_gc) lua_gc(script->state, LUA_GCSTOP);

    luaL_openlibs(script->state);
    LDefs_LoadIntoScripting(script);
}

uint32_t Scripting_LoadScript(gg_scripting_t* script, gg_script_t* code) {
    // printf("Loading script %s\n", data);
    if (code == NULL) {
        printf("ERROR (loading): no code!\n");
        return SCRIPTING_BAD_HANDLE;
    }
    if (!code->ok) {
        printf("ERROR (loading): bad code!\n");
        return SCRIPTING_BAD_HANDLE;
    }

    char handle_string[16] = {0};
    uint32_t handle = script->next_handle++;
    sprintf(handle_string, SCRIPTING_HANDLE_FORMAT, handle);

    if (luaL_dostring(script->state, code->text) != LUA_OK) {
        printf("ERROR (loading): %s\n", lua_tostring(script->state, -1));
    }
    lua_setglobal(script->state, handle_string);

    lua_settop(script->state, 0);  // Empty the stack

    return handle;
}

bool Scripting_IsOk(gg_scripting_t* script) {
    if (script == NULL) return false;
    if (script->state == NULL) return false;
    return true;
}

void Scripting_DoGarbageCollection(gg_scripting_t* script) { lua_gc(script->state, LUA_GCCOLLECT); }

void Scripting_Call(gg_scripting_t* script, const char* func_name, uint32_t handle) {
    if (!Scripting_IsOk(script)) return;

    char handle_string[16] = {0};
    sprintf(handle_string, SCRIPTING_HANDLE_FORMAT, handle);

    lua_getglobal(script->state, handle_string);  // [table]
    lua_getfield(script->state, -1, func_name);   // [table] [function]
    if (!lua_isnil(script->state, -1)) {
        lua_pushvalue(script->state, -2);                   // [table] [function] [table]
        if (lua_pcall(script->state, 1, 0, 0) != LUA_OK) {  // [table]
            char* err_text = Log_TextFormat("ERROR (func %s): %s\n", func_name, lua_tostring(script->state, -1));
            Log_Err(err_text);
            free(err_text);
        }
    } else {
        // printf("nil func_name %s\n", func_name);
    }
    lua_settop(script->state, 0);  // Empty the stack
}

void Scripting_CallWithFloat(gg_scripting_t* script, const char* func_name, uint32_t handle, float f) {
    if (!Scripting_IsOk(script)) return;

    char handle_string[16] = {0};
    sprintf(handle_string, SCRIPTING_HANDLE_FORMAT, handle);

    lua_getglobal(script->state, handle_string);  // [table]
    lua_getfield(script->state, -1, func_name);   // [table] [function]
    if (!lua_isnil(script->state, -1)) {
        lua_pushvalue(script->state, -2);                   // [table] [function] [table]
        lua_pushnumber(script->state, f);                   // [table] [function] [table] [float]
        if (lua_pcall(script->state, 2, 0, 0) != LUA_OK) {  // [table]
            char* err_text = Log_TextFormat("ERROR (func %s): %s\n", func_name, lua_tostring(script->state, -1));
            Log_Err(err_text);
            free(err_text);
        }
    } else {
        // printf("nil func_name %s\n", func_name);
    }
    lua_settop(script->state, 0);  // Empty the stack
}

void Scripting_CallWithPointer(gg_scripting_t* script, const char* func_name, uint32_t handle, void* pointer) {
    if (!Scripting_IsOk(script)) return;

    char handle_string[16] = {0};
    sprintf(handle_string, SCRIPTING_HANDLE_FORMAT, handle);

    lua_getglobal(script->state, handle_string);  // [table]
    lua_getfield(script->state, -1, func_name);   // [table] [function]
    if (!lua_isnil(script->state, -1)) {
        lua_pushvalue(script->state, -2);                   // [table] [function] [table]
        lua_pushlightuserdata(script->state, pointer);      // [table] [function] [table] [ptr]
        if (lua_pcall(script->state, 2, 0, 0) != LUA_OK) {  // [table]
            char* err_text = Log_TextFormat("ERROR (func %s): %s\n", func_name, lua_tostring(script->state, -1));
            Log_Err(err_text);
            free(err_text);
        }
    } else {
        // printf("nil func_name %s\n", func_name);
    }
    lua_settop(script->state, 0);  // Empty the stack
}

void Scripting_CallWithTwoPointers(gg_scripting_t* script, const char* func_name, uint32_t handle, void* pointer1,
                                   void* pointer2) {
    if (!Scripting_IsOk(script)) return;

    char handle_string[16] = {0};
    sprintf(handle_string, SCRIPTING_HANDLE_FORMAT, handle);

    lua_getglobal(script->state, handle_string);  // [table]
    lua_getfield(script->state, -1, func_name);   // [table] [function]
    if (!lua_isnil(script->state, -1)) {
        lua_pushvalue(script->state, -2);                   // [table] [function] [table]
        lua_pushlightuserdata(script->state, pointer1);     // [table] [function] [table] [ptr1]
        lua_pushlightuserdata(script->state, pointer2);     // [table] [function] [table] [ptr1] [ptr2]
        if (lua_pcall(script->state, 3, 0, 0) != LUA_OK) {  // [table]
            char* err_text = Log_TextFormat("ERROR (func %s): %s\n", func_name, lua_tostring(script->state, -1));
            Log_Err(err_text);
            free(err_text);
        }
    } else {
        // printf("nil func_name %s\n", func_name);
    }
    lua_settop(script->state, 0);  // Empty the stack
}

void Scripting_CallWithPointerBouquet(gg_scripting_t* script, const char* func_name, uint32_t handle, gg_actor_t* actor,
                                      gg_state_t* state, gg_window_t* window, gg_assets_t* assets) {
    if (!Scripting_IsOk(script)) return;

    char handle_string[16] = {0};
    sprintf(handle_string, SCRIPTING_HANDLE_FORMAT, handle);

    lua_getglobal(script->state, handle_string);  // [table]
    lua_getfield(script->state, -1, func_name);   // [table] [function]
    if (!lua_isnil(script->state, -1)) {
        lua_pushvalue(script->state, -2);              // [table] [function] [table]
        lua_pushlightuserdata(script->state, actor);   // [table] [function] [table] [actor]
        lua_pushlightuserdata(script->state, state);   // [table] [function] [table] [actor] [state]
        lua_pushlightuserdata(script->state, window);  // [table] [function] [table] [actor] [state] [window]
        lua_pushlightuserdata(script->state, assets);  // [table] [function] [table] [actor] [state] [window] [assets]
        if (lua_pcall(script->state, 5, 0, 0) != LUA_OK) {  // [table]
            char* err_text = Log_TextFormat("ERROR (func %s): %s\n", func_name, lua_tostring(script->state, -1));
            Log_Err(err_text);
            free(err_text);
        }
    } else {
        // printf("nil func_name %s\n", func_name);
    }
    lua_settop(script->state, 0);  // Empty the stack
}
