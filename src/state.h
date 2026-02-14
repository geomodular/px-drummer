#pragma once

#include <SDL3/SDL_stdinc.h>

/* NOT CHANGEABLE! */
#define PATTERNS 16
#define CHANNELS 6
#define BLOCKS 16

typedef struct ProgramState {

  enum {
    IDLE,
    PLAYING,
    PAUSE // NOT USED
  } state;

  enum { M_34 = 12, M_44 = 16 } measure;

  int tempo;
  bool muted[CHANNELS];
  int blocks[PATTERNS][CHANNELS][BLOCKS];
  int patterns;
  long counter;
  Uint32 t_sum;
  bool locked;
} ProgramState;
