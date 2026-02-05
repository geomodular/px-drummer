#pragma once

#include <SDL2/SDL.h>

typedef struct Blitter
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int tile_width;
	int tile_height;
} Blitter;

void blit(const Blitter* blitter, int x, int y, int index);
void blit_ex(const Blitter* blitter, int x, int y, int count, ...);

