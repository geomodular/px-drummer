#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL3/SDL.h>

#include "blitter.h"
#include "db16.h"
#include "font.h"
#include "mywidgets.h"
#include "screen.h"
#include "state.h"

// clang-format off
static const Uint8 tiles_raw[] = {
#embed <tiles.png>
};

static const Uint8 kick_raw[] = {
#embed <kick.wav>
};

static const Uint8 snare_raw[] = {
#embed <snare.wav>
};


static const Uint8 tom1_raw[] = {
#embed <tom1.wav>
};

static const Uint8 tom2_raw[] = {
#embed <tom2.wav>
};

static const Uint8 hihat1_raw[] = {
#embed <hihat1.wav>
};

static const Uint8 hihat2_raw[] = {
#embed <hihat2.wav>
};
// clang-format on

static struct ProgramState pstate = {
    .state = IDLE,
    .measure = M_44,
    .tempo = 120,
    .muted = {false, false, false, false, false, false},
    .blocks = {{{0}}},
    .patterns = 1,
    .counter = 0,
    .t_sum = 0,
    .locked = true,
};

typedef struct Sound {
  Uint8 *wav_data;
  Uint32 wav_data_len;
  SDL_AudioStream *stream;
  SDL_AudioSpec spec;
} Sound;

static SDL_Texture *load_texture_io(SDL_Renderer *renderer, const Uint8 *data, size_t len);
static Sound load_wave_io(const Uint8 *data, size_t len);
static void screen_bg_draw(const struct Blitter *blitter);

int main(int argc, char **argv) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed initializing SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_AudioDeviceID audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (audio_device == 0) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed opening audio device: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  Sound kick = load_wave_io(kick_raw, sizeof(kick_raw));
  if (!SDL_BindAudioStream(audio_device, kick.stream)) {
    SDL_Log("failed binding audio device from `kick` sample: %s", SDL_GetError());
  }

  Sound snare = load_wave_io(snare_raw, sizeof(snare_raw));
  if (!SDL_BindAudioStream(audio_device, snare.stream)) {
    SDL_Log("failed binding audio device from `snare` sample: %s", SDL_GetError());
  }

  Sound tom1 = load_wave_io(tom1_raw, sizeof(tom1_raw));
  if (!SDL_BindAudioStream(audio_device, tom1.stream)) {
    SDL_Log("failed binding audio device from `tom1` sample: %s", SDL_GetError());
  }

  Sound tom2 = load_wave_io(tom2_raw, sizeof(tom2_raw));
  if (!SDL_BindAudioStream(audio_device, tom2.stream)) {
    SDL_Log("failed binding audio device from `tom2` sample: %s", SDL_GetError());
  }

  Sound hihat1 = load_wave_io(hihat1_raw, sizeof(hihat1_raw));
  if (!SDL_BindAudioStream(audio_device, hihat1.stream)) {
    SDL_Log("failed binding audio device from `hihat1` sample: %s", SDL_GetError());
  }

  Sound hihat2 = load_wave_io(hihat2_raw, sizeof(hihat2_raw));
  if (!SDL_BindAudioStream(audio_device, hihat2.stream)) {
    SDL_Log("failed binding audio device from `hihat2` sample: %s", SDL_GetError());
  }

  Sound *sounds[] = {&hihat2, &hihat1, &tom2, &tom1, &snare, &kick};
  const size_t sounds_len = 6;

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  if (!SDL_CreateWindowAndRenderer("PX drummer", 320, 288, 0, &window, &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed opening window and renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_assert(window != NULL);
  SDL_assert(renderer != NULL);

  SDL_SetRenderLogicalPresentation(renderer, 160, 144, SDL_LOGICAL_PRESENTATION_STRETCH);

  SDL_Texture *tiles = load_texture_io(renderer, tiles_raw, sizeof(tiles_raw));
  SDL_assert(tiles != NULL);

  Blitter blitter = {renderer, tiles, 8, 8};
  Font font = {renderer, tiles, 8, 8, 0, "0123456789ABCDEF"};

  Uint32 EVENT_BTN_PLAY = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_STOP = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_TLEFT = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_TRIGHT = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_MLEFT = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_MRIGHT = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_PLUS = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_MINUS = SDL_RegisterEvents(1);
  Uint32 EVENT_BTN_CLEAR = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_MUTE0 = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_MUTE1 = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_MUTE2 = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_MUTE3 = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_MUTE4 = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_MUTE5 = SDL_RegisterEvents(1);
  Uint32 EVENT_SW_LOCK = SDL_RegisterEvents(1);
  Uint32 EVENT_PATTERN = SDL_RegisterEvents(1);
  Uint32 EVENT_TRACK = SDL_RegisterEvents(1);

  Button btn_play = btn_play_init(&blitter, EVENT_BTN_PLAY);
  Button btn_stop = btn_stop_init(&blitter, EVENT_BTN_STOP);
  Button btn_tleft = btn_tleft_init(&blitter, EVENT_BTN_TLEFT);
  Button btn_tright = btn_tright_init(&blitter, EVENT_BTN_TRIGHT);
  Button btn_mleft = btn_mleft_init(&blitter, EVENT_BTN_MLEFT);
  Button btn_mright = btn_mright_init(&blitter, EVENT_BTN_MRIGHT);
  Button btn_plus = btn_plus_init(&blitter, EVENT_BTN_PLUS);
  Button btn_minus = btn_minus_init(&blitter, EVENT_BTN_MINUS);
  Button btn_clear = btn_clear_init(&blitter, EVENT_BTN_CLEAR);
  Switch sw_mute0 = sw_mute_init(&blitter, EVENT_SW_MUTE0, &pstate.muted[0]);
  Switch sw_mute1 = sw_mute_init(&blitter, EVENT_SW_MUTE1, &pstate.muted[1]);
  Switch sw_mute2 = sw_mute_init(&blitter, EVENT_SW_MUTE2, &pstate.muted[2]);
  Switch sw_mute3 = sw_mute_init(&blitter, EVENT_SW_MUTE3, &pstate.muted[3]);
  Switch sw_mute4 = sw_mute_init(&blitter, EVENT_SW_MUTE4, &pstate.muted[4]);
  Switch sw_mute5 = sw_mute_init(&blitter, EVENT_SW_MUTE5, &pstate.muted[5]);
  Switch sw_lock = sw_lock_init(&blitter, EVENT_SW_LOCK, &pstate.locked);
  Pattern pattern = pattern_init(&blitter, EVENT_PATTERN, &pstate);
  Track track = track_init(&blitter, EVENT_TRACK, &pstate);

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

  Uint64 t_last = SDL_GetTicks();
  bool running = true;
  while (running == true) {
    Uint64 t_actual = SDL_GetTicks();
    Uint64 dt = t_actual - t_last;
    t_last = t_actual;

    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {

      SDL_ConvertEventToRenderCoordinates(renderer, &e);

      switch (e.type) {
      case SDL_EVENT_QUIT:
        running = false;
        break;

      case SDL_EVENT_MOUSE_MOTION:
        screen_mouse_move(e.motion.x, e.motion.y);
        break;

      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        screen_mouse_down(e.button.x, e.button.y);
        break;

      case SDL_EVENT_MOUSE_BUTTON_UP:
        screen_mouse_up(e.button.x, e.button.y);
        break;

      case SDL_EVENT_KEY_DOWN:
        if (e.key.key == SDLK_SPACE) {
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
        break;
      }

      if (e.type == EVENT_BTN_PLAY) {
        pstate.state = PLAYING;
        widget_disable(&btn_play);
        widget_enable(&btn_stop);

      } else if (e.type == EVENT_BTN_STOP) {
        pstate.state = IDLE;
        widget_disable(&btn_stop);
        widget_enable(&btn_play);
        pstate.t_sum = 0;
        pstate.counter = 0;

        for (size_t i = 0; i < sounds_len; i++) {
          SDL_ClearAudioStream(sounds[i]->stream);
        }

      } else if (e.type == EVENT_BTN_TLEFT) {
        pstate.tempo--;
        if (pstate.tempo < 40)
          pstate.tempo = 40;

      } else if (e.type == EVENT_BTN_TRIGHT) {
        pstate.tempo++;
        if (pstate.tempo > 240)
          pstate.tempo = 240;

      } else if (e.type == EVENT_BTN_MLEFT) {
        if (pstate.measure == M_44) {
          pstate.measure = M_34;
        } else {
          pstate.measure = M_44;
        }

      } else if (e.type == EVENT_BTN_MRIGHT) {
        if (pstate.measure == M_44) {
          pstate.measure = M_34;
        } else {
          pstate.measure = M_44;
        }

      } else if (e.type == EVENT_BTN_CLEAR) {
        memset(pstate.blocks, 0, PATTERNS * CHANNELS * BLOCKS);
        pstate.patterns = 1;
        pstate.counter = 0;

      } else if (e.type == EVENT_SW_MUTE0) {
        pstate.muted[0] = !pstate.muted[0];

      } else if (e.type == EVENT_SW_MUTE1) {
        pstate.muted[1] = !pstate.muted[1];

      } else if (e.type == EVENT_SW_MUTE2) {
        pstate.muted[2] = !pstate.muted[2];

      } else if (e.type == EVENT_SW_MUTE3) {
        pstate.muted[3] = !pstate.muted[3];

      } else if (e.type == EVENT_SW_MUTE4) {
        pstate.muted[4] = !pstate.muted[4];

      } else if (e.type == EVENT_SW_MUTE5) {
        pstate.muted[5] = !pstate.muted[5];

      } else if (e.type == EVENT_SW_LOCK) {
        if (pstate.locked == false) {
          track_set_offset(&track, 0);
          pstate.locked = true;
        } else {
          pstate.locked = false;
        }

      } else if (e.type == EVENT_PATTERN) {
        int *position = e.user.data1;
        track_set_offset(&track, *(position)*pstate.measure * 8);

      } else if (e.type == EVENT_TRACK) {
        struct TrackFeedback *feedback = e.user.data1;
        int pattern_index = feedback->position / pstate.measure;
        int block_index = feedback->position % pstate.measure;

        if (pstate.blocks[pattern_index][feedback->channel][block_index] == 0) {
          size_t ch = feedback->channel;
          pstate.blocks[pattern_index][ch][block_index] = 1;
          SDL_ClearAudioStream(sounds[ch]->stream);
          SDL_PutAudioStreamData(sounds[ch]->stream, sounds[ch]->wav_data, sounds[ch]->wav_data_len);
        } else {
          pstate.blocks[pattern_index][feedback->channel][block_index] = 0;
        }

      } else if (e.type == EVENT_BTN_PLUS) {
        pstate.patterns++;
        if (pstate.patterns > 16)
          pstate.patterns = 16;

      } else if (e.type == EVENT_BTN_MINUS) {
        pstate.patterns--;
        if (pstate.patterns < 1)
          pstate.patterns = 1;
      }
    }

    SDL_RenderClear(renderer);

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

        for (size_t i = 0; i < 6; i++) {
          int value = pstate.blocks[pattern_index][i][block_index];
          if (value == 1 && pstate.muted[i] == false) {
            SDL_ClearAudioStream(sounds[i]->stream);
            SDL_PutAudioStreamData(sounds[i]->stream, sounds[i]->wav_data, sounds[i]->wav_data_len);
          }
        }

        if (pstate.locked == true && block_index == 0)
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

  SDL_DestroyAudioStream(kick.stream);
  SDL_free(kick.wav_data);

  SDL_CloseAudioDevice(audio_device);
  SDL_Quit();

  return EXIT_SUCCESS;
}

static SDL_Texture *load_texture_io(SDL_Renderer *renderer, const Uint8 *data, size_t len) {
  SDL_assert(renderer != NULL);
  SDL_assert(data != NULL);

  SDL_IOStream *stream = SDL_IOFromConstMem(data, len);
  SDL_assert(stream != NULL);

  SDL_Surface *surface = SDL_LoadPNG_IO(stream, true);
  if (surface == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_LoadPNG: %s\n", SDL_GetError());
    return NULL;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
  SDL_DestroySurface(surface);

  return texture;
}

static Sound load_wave_io(const Uint8 *data, size_t len) {
  SDL_assert(data != NULL);

  Sound sound;

  SDL_IOStream *stream = SDL_IOFromConstMem(data, len);
  SDL_assert(stream != NULL);

  if (!SDL_LoadWAV_IO(stream, true, &sound.spec, &sound.wav_data, &sound.wav_data_len)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed loading wave: %s\n", SDL_GetError());
    SDL_assert(true);
  }

  sound.stream = SDL_CreateAudioStream(&sound.spec, NULL);
  if (sound.stream == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed creating audio stream: %s\n", SDL_GetError());
    SDL_assert(true);
  }

  return sound;
}

static void screen_bg_draw(const struct Blitter *blitter) {
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
