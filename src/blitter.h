#pragma once

#include <SDL2/SDL.h>

struct Blitter
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int tile_width;
	int tile_height;
};

void blit(const struct Blitter* blitter, int x, int y, int index);
void blit_ex(const struct Blitter* blitter, int x, int y, int count, ...);

