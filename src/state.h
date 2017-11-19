#pragma once

/* NOT CHANGEABLE! */
#define PATTERNS 16
#define CHANNELS 6
#define BLOCKS 16

struct ProgramState {

	enum {
		IDLE,
		PLAYING,
		PAUSE // NOT USED
	} state;

	enum {
		M_34 = 12,
		M_44 = 16
	} measure;

	int tempo;
	SDL_bool muted[CHANNELS];
	int blocks[PATTERNS][CHANNELS][BLOCKS];
	int patterns;
	long counter;
	Uint32 t_sum;
	SDL_bool locked;
};
