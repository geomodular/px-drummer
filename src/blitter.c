#include <stdarg.h>
#include "blitter.h"

void blit(const Blitter* blitter, int x, int y, int index)
{
	int w = blitter->tile_width;
	int h = blitter->tile_height;

	int tex_w, tex_h;
	SDL_QueryTexture(blitter->texture, NULL, NULL, &tex_w, &tex_h);

	int tiles_on_x = tex_w / w;
	int tile_x = index % tiles_on_x * w;
	int tile_y = index / tiles_on_x * h;

	SDL_RenderCopy(
		blitter->renderer,
		blitter->texture,
		&(SDL_Rect) {tile_x, tile_y, w, h}, // Position on texture
		&(SDL_Rect) {x, y, w, h}  // Position on surface
	);
}

void blit_ex(const Blitter* blitter, int x, int y, int count, ...)
{
	int w = blitter->tile_width;
	int h = blitter->tile_height;

	int tex_w, tex_h;
	SDL_QueryTexture(blitter->texture, NULL, NULL, &tex_w, &tex_h);

	int tiles_on_x = tex_w / w;

	va_list arg_list;
	va_start(arg_list, count);

	int index = va_arg(arg_list, int);
	int origin_tile_x = index % tiles_on_x * w;
	int origin_tile_y = index / tiles_on_x * h;

	SDL_RenderCopy(
		blitter->renderer,
		blitter->texture,
		&(SDL_Rect) {origin_tile_x, origin_tile_y, w, h},
		&(SDL_Rect) {x, y, w, h}
	);

	for (int i = 0; i < count - 1; i++) {

		index = va_arg(arg_list, int);
		int tile_x = index % tiles_on_x * w;
		int tile_y = index / tiles_on_x * h;

		SDL_RenderCopy(
			blitter->renderer,
			blitter->texture,
			&(SDL_Rect) {tile_x, tile_y, w, h},
			&(SDL_Rect) {
				x + (tile_x - origin_tile_x),
				y + (tile_y - origin_tile_y),
				w, h
			}
		);
	}

	va_end(arg_list);
}

