#include <SDL2/SDL.h>
#include "widget.h"

static void phony_function(void* instance) {}

inline struct Widget widget_init()
{
	return (struct Widget) {
		.rect = {0, 0, 0, 0},
		.mouse.over = SDL_FALSE,
		.mouse.down = SDL_FALSE,
		.mouse.down_t = 0,
		.mouse.x = 0,
		.mouse.y = 0,
		.disabled = SDL_FALSE,
		.draw = phony_function,
		.click = phony_function,
		.down_delayed = NULL,
		.drag = NULL
	};
}

inline void widget_set_down_delayed(void* widget, widget_down_delayed down_delayed)
{
	struct Widget* w = widget;
	w->down_delayed = down_delayed;
}

inline void widget_set_draw(void* widget, widget_draw draw)
{
	struct Widget* w = widget;
	w->draw = draw;
}

inline void widget_set_click(void* widget, widget_click click)
{
	struct Widget* w = widget;
	w->click = click;
}

inline void widget_enable(void* widget)
{
	struct Widget* w = widget;
	w->disabled = SDL_FALSE;
}

inline void widget_disable(void* widget)
{
	struct Widget* w = widget;
	w->disabled = SDL_TRUE;
}
