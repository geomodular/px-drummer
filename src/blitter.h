#pragma once

#include <SDL3/SDL_render.h>

typedef struct Blitter {
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  int tile_width;
  int tile_height;
} Blitter;

void blit(const Blitter *blitter, int x, int y, int index);
void blit_ex(const Blitter *blitter, int x, int y, int count, ...);
