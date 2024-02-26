#include "ldefs.h"

#include "actor.h"
#include "keys.h"
#include "log.h"
#include "scene.h"
#include "state.h"
#include "tiled.h"
#include "window.h"

// STATE FUNCTIONS
LDEF(State_GetScene) {
    gg_state_t* state = (gg_state_t*)lua_topointer(L, 1);

    if (state != NULL) {
        lua_pushlightuserdata(L, state->current_scene);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

LDEF(State_GetKeys) {
    gg_state_t* state = (gg_state_t*)lua_topointer(L, 1);

    if (state != NULL) {
        lua_pushlightuserdata(L, &state->keys);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

// SCENE FUNCTIONS
LDEF(Scene_GetCamera) {
    gg_scene_t* scene = (gg_scene_t*)lua_topointer(L, 1);

    if (scene != NULL) {
        lua_pushlightuserdata(L, &scene->camera);
    } else {
        lua_pushnil(L);
    }

    return 1;
}

LDEF(Scene_GetState) {
    gg_scene_t* scene = (gg_scene_t*)lua_topointer(L, 1);

    lua_pushlightuserdata(L, &scene->state);

    return 1;
}

LDEF(Scene_GetActorByName) {
    gg_scene_t* scene = (gg_scene_t*)lua_topointer(L, 1);
    const char* name = (const char*)lua_tostring(L, 2);

    if (scene != NULL) {
        lua_pushlightuserdata(L, Scene_GetActorByName(scene, name));
    } else {
        lua_pushnil(L);
    }

    return 1;
}

// ASSETS FUNCTIONS
static void LDefs_S_PushAssetValue(lua_State* L, gg_asset_t* asset, int asset_type) {
    switch (asset_type) {
        case ASSET_SCENE:
            lua_pushlightuserdata(L, &asset->data.as_scene);
            break;
        case ASSET_SCRIPT:
            lua_pushlightuserdata(L, &asset->data.as_script);
            break;
        case ASSET_TEXTURE:
            lua_pushlightuserdata(L, &asset->data.as_tex);
            break;
        case ASSET_TILED_MAP:
            lua_pushlightuserdata(L, &asset->data.as_tiled_map);
            break;
        default:
            lua_pushnil(L);
            break;
    }
}

LDEF(Assets_GetAsset) {
    gg_assets_t* assets = (gg_assets_t*)lua_topointer(L, 1);

    int type = lua_tointeger(L, 2);
    const char* name = (const char*)lua_tostring(L, 3);

    gg_asset_t* asset;
    bool valid = Assets_Get(assets, &asset, name);
    if (valid) {
        lua_pushlightuserdata(L, asset);
        return 1;
    } else {
        lua_pushnil(L);
        return 1;
    }
}

LDEF(Assets_Get) {
    gg_assets_t* assets = (gg_assets_t*)lua_topointer(L, 1);

    int type = lua_tointeger(L, 2);
    const char* name = (const char*)lua_tostring(L, 3);

    if (assets != NULL) {
        gg_asset_t* asset;
        bool valid = Assets_Get(assets, &asset, name);
        if (valid) {
            LDefs_S_PushAssetValue(L, asset, type);
            return 1;
        } else {
            lua_pushnil(L);
            return 1;
        }
    } else {
        lua_pushnil(L);
        return 1;
    }
}

// ASSET FUNCTIONS
LDEF(Asset_GetValue) {
    gg_asset_t* asset = (gg_asset_t*)lua_topointer(L, 1);
    int asset_type = lua_tointeger(L, 2);

    LDefs_S_PushAssetValue(L, asset, asset_type);

    return 1;
}

// CAMERA FUNCTIONS
LDEF(Camera_SetPosition) {
    gg_camera_t* camera = (gg_camera_t*)lua_topointer(L, 1);

    int32_t x = lua_tonumber(L, 2);
    int32_t y = lua_tonumber(L, 3);

    if (camera != NULL) {
        camera->x = x;
        camera->y = y;
    } else {
        Log_Err("LUA: NULL camera to Camera_SetPosition");
    }

    return 0;
}

LDEF(Camera_SetZoom) {
    gg_camera_t* camera = (gg_camera_t*)lua_topointer(L, 1);

    float zoom = lua_tonumber(L, 2);

    if (camera != NULL) {
        camera->zoom = zoom;
    } else {
        Log_Err("LUA: NULL camera to Camera_SetZoom");
    }

    return 0;
}

// ACTOR FUNCTIONS
LDEF(Actor_GetPosition) {
    gg_actor_t* actor = (gg_actor_t*)lua_topointer(L, 1);

    if (actor != NULL) {
        lua_pushnumber(L, actor->transform.pos.x);
        lua_pushnumber(L, actor->transform.pos.y);
    } else {
        lua_pushnil(L);
        lua_pushnil(L);
    }

    return 2;
}

LDEF(Actor_SetPosition) {
    gg_actor_t* actor = (gg_actor_t*)lua_topointer(L, 1);

    int32_t x = lua_tointeger(L, 2);
    int32_t y = lua_tointeger(L, 3);

    actor->transform.pos.x = x;
    actor->transform.pos.y = y;

    return 0;
}

// TILED MAP FUNCTIONS
LDEF(TiledMap_Draw) {
    gg_tiled_map_t* map = (gg_tiled_map_t*)lua_topointer(L, 1);
    gg_window_t* window = (gg_window_t*)lua_topointer(L, 2);

    int32_t x = lua_tointeger(L, 2);
    int32_t y = lua_tointeger(L, 3);

    TiledMap_Draw(map, window, x, y);

    return 0;
}

// WINDOW FUNCTIONS
LDEF(Window_DrawRectangle) {
    gg_window_t* window = (gg_window_t*)lua_topointer(L, 1);

    int32_t x = lua_tointeger(L, 2);
    int32_t y = lua_tointeger(L, 3);
    uint32_t w = lua_tointeger(L, 4);
    uint32_t h = lua_tointeger(L, 5);

    Window_DrawRectangle(window, x, y, w, h, COL(255, 128, 128));

    return 0;
}

LDEF(Window_DrawTexture) {
    gg_window_t* window = (gg_window_t*)lua_topointer(L, 1);
    gg_texture_t* tex = (gg_texture_t*)lua_topointer(L, 2);

    float x = lua_tonumber(L, 3);
    float y = lua_tonumber(L, 4);

    if (tex != NULL) {
        Window_DrawTexture(window, tex, x, y);
    } else {
        Log_Err("LUA: NULL tex passed to Window_DrawTexture");
    }

    return 0;
}

// KEYBOARD FUNCTIONS
LDEF(Keys_KeyDown) {
    gg_keys_t* keys = (gg_keys_t*)lua_topointer(L, 1);

    int key = lua_tointeger(L, 2);

    bool down = Keys_KeyDown(keys, key);
    lua_pushboolean(L, down);

    return 1;
}

// UTILITY FUNCTIONS
LDEF(IsNull) {
    const void* ptr = lua_topointer(L, 1);

    lua_pushboolean(L, ptr == NULL);

    return 1;
}

void LDefs_LoadIntoScripting(gg_scripting_t* scripting) {
    // Globals
    lua_pushinteger(scripting->state, ASSET_NONE);
    lua_setglobal(scripting->state, "ASSET_NONE");

    lua_pushinteger(scripting->state, ASSET_SCENE);
    lua_setglobal(scripting->state, "ASSET_SCENE");

    lua_pushinteger(scripting->state, ASSET_SCRIPT);
    lua_setglobal(scripting->state, "ASSET_SCRIPT");

    lua_pushinteger(scripting->state, ASSET_TEXTURE);
    lua_setglobal(scripting->state, "ASSET_TEXTURE");

    lua_pushinteger(scripting->state, ASSET_TILED_MAP);
    lua_setglobal(scripting->state, "ASSET_TILED_MAP");

    // Functions
    LDEF_LOAD(scripting, State_GetKeys);
    LDEF_LOAD(scripting, State_GetScene);

    LDEF_LOAD(scripting, Scene_GetCamera);
    LDEF_LOAD(scripting, Scene_GetActorByName);

    LDEF_LOAD(scripting, Assets_Get);

    LDEF_LOAD(scripting, Camera_SetZoom);
    LDEF_LOAD(scripting, Camera_SetPosition);

    LDEF_LOAD(scripting, Actor_GetPosition);
    LDEF_LOAD(scripting, Actor_SetPosition);

    LDEF_LOAD(scripting, TiledMap_Draw);

    LDEF_LOAD(scripting, Window_DrawRectangle);
    LDEF_LOAD(scripting, Window_DrawTexture);

    LDEF_LOAD(scripting, Keys_KeyDown);

    LDEF_LOAD(scripting, IsNull);
}
