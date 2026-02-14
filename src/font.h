#pragma once

#include <SDL3/SDL_render.h>

typedef struct Font {
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int tile_width;
  int tile_height;
  int tile_offset;
  const char *alphabet;
} Font;

void text(const Font *font, int x, int y, const char *text);
