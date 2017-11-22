#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

#include "blitter.h"
#include "mywidgets.h"
#include "db16.h"
#include "font.h"
#include "screen.h"
#include "state.h"

static struct ProgramState pstate = {
	.state = IDLE,
	.measure = M_44,
	.tempo = 120,
	.muted = {SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE, SDL_FALSE},
	.blocks = {{{0}}},
	.patterns = 1,
	.counter = 0,
	.t_sum = 0,
	.locked = SDL_TRUE
};

static SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename);
static void screen_bg_draw(const struct Blitter* blitter);

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL could not initialize: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 256) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialize audio: %s\n", Mix_GetError());
		SDL_Quit();
		return EXIT_FAILURE;
	}

	Mix_Chunk* kick = Mix_LoadWAV("./assets/kick.wav");
	Mix_Chunk* snare = Mix_LoadWAV("./assets/snare.wav");
	Mix_Chunk* tom1 = Mix_LoadWAV("./assets/tom1.wav");
	Mix_Chunk* tom2 = Mix_LoadWAV("./assets/tom2.wav");
	Mix_Chunk* hihat1 = Mix_LoadWAV("./assets/hihat1.wav");
	Mix_Chunk* hihat2 = Mix_LoadWAV("./assets/hihat2.wav");

	assert(kick != NULL);
	assert(snare != NULL);
	assert(tom1 != NULL);
	assert(tom2 != NULL);
	assert(hihat1 != NULL);
	assert(hihat2 != NULL);

	SDL_Window* window = SDL_CreateWindow("PX drummer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 320, 288, SDL_WINDOW_SHOWN);
	SDL_assert(window != NULL);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE|SDL_RENDERER_SOFTWARE);
	SDL_assert(renderer != NULL);
	SDL_RenderSetLogicalSize(renderer, 160, 144);

	SDL_Texture* tiles = load_texture(renderer, "./assets/tiles.gif");
	SDL_assert(tiles != NULL);

	struct Blitter blitter = {renderer, tiles, 8, 8};
	struct Font font = {renderer, tiles, 8, 8, 0, "0123456789ABCDEF"};

	Uint32 BTN_PLAY_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_STOP_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_TLEFT_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_TRIGHT_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_MLEFT_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_MRIGHT_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_PLUS_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_MINUS_EVENT = SDL_RegisterEvents(1);
	Uint32 BTN_CLEAR_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_MUTE0_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_MUTE1_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_MUTE2_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_MUTE3_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_MUTE4_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_MUTE5_EVENT = SDL_RegisterEvents(1);
	Uint32 SW_LOCK_EVENT = SDL_RegisterEvents(1);
	Uint32 PATTERN_EVENT = SDL_RegisterEvents(1);
	Uint32 TRACK_EVENT = SDL_RegisterEvents(1);

	struct Button btn_play = btn_play_init(&blitter, BTN_PLAY_EVENT);
	struct Button btn_stop = btn_stop_init(&blitter, BTN_STOP_EVENT);
	struct Button btn_tleft = btn_tleft_init(&blitter, BTN_TLEFT_EVENT);
	struct Button btn_tright = btn_tright_init(&blitter, BTN_TRIGHT_EVENT);
	struct Button btn_mleft = btn_mleft_init(&blitter, BTN_MLEFT_EVENT);
	struct Button btn_mright = btn_mright_init(&blitter, BTN_MRIGHT_EVENT);
	struct Button btn_plus = btn_plus_init(&blitter, BTN_PLUS_EVENT);
	struct Button btn_minus = btn_minus_init(&blitter, BTN_MINUS_EVENT);
	struct Button btn_clear = btn_clear_init(&blitter, BTN_CLEAR_EVENT);
	struct Switch sw_mute0 = sw_mute_init(&blitter, SW_MUTE0_EVENT, &pstate.muted[0]);
	struct Switch sw_mute1 = sw_mute_init(&blitter, SW_MUTE1_EVENT, &pstate.muted[1]);
	struct Switch sw_mute2 = sw_mute_init(&blitter, SW_MUTE2_EVENT, &pstate.muted[2]);
	struct Switch sw_mute3 = sw_mute_init(&blitter, SW_MUTE3_EVENT, &pstate.muted[3]);
	struct Switch sw_mute4 = sw_mute_init(&blitter, SW_MUTE4_EVENT, &pstate.muted[4]);
	struct Switch sw_mute5 = sw_mute_init(&blitter, SW_MUTE5_EVENT, &pstate.muted[5]);
	struct Switch sw_lock = sw_lock_init(&blitter, SW_LOCK_EVENT, &pstate.locked);
	struct Pattern pattern = pattern_init(&blitter, PATTERN_EVENT, &pstate);
	struct Track track = track_init(&blitter, TRACK_EVENT, &pstate);

	screen_attach(&btn_play);
	screen_attach(&btn_stop);
	screen_attach(&btn_tleft);
	screen_attach(&btn_tright);
	screen_attach(&btn_mleft);
	screen_attach(&btn_mright);
	screen_attach(&btn_plus);
	screen_attach(&btn_minus);
	screen_attach(&btn_clear);
	screen_attach(&sw_mute0);
	screen_attach(&sw_mute1);
	screen_attach(&sw_mute2);
	screen_attach(&sw_mute3);
	screen_attach(&sw_mute4);
	screen_attach(&sw_mute5);
	screen_attach(&sw_lock);
	screen_attach(&pattern);
	screen_attach(&track);

	Uint32 t_last = SDL_GetTicks();
	SDL_bool running = SDL_TRUE;
	while (running == SDL_TRUE) {
		Uint32 t_actual = SDL_GetTicks();
		Uint32 dt = t_actual - t_last;
		t_last = t_actual;

		SDL_Event e;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				running = SDL_FALSE;
			} else if (e.type == SDL_MOUSEMOTION) {
				screen_mouse_move(e.motion.x, e.motion.y);
			} else if (e.type == SDL_MOUSEBUTTONDOWN) {
				screen_mouse_down(e.button.x, e.button.y);
			} else if (e.type == SDL_MOUSEBUTTONUP) {
				screen_mouse_up(e.button.x, e.button.y);
			} else if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_SPACE) {
					if (pstate.state == IDLE) {
						pstate.state = PLAYING;
						widget_disable(&btn_play);
						widget_enable(&btn_stop);
					} else if (pstate.state == PLAYING) {
						pstate.state = IDLE;
						widget_disable(&btn_stop);
						widget_enable(&btn_play);
						pstate.t_sum = 0;
						pstate.counter = 0;
					}
				}
			} else if (e.type == BTN_PLAY_EVENT) {
				pstate.state = PLAYING;
				widget_disable(&btn_play);
				widget_enable(&btn_stop);
			} else if (e.type == BTN_STOP_EVENT) {
				pstate.state = IDLE;
				widget_disable(&btn_stop);
				widget_enable(&btn_play);
				pstate.t_sum = 0;
				pstate.counter = 0;
			} else if (e.type == BTN_TLEFT_EVENT) {
				pstate.tempo--;
				if (pstate.tempo < 40) pstate.tempo = 40;
			} else if (e.type == BTN_TRIGHT_EVENT) {
				pstate.tempo++;
				if (pstate.tempo > 240) pstate.tempo = 240;
			} else if (e.type == BTN_MLEFT_EVENT) {
				if (pstate.measure == M_44) {
					pstate.measure = M_34;
				} else {
					pstate.measure = M_44;
				}
			} else if (e.type == BTN_MRIGHT_EVENT) {
				if (pstate.measure == M_44) {
					pstate.measure = M_34;
				} else {
					pstate.measure = M_44;
				}
			} else if (e.type == BTN_CLEAR_EVENT) {
				memset(pstate.blocks, 0, PATTERNS * CHANNELS * BLOCKS);
				pstate.patterns = 1;
				pstate.counter = 0;
			} else if (e.type == SW_MUTE0_EVENT) {
				pstate.muted[0] = pstate.muted[0] == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
			} else if (e.type == SW_MUTE1_EVENT) {
				pstate.muted[1] = pstate.muted[1] == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
			} else if (e.type == SW_MUTE2_EVENT) {
				pstate.muted[2] = pstate.muted[2] == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
			} else if (e.type == SW_MUTE3_EVENT) {
				pstate.muted[3] = pstate.muted[3] == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
			} else if (e.type == SW_MUTE4_EVENT) {
				pstate.muted[4] = pstate.muted[4] == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
			} else if (e.type == SW_MUTE5_EVENT) {
				pstate.muted[5] = pstate.muted[5] == SDL_TRUE ? SDL_FALSE : SDL_TRUE;
			} else if (e.type == SW_LOCK_EVENT) {
				if (pstate.locked == SDL_FALSE) {
					track_set_offset(&track, 0);
					pstate.locked = SDL_TRUE;
				} else {
					pstate.locked = SDL_FALSE;
				}
			} else if (e.type == PATTERN_EVENT) {
				int* position = e.user.data1;
				track_set_offset(&track, *(position) * pstate.measure * 8);
			} else if (e.type == TRACK_EVENT) {
				struct TrackFeedback* feedback = e.user.data1;
				int pattern_index = feedback->position / pstate.measure;
				int block_index = feedback->position % pstate.measure;

				if (pstate.blocks[pattern_index][feedback->channel][block_index] == 0) {
					pstate.blocks[pattern_index][feedback->channel][block_index] = 1;
					switch (feedback->channel) {
						case 0: Mix_PlayChannel(-1, hihat2, 0); break;
						case 1: Mix_PlayChannel(-1, hihat1, 0); break;
						case 2: Mix_PlayChannel(-1, tom2, 0); break;
						case 3: Mix_PlayChannel(-1, tom1, 0); break;
						case 4: Mix_PlayChannel(-1, snare, 0); break;
						case 5: Mix_PlayChannel(-1, kick, 0); break;
					}
				} else {
					pstate.blocks[pattern_index][feedback->channel][block_index] = 0;
				}

			} else if (e.type == BTN_PLUS_EVENT) {
				pstate.patterns++;
				if (pstate.patterns > 16) pstate.patterns = 16;
			} else if (e.type == BTN_MINUS_EVENT) {
				pstate.patterns--;
				if (pstate.patterns < 1) pstate.patterns = 1;
			}
		}

		screen_bg_draw(&blitter);
		screen_draw();

		{ // Secret scope :)
			char tempo_buffer[4];
			snprintf(tempo_buffer, 4, "%d", pstate.tempo);
			text(&font, 112, 24, tempo_buffer);
		}

		if (pstate.measure == M_44) {
			text(&font, 112, 16, "4 4");
		} else {
			text(&font, 112, 16, "3 4");
		}
		blit(&blitter, 120, 16, 54);

		SDL_RenderPresent(renderer);
		screen_update(dt);

		if (pstate.state == PLAYING) {
			const float tick = (1.0 / (pstate.tempo / 60.0) / 4.0) * 1000;
			pstate.t_sum += dt;
			if (pstate.t_sum >= tick) {
				long position = pstate.counter % (pstate.patterns * pstate.measure);
				int pattern_index = position / pstate.measure;
				int block_index = position % pstate.measure;
				int value;

				value = pstate.blocks[pattern_index][0][block_index];
				if (value == 1 && pstate.muted[0] == SDL_FALSE) Mix_PlayChannel(-1, hihat2, 0);
				value = pstate.blocks[pattern_index][1][block_index];
				if (value == 1 && pstate.muted[1] == SDL_FALSE) Mix_PlayChannel(-1, hihat1, 0);
				value = pstate.blocks[pattern_index][2][block_index];
				if (value == 1 && pstate.muted[2] == SDL_FALSE) Mix_PlayChannel(-1, tom2, 0);
				value = pstate.blocks[pattern_index][3][block_index];
				if (value == 1 && pstate.muted[3] == SDL_FALSE) Mix_PlayChannel(-1, tom1, 0);
				value = pstate.blocks[pattern_index][4][block_index];
				if (value == 1 && pstate.muted[4] == SDL_FALSE) Mix_PlayChannel(-1, snare, 0);
				value = pstate.blocks[pattern_index][5][block_index];
				if (value == 1 && pstate.muted[5] == SDL_FALSE) Mix_PlayChannel(-1, kick, 0);

				if (pstate.locked == SDL_TRUE && block_index == 0)
					track_set_offset(&track, pattern_index * pstate.measure * 8);

				pstate.t_sum -= tick;
				pstate.counter++;
			}
		}

		SDL_Delay(1);
	}

	screen_detach_all();
	SDL_DestroyTexture(tiles);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_FreeChunk(hihat2);
	Mix_FreeChunk(hihat1);
	Mix_FreeChunk(tom2);
	Mix_FreeChunk(tom1);
	Mix_FreeChunk(snare);
	Mix_FreeChunk(kick);
	Mix_CloseAudio();
	SDL_Quit();

	return EXIT_SUCCESS;
}

static SDL_Texture* load_texture(SDL_Renderer* renderer, const char* filename)
{
	SDL_assert(renderer != NULL);
	SDL_assert(filename != NULL);

	SDL_Surface* surface = IMG_Load(filename);
	if (surface == NULL) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG_Load: %s\n", IMG_GetError());
		return NULL;
	}

	/* This should be called once before creating texture but.. does it change anything? */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}

static void screen_bg_draw(const struct Blitter* blitter)
{
	SDL_SetRenderDrawColor(blitter->renderer, db16_pal[15].r, db16_pal[15].g, db16_pal[15].b, db16_pal[15].a);
	SDL_RenderClear(blitter->renderer);

	blit(blitter, 0, 0, 75);
	blit(blitter, 152, 0, 77);
	blit(blitter, 0, 136, 107);
	blit(blitter, 152, 136, 109);

	for (int i = 1; i < 19; i++) {
		blit(blitter, i * 8, 0, 76);
		blit(blitter, i * 8, 136, 108);
	}

	for (int i = 1; i < 17; i++) {
		blit(blitter, 0, i * 8, 91);
		blit(blitter, 152, i * 8, 93);
	}
}

