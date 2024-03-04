#include "texture.h"

#include <stdint.h>
#include <stdio.h>

#include "utils.h"

void Texture_LoadFromFile(gg_texture_t* texture, const char* path) {
    texture->_handle = LoadTexture(path);
    texture->ok = true;
}

uint32_t Texture_GetWidth(gg_texture_t* texture) { return texture->_handle.width; }

uint32_t Texture_GetHeight(gg_texture_t* texture) { return texture->_handle.height; }

void Texture_Destroy(gg_texture_t* texture) {
    UnloadTexture(texture->_handle);
    texture->ok = false;
}
