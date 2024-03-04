#if !defined(GG_TEXTURE_H)
#define GG_TEXTURE_H

#include <raylib.h>  // Platform specific
#include <stdint.h>
#include <stdbool.h>

typedef struct gg_texture {
    Texture2D _handle;  // Platform-specific
    bool ok;
} gg_texture_t;

void Texture_LoadFromFile(gg_texture_t* texture, const char* path);
uint32_t Texture_GetWidth(gg_texture_t* texture);
uint32_t Texture_GetHeight(gg_texture_t* texture);
void Texture_Destroy(gg_texture_t* texture);

#endif  // GG_TEXTURE_H
