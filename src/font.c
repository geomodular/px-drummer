#include "font.h"
#include <string.h>

void text(const Font *font, int x, int y, const char *text) {
  int w = font->tile_width;
  int h = font->tile_height;

  SDL_PropertiesID ps = SDL_GetTextureProperties(font->texture);
  long tex_w = SDL_GetNumberProperty(ps, SDL_PROP_TEXTURE_WIDTH_NUMBER, -1);

  int tiles_on_x = tex_w / w;

  int text_pos_x = x;
  int len = strlen(text);
  for (int i = 0; i < len; i++) {
    const char letter = text[i];

    int len_ = strlen(font->alphabet);
    int index;
    for (index = 0; index < len_; index++) {
      if (letter == font->alphabet[index]) {
        break;
      }
    }

    if (index == len_) {
      text_pos_x += w;
      continue;
    }

    int tile_x = (index + font->tile_offset) % tiles_on_x * w;
    int tile_y = (index + font->tile_offset) / tiles_on_x * h;

    SDL_RenderTexture(font->renderer, font->texture,
                      &(SDL_FRect){tile_x, tile_y, w, h}, // Position on texture
                      &(SDL_FRect){text_pos_x, y, w, h}   // Position on surface
    );

    text_pos_x += w;
  }
}
