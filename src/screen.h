#pragma once

#include <SDL2/SDL.h>

void screen_attach(void* widget);
void screen_detach_all();

void screen_draw();
void screen_update(Uint32 dt);
void screen_mouse_move(int x, int y);
void screen_mouse_down(int x, int y);
void screen_mouse_up(int x, int y);

