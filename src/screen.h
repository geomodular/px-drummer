#pragma once

#include <SDL3/SDL_stdinc.h>

void screen_attach(void *widget);
void screen_detach_all(void);

void screen_draw(void);
void screen_update(Uint32 dt);
void screen_mouse_move(int x, int y);
void screen_mouse_down(int x, int y);
void screen_mouse_up(int x, int y);
