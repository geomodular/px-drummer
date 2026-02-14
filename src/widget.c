#include "widget.h"

static void phony_function(void *instance) {}

Widget widget_init(void) {
  return (Widget){.rect = {0, 0, 0, 0},
                  .mouse.over = false,
                  .mouse.down = false,
                  .mouse.down_t = 0,
                  .mouse.x = 0,
                  .mouse.y = 0,
                  .disabled = false,
                  .draw = phony_function,
                  .click = phony_function,
                  .down_delayed = NULL,
                  .drag = NULL};
}

void widget_set_down_delayed(void *widget, widget_down_delayed down_delayed) {
  Widget *w = widget;
  w->down_delayed = down_delayed;
}

void widget_set_draw(void *widget, widget_draw draw) {
  Widget *w = widget;
  w->draw = draw;
}

void widget_set_click(void *widget, widget_click click) {
  Widget *w = widget;
  w->click = click;
}

void widget_enable(void *widget) {
  Widget *w = widget;
  w->disabled = false;
}

void widget_disable(void *widget) {
  Widget *w = widget;
  w->disabled = true;
}
