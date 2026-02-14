#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_stdinc.h>

typedef void (*widget_down_delayed)(void *instance);
typedef void (*widget_draw)(void *instance);
typedef void (*widget_click)(void *instance);
typedef void (*widget_drag)(void *instance, int last_x, int last_y);

typedef struct Widget {
  SDL_Rect rect;

  struct {
    bool over;
    bool down;
    Uint32 down_t;
    int x;
    int y;
  } mouse;

  bool disabled;

  /* Methods */
  widget_draw draw;
  widget_click click;
  widget_down_delayed down_delayed;
  widget_drag drag;
} Widget;

Widget widget_init(void);
void widget_set_down_delayed(void *widget, widget_down_delayed down_delayed);
void widget_set_draw(void *widget, widget_draw draw);
void widget_set_click(void *widget, widget_click click);
void widget_enable(void *widget);
void widget_disable(void *widget);
