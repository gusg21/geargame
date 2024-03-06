#include "cimgui_impl_raylib.h"

#include "assets.h"
#include "color.h"
#include "log.h"
#include "scene.h"
#include "state.h"
#include "tiled.h"

void State_Init(gg_state_t* state) {
#ifdef GG_EDITOR
    igCreateContext(NULL);

    ImGui_ImplRaylib_Init();
    igStyleColorsDark(NULL);

#endif

    Keys_Create(&state->keys);
    state->current_scene = NULL;

    #ifdef GG_EDITOR
    state->render_to_window = false;
    #else
    state->render_to_window = true;
    #endif
    state->wants_exit = false;
}

void State_SetCurrentScene(gg_state_t* state, gg_scene_t* scene) {
    state->current_scene = scene;
#ifdef GG_EDITOR
    Editor_Create(&state->editor, &scene->scripting);
#endif
}

void State_DoLoop(gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    if (state->current_scene != NULL) {
        Scene_Ready(state->current_scene);
    }

    while (!Window_ShouldClose(window)) {
#ifdef GG_EDITOR
        ImGuiIO* io = igGetIO();

        ImGui_ImplRaylib_ProcessEvent();

        ImGui_ImplRaylib_NewFrame();
        igNewFrame();
#endif

        State_Tick(state, window);

#ifdef GG_EDITOR
        State_TickEditor(state, assets, window);
#endif

        State_Draw(state, window);

        if (state->wants_exit) break;
    }

    State_Destroy(state);
}

void State_Tick(gg_state_t* state, gg_window_t* window) {
    float delta = Window_GetDeltaTime(window);

    if (state->current_scene != NULL) {
        Scene_Update(state->current_scene, delta);
    }

    if (1.f / delta < 30.f) {
        Log_Warn(Log_TextFormat("Dead frame! (@%.2fFPS)", 1.f / delta));
    }
}

#ifdef GG_EDITOR
void State_TickEditor(gg_state_t* state, gg_assets_t* assets, gg_window_t* window) {
    Editor_Update(&state->editor, state, assets, window);
}
#endif

void State_Draw(gg_state_t* state, gg_window_t* window) {
    Window_ClearScreen(window, COL(10, 20, 30));

    if (state->current_scene != NULL) {
        Window_SetTarget(window);
        {
            Window_ClearScreen(window, COL(10, 20, 30));
            
            // Background Space Rendering
            Window_BeginParallaxCameraDrawing(window, &state->current_scene->camera, 0.2f);
            { Window_DrawSpaceOrigin(window); }
            Window_EndCameraDrawing(window, &state->current_scene->camera);

            // World Space Rendering
            Window_BeginCameraDrawing(window, &state->current_scene->camera);
            {
                Scene_Draw(state->current_scene, window);
                Window_DrawSpaceOrigin(window);
            }
            Window_EndCameraDrawing(window, &state->current_scene->camera);
        }
        Window_ReleaseTarget(window);

        Window_BeginDrawing(window);
        {
            if (state->render_to_window) {
                Window_DrawWindowTexture(window);
            }

#ifdef GG_EDITOR
            ImGui_ImplRaylib_Render();
            // igUpdatePlatformWindows();
            // igRenderPlatformWindowsDefault(NULL, NULL);
#endif
        

        }
        Window_EndDrawing(window);

    } else {
        Window_BeginDrawing(window);
        { Window_DrawRectangle(window, 0, 0, Window_GetWidth(window), Window_GetHeight(window), COL(220, 200, 10)); }
        Window_EndDrawing(window);
    }
}

void State_Quit(gg_state_t* state) { state->wants_exit = true; }

void State_Destroy(gg_state_t* state) {
    // NOTE: Don't need to kill the scene; we only have a pointer to a gg_scene_t managed by the assets
    Keys_Destroy(&state->keys);
#ifdef GG_EDITOR
    Editor_Destroy(&state->editor);
#endif
}
