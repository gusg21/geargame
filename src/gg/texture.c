#include "texture.h"

#include <stdint.h>
#include <stdio.h>

#include "utils.h"

void Texture_LoadFromName(gg_texture_t* texture, const char* name) {
    char formatted_path[256] = {0};  // TODO: fixed buffer size :3
    sprintf_s(formatted_path, 256, "assets/%s.png", name);

    Texture_LoadFromFile(texture, formatted_path);
}

void Texture_LoadFromFile(gg_texture_t* texture, const char* path) { texture->_handle = LoadTexture(path); }

uint32_t Texture_GetWidth(gg_texture_t* texture) { return texture->_handle.width; }

uint32_t Texture_GetHeight(gg_texture_t* texture) { return texture->_handle.height; }
