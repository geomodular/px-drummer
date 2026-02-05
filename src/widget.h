#pragma once

#include <SDL2/SDL.h>

typedef void (*widget_down_delayed)(void* instance);
typedef void (*widget_draw)(void* instance);
typedef void (*widget_click)(void* instance);
typedef void (*widget_drag)(void* instance, int last_x, int last_y);

typedef struct Widget {
	SDL_Rect rect;

	struct {
		SDL_bool over;
		SDL_bool down;
		Uint32 down_t;
		int x;
		int y;
	} mouse;

	SDL_bool disabled;

	/* Methods */
	widget_draw draw;
	widget_click click;
	widget_down_delayed down_delayed; // Could be NULL
	widget_drag drag; // Could be NULL
} Widget;

Widget widget_init(void);
void widget_set_down_delayed(void* widget, widget_down_delayed down_delayed);
void widget_set_draw(void* widget, widget_draw draw);
void widget_set_click(void* widget, widget_click click);
void widget_enable(void* widget);
void widget_disable(void* widget);

