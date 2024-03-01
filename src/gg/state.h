#if !defined(GG_STATE_H)
#define GG_STATE_H

#include "keys.h"
#include "scene.h"
#include "tiled.h"
#include "window.h"
#include "editor/editor.h"

typedef struct gg_state {
    gg_scene_t* current_scene;
    gg_keys_t keys;
#ifdef GG_EDITOR
    gg_editor_t editor;
#endif

    bool wants_exit;
} gg_state_t;

void State_Init(gg_state_t* state);
void State_SetCurrentScene(gg_state_t* state, gg_scene_t* scene);
void State_DoLoop(gg_state_t* state, gg_assets_t* assets, gg_window_t* window);
void State_Tick(gg_state_t* state, gg_window_t* window);
#ifdef GG_EDITOR
void State_TickEditor(gg_state_t* state, gg_assets_t* assets, gg_window_t* window);
#endif
void State_Draw(gg_state_t* state, gg_window_t* window);
void State_Quit(gg_state_t* state);
void State_Destroy(gg_state_t* state);

#endif  // GG_STATE_H