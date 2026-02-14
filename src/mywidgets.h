#pragma once

#include <SDL3/SDL_stdinc.h>

#include "blitter.h"
#include "state.h"
#include "widget.h"

typedef struct Button {
  Widget widget;
  const Blitter *blitter;
  Uint32 event;
} Button;

Button btn_play_init(const Blitter *blitter, Uint32 event);
Button btn_stop_init(const Blitter *blitter, Uint32 event);
Button btn_tleft_init(const Blitter *blitter, Uint32 event);
Button btn_tright_init(const Blitter *blitter, Uint32 event);
Button btn_mleft_init(const Blitter *blitter, Uint32 event);
Button btn_mright_init(const Blitter *blitter, Uint32 event);
Button btn_plus_init(const Blitter *blitter, Uint32 event);
Button btn_minus_init(const Blitter *blitter, Uint32 event);
Button btn_clear_init(const Blitter *blitter, Uint32 event);

typedef struct Switch {
  Widget widget;
  const Blitter *blitter;
  Uint32 event;
  const bool *value;
} Switch;

Switch sw_mute_init(const Blitter *blitter, Uint32 event, const bool *value);
Switch sw_lock_init(const Blitter *blitter, Uint32 event, const bool *value);

typedef struct Track {
  Widget widget;
  const Blitter *blitter;
  Uint32 event;
  const ProgramState *pstate;
  int offset;
} Track;

struct Track track_init(const Blitter *blitter, Uint32 event,
                        const ProgramState *pstate);
void track_set_offset(Track *track, int offset);

typedef struct TrackFeedback {
  int position;
  int channel;
} TrackFeedback;

typedef struct Pattern {
  Widget widget;
  const Blitter *blitter;
  Uint32 event;
  const ProgramState *pstate;
  int offset;
} Pattern;

Pattern pattern_init(const Blitter *blitter, Uint32 event,
                     const ProgramState *pstate);
