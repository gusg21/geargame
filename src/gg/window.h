#if !defined(GG_WINDOW_H)
#define GG_WINDOW_H

#include <stdbool.h>
#include <stdint.h>

#include "camera.h"
#include "color.h"
#include "texture.h"

// TODO: Should this be split into a Window_* space and a Renderer_* space?
// Might make it more reasonable because most systems don't need to know
// about the window if they just want to put something on screen.

typedef struct gg_window {
    bool initialized;
    bool fullscreen;
    uint32_t width, height;
    RenderTexture2D _tex;
    // NOTE: you would probably want to include a handle to the window
    // in most other cases; in this case Raylib manages window state so
    // this might be a little silly looking in this context.
    // In fact most of the data in this structure could be stripped out, it's
    // only needed for backends that don't keep track of their own context.
    // (SDL, SFML, GLFW, etc.)
} gg_window_t;

// Window
void Window_CreateWindowed(gg_window_t* window, const char* title, uint32_t width, uint32_t height);
void Window_CreateFullscreen(gg_window_t* window, const char* title);
void Window_SetFullscreen(gg_window_t* window, bool fullscreen);
void Window_Destroy(gg_window_t* window);

// Getters
uint32_t Window_GetWidth(gg_window_t* window);
uint32_t Window_GetHeight(gg_window_t* window);
float Window_GetDeltaTime(gg_window_t* window);
bool Window_ShouldClose(gg_window_t* window);
bool Window_WasJustResized(gg_window_t* window);

// Rendering State
void Window_SetTarget(gg_window_t* window);
void Window_ReleaseTarget(gg_window_t* window);
void Window_BeginDrawing(gg_window_t* window);
void Window_BeginCameraDrawing(gg_window_t* window, gg_camera_t* camera);
void Window_BeginParallaxCameraDrawing(gg_window_t* window, gg_camera_t* camera, float parallax);
void Window_EndDrawing(gg_window_t* window);
void Window_EndCameraDrawing(gg_window_t* window, gg_camera_t* camera);

// Drawing
void Window_ClearScreen(gg_window_t* window, gg_color_t color);
void Window_DrawWindowTexture(gg_window_t* window);
void Window_DrawTexture(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y);
void Window_DrawTextureCentered(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y);
void Window_DrawTextureCenteredSR(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y, float scale,
                                  float rotation);
void Window_DrawSubTexture(gg_window_t* window, gg_texture_t* texture, int32_t x, int32_t y, uint32_t sx, uint32_t sy,
                           uint32_t sw, uint32_t sh);
void Window_DrawRectangle(const gg_window_t* window, int32_t x, int32_t y, uint32_t w, uint32_t h, gg_color_t color);
void Window_DrawDebugFPS(gg_window_t* window);
void Window_DrawSpaceOrigin(gg_window_t* window);

#endif  // GG_WINDOW_H
