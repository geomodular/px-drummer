#pragma once

#include <SDL2/SDL.h>
#include "blitter.h"
#include "widget.h"
#include "state.h"

struct Button
{
	struct Widget widget;
	const struct Blitter* blitter;
	Uint32 event;
};

struct Button btn_play_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_stop_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_tleft_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_tright_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_mleft_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_mright_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_plus_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_minus_init(const struct Blitter* blitter, Uint32 event);
struct Button btn_clear_init(const struct Blitter* blitter, Uint32 event);

struct Switch
{
	struct Widget widget;
	const struct Blitter* blitter;
	Uint32 event;
	const SDL_bool* value;
};

struct Switch sw_mute_init(const struct Blitter* blitter, Uint32 event, const SDL_bool* value);
struct Switch sw_lock_init(const struct Blitter* blitter, Uint32 event, const SDL_bool* value);

struct Track
{
	struct Widget widget;
	const struct Blitter* blitter;
	Uint32 event;
	const struct ProgramState* pstate;
	int offset;
};

struct Track track_init(const struct Blitter* blitter, Uint32 event, const struct ProgramState* pstate);
void track_set_offset(struct Track* track, int offset);

struct TrackFeedback
{
	int position;
	int channel;
};

struct Pattern
{
	struct Widget widget;
	const struct Blitter* blitter;
	Uint32 event;
	const struct ProgramState* pstate;
	int offset;
};

struct Pattern pattern_init(const struct Blitter* blitter, Uint32 event, const struct ProgramState* pstate);
