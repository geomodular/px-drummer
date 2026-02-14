#include "blitter.h"
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_render.h>
#include <stdarg.h>

void blit(const Blitter *blitter, int x, int y, int index) {
  int w = blitter->tile_width;
  int h = blitter->tile_height;

  SDL_PropertiesID ps = SDL_GetTextureProperties(blitter->texture);
  long tex_w = SDL_GetNumberProperty(ps, SDL_PROP_TEXTURE_WIDTH_NUMBER, -1);

  int tiles_on_x = tex_w / w;
  int tile_x = index % tiles_on_x * w;
  int tile_y = index / tiles_on_x * h;

  SDL_RenderTexture(blitter->renderer, blitter->texture,
                    &(SDL_FRect){tile_x, tile_y, w, h}, // Position on texture
                    &(SDL_FRect){x, y, w, h}            // Position on surface
  );
}

void blit_ex(const Blitter *blitter, int x, int y, int count, ...) {
  int w = blitter->tile_width;
  int h = blitter->tile_height;

  SDL_PropertiesID ps = SDL_GetTextureProperties(blitter->texture);
  long tex_w = SDL_GetNumberProperty(ps, SDL_PROP_TEXTURE_WIDTH_NUMBER, -1);

  int tiles_on_x = tex_w / w;

  va_list arg_list;
  va_start(arg_list, count);

  int index = va_arg(arg_list, int);
  int origin_tile_x = index % tiles_on_x * w;
  int origin_tile_y = index / tiles_on_x * h;

  SDL_RenderTexture(blitter->renderer, blitter->texture,
                    &(SDL_FRect){origin_tile_x, origin_tile_y, w, h},
                    &(SDL_FRect){x, y, w, h});

  for (int i = 0; i < count - 1; i++) {

    index = va_arg(arg_list, int);
    int tile_x = index % tiles_on_x * w;
    int tile_y = index / tiles_on_x * h;

    SDL_RenderTexture(blitter->renderer, blitter->texture,
                      &(SDL_FRect){tile_x, tile_y, w, h},
                      &(SDL_FRect){x + (tile_x - origin_tile_x),
                                   y + (tile_y - origin_tile_y), w, h});
  }

  va_end(arg_list);
}
