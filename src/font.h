#pragma once

#include <SDL2/SDL.h>

struct Font
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	int tile_width;
	int tile_height;
	int tile_offset;
	const char* alphabet;
};

void text(const struct Font* font, int x, int y, const char* text);
