#include "gg/window.h"
#include "gg/assets.h"
#include "gg/state.h"

#include "gg/memory.h"

int main() {
    // Create the window
    gg_window_t window = {0};
    Window_CreateWindowed(&window, "Gear Game", 1600, 900);

    // Load the assets
    gg_assets_t assets = {0};
    Assets_Create(&assets);

    // Initialize the game
    gg_state_t state = {0};
    State_Init(&state);

    // Load the assets
    Assets_LoadInternals(&assets, &window, &state);
    Assets_Load(&assets, &window, &state, ASSET_SCRIPT, "sc_camera");
    Assets_Load(&assets, &window, &state, ASSET_SCRIPT, "sc_diamond");
    Assets_Load(&assets, &window, &state, ASSET_TEXTURE, "sunset");
    Assets_Load(&assets, &window, &state, ASSET_TILED_MAP, "area1");
    Assets_Load(&assets, &window, &state, ASSET_SCENE, "scene1");
    Assets_Load(&assets, &window, &state, ASSET_ACTOR_SPEC, "as_diamond");

    // Load up the scene
    gg_asset_t* scene_asset;
    bool valid = Assets_Get(&assets, &scene_asset, "scene1");
    if (valid) {
        State_SetCurrentScene(&state, &scene_asset->data.as_scene);
    }

    // Loop
    State_DoLoop(&state, &assets, &window);

    // Cleanup
    State_Destroy(&state);
    Assets_Destroy(&assets);
    Window_Destroy(&window);

    // Show any un-cleaned-up allocations
    Memory_PrintAllocations();

    return 0;
}