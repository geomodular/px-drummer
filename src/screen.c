#include <SDL3/SDL_assert.h>
#include <stdlib.h>

#include "screen.h"
#include "widget.h"

/* Constants determines when is the down_delayed function called
 * First call is performed after MOUSE_DOWN_THRESHOLD
 * Every next call after MOUSE_DOWN_CLICK_INTERVAL
 * */
static const Uint32 MOUSE_DOWN_THRESHOLD = 300;     // [ms]
static const Uint32 MOUSE_DOWN_CLICK_INTERVAL = 60; // [ms]
static const int MOUSE_DOWN_DISTANCE = 2;           // [px]
static int __mouse_down_last_x = 0;
static int __mouse_down_last_y = 0;
static int __mouse_down_distance = 0;
static bool __mouse_down_click = false;
static Uint32 __mouse_down_click_t = 0;

struct Item {
  void *widget;
  struct Item *next;
};

static struct Item *last_widget;

void screen_attach(void *widget) {
  struct Item *l = calloc(1, sizeof *l);

  SDL_assert(l != NULL);

  l->widget = widget;

  if (last_widget != NULL)
    l->next = last_widget;
  last_widget = l;
}

void screen_detach_all(void) {
  struct Item *actual = last_widget;

  while (actual != NULL) {
    struct Item *next = actual->next;
    free(actual);
    actual = next;
  }
}

void screen_draw(void) {
  struct Item *actual = last_widget;

  while (actual != NULL) {
    struct Widget *w = actual->widget;

    SDL_assert(w != NULL);

    w->draw(w);
    actual = actual->next;
  }
}

void screen_update(Uint32 dt) {
  struct Item *actual = last_widget;

  while (actual != NULL) {
    struct Widget *w = actual->widget;

    SDL_assert(w != NULL);

    if (w->mouse.down == true) {
      if (w->down_delayed != NULL && w->mouse.down_t >= MOUSE_DOWN_THRESHOLD) {
        __mouse_down_click = true;
        __mouse_down_click_t += dt;
        if (__mouse_down_click_t >= MOUSE_DOWN_CLICK_INTERVAL) {
          w->down_delayed(w);
          __mouse_down_click_t = 0;
        }
      }
      w->mouse.down_t += dt;
    }
    actual = actual->next;
  }
}

void screen_mouse_move(int x, int y) {
  struct Item *actual = last_widget;

  while (actual != NULL) {
    struct Widget *w = actual->widget;

    /* Check if mouse is in rect */
    if (!w->disabled) {
      if (SDL_PointInRect(&(SDL_Point){x, y}, &w->rect) == true) {
        w->mouse.over = true;
        w->mouse.x = x;
        w->mouse.y = y;

        if (w->mouse.down == true)
          if (w->drag != NULL) {
            __mouse_down_distance +=
                abs(x - __mouse_down_last_x) + abs(y - __mouse_down_last_y);
            if (__mouse_down_distance > MOUSE_DOWN_DISTANCE)
              __mouse_down_click = true;
            w->drag(w, __mouse_down_last_x, __mouse_down_last_y);
            __mouse_down_last_x = x;
            __mouse_down_last_y = y;
          }
      } else {
        w->mouse.over = false;
        w->mouse.down = false;
      }
    }

    actual = actual->next;
  }
}

void screen_mouse_down(int x, int y) {
  struct Item *actual = last_widget;

  while (actual != NULL) {
    struct Widget *w = actual->widget;

    /* Check if mouse is in rect */
    if (!w->disabled) {
      if (SDL_PointInRect(&(SDL_Point){x, y}, &w->rect) == true) {
        w->mouse.down = true;
        w->mouse.down_t = 0;
      } else {
        w->mouse.down = false;
      }
    }

    actual = actual->next;
  }

  __mouse_down_last_x = x;
  __mouse_down_last_y = y;
  __mouse_down_click = false;
  __mouse_down_click_t = 0;
  __mouse_down_distance = 0;
}

void screen_mouse_up(int x, int y) {
  struct Item *actual = last_widget;

  while (actual != NULL) {
    struct Widget *w = actual->widget;

    /* Check if mouse is in rect */
    if (!w->disabled) {
      if (SDL_PointInRect(&(SDL_Point){x, y}, &w->rect) == true) {
        if (w->mouse.down == true) {
          w->mouse.down = false;
          if (__mouse_down_click != true) {
            w->click(w);
          }
        }
      }
    }

    actual = actual->next;
  }
}
