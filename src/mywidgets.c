/* Some notes about struct initialization
	http://en.cppreference.com/w/c/language/struct_initialization
*/

#include "mywidgets.h"

static void click(void* instance)
{
	const Button* btn = instance;
	SDL_Event event;
	event.type = btn->event;
	SDL_PushEvent(&event);
}

static void draw_play(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.disabled == SDL_TRUE) {

		// Disabled
		blit_ex(btn->blitter, x, y, 4, 112, 113, 128, 129);
	} else {
		if (btn->widget.mouse.down == SDL_FALSE) {
			if (btn->widget.mouse.over == SDL_FALSE) {

				// Default
				blit_ex(btn->blitter, x, y, 4, 16, 17, 32, 33);
			} else {

				// Mouse on
				blit_ex(btn->blitter, x, y, 4, 48, 49, 64, 65);
			}
		} else {

			// Mouse down
			blit_ex(btn->blitter, x, y, 4, 80, 81, 96, 97);
		}
	}
}

Button btn_play_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {8, 16, 16, 16};
	widget.draw = draw_play;
	widget.click = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_stop(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.disabled == SDL_TRUE) {

		// Disabled
		blit_ex(btn->blitter, x, y, 4, 114, 115, 130, 131);
	} else {
		if (btn->widget.mouse.down == SDL_FALSE) {
			if (btn->widget.mouse.over == SDL_FALSE) {

				// Default
				blit_ex(btn->blitter, x, y, 4, 18, 19, 34, 35);
			} else {

				// Mouse on
				blit_ex(btn->blitter, x, y, 4, 50, 51, 66, 67);
			}
		} else {

			// Mouse down
			blit_ex(btn->blitter, x, y, 4, 82, 83, 98, 99);
		}
	}
}

Button btn_stop_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {24, 16, 16, 16};
	widget.draw = draw_stop;
	widget.click = click;
	widget.disabled = SDL_TRUE;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_left(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.mouse.down == SDL_FALSE) {
		if (btn->widget.mouse.over == SDL_FALSE) {

			// Default
			blit(btn->blitter, x, y, 73);
		} else {

			// Mouse on
			blit(btn->blitter, x, y, 89);
		}
	} else {

		// Mouse down
		blit(btn->blitter, x, y, 105);
	}
}

// Tempo LEFT
Button btn_tleft_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {136, 24, 8, 8};
	widget.draw = draw_left;
	widget.click = click;
	widget.down_delayed = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_right(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.mouse.down == SDL_FALSE) {
		if (btn->widget.mouse.over == SDL_FALSE) {

			// Default
			blit(btn->blitter, x, y, 74);
		} else {

			// Mouse on
			blit(btn->blitter, x, y, 90);
		}
	} else {

		// Mouse down
		blit(btn->blitter, x, y, 106);
	}
}

// Tempo RIGHT
Button btn_tright_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {144, 24, 8, 8};
	widget.draw = draw_right;
	widget.click = click;
	widget.down_delayed = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

// Measure LEFT
Button btn_mleft_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {136, 16, 8, 8};
	widget.draw = draw_left;
	widget.click = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

// Measure RIGHT
Button btn_mright_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {144, 16, 8, 8};
	widget.draw = draw_right;
	widget.click = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_plus(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.mouse.down == SDL_FALSE) {
		if (btn->widget.mouse.over == SDL_FALSE) {

			// Default
			blit(btn->blitter, x, y, 70);
		} else {

			// Mouse on
			blit(btn->blitter, x, y, 86);
		}
	} else {

		// Mouse down
		blit(btn->blitter, x, y, 102);
	}
}

Button btn_plus_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {8, 40, 8, 8};
	widget.draw = draw_plus;
	widget.click = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_minus(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.mouse.down == SDL_FALSE) {
		if (btn->widget.mouse.over == SDL_FALSE) {

			// Default
			blit(btn->blitter, x, y, 71);
		} else {

			// Mouse on
			blit(btn->blitter, x, y, 87);
		}
	} else {

		// Mouse down
		blit(btn->blitter, x, y, 103);
	}
}

Button btn_minus_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {8, 48, 8, 8};
	widget.draw = draw_minus;
	widget.click = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_clear(void* instance)
{
	Button* btn = instance;
	int x = btn->widget.rect.x;
	int y = btn->widget.rect.y;

	if (btn->widget.mouse.down == SDL_FALSE) {
		if (btn->widget.mouse.over == SDL_FALSE) {

			// Default
			blit_ex(btn->blitter, x, y, 2, 118, 119);
		} else {

			// Mouse on
			blit_ex(btn->blitter, x, y, 2, 120, 121);
		}
	} else {

		// Mouse down
		blit_ex(btn->blitter, x, y, 2, 122, 123);
	}
}

Button btn_clear_init(const struct Blitter* blitter, Uint32 event)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {48, 16, 16, 8};
	widget.draw = draw_clear;
	widget.click = click;

	return (Button) {
		.widget = widget,
		.blitter = blitter,
		.event = event
	};
}

static void draw_mute(void* instance)
{
	struct Switch* sw = instance;
	int x = sw->widget.rect.x;
	int y = sw->widget.rect.y;

	if (sw->widget.mouse.down == SDL_FALSE) {
		if (sw->widget.mouse.over == SDL_FALSE) {

			// Default
			if (*sw->value == SDL_TRUE) {
				blit(sw->blitter, x, y, 72);
			} else {
				blit(sw->blitter, x, y, 104);
			}
		} else {

			// Mouse on
			blit(sw->blitter, x, y, 88);
		}
	} else {

		// Mouse down
		if (*sw->value == SDL_TRUE) {
			blit(sw->blitter, x, y, 104);
		} else {
			blit(sw->blitter, x, y, 72);
		}
	}
}

static int __mute_index = 0;
struct Switch sw_mute_init(const struct Blitter* blitter, Uint32 event, const SDL_bool* value)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {8, 80 + (__mute_index++) * 8, 8, 8};
	widget.draw = draw_mute;
	widget.click = click;

	return (struct Switch) {
		.widget = widget,
		.blitter = blitter,
		.event = event,
		.value = value
	};
}

static void draw_lock(void* instance)
{
	struct Switch* sw = instance;
	int x = sw->widget.rect.x;
	int y = sw->widget.rect.y;

	if (sw->widget.mouse.down == SDL_FALSE) {
		if (sw->widget.mouse.over == SDL_FALSE) {

			// Default
			if (*sw->value == SDL_TRUE) {
				blit_ex(sw->blitter, x, y, 2, 134, 135);
			} else {
				blit_ex(sw->blitter, x, y, 2, 138, 139);
			}
		} else {

			// Mouse on
			blit_ex(sw->blitter, x, y, 2, 136, 137);
		}
	} else {

		// Mouse down
		if (*sw->value == SDL_TRUE) {
			blit_ex(sw->blitter, x, y, 2, 138, 139);
		} else {
			blit_ex(sw->blitter, x, y, 2, 134, 135);
		}
	}
}


struct Switch sw_lock_init(const struct Blitter* blitter, Uint32 event, const SDL_bool* value)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {48, 24, 16, 8};
	widget.draw = draw_lock;
	widget.click = click;

	return (struct Switch) {
		.widget = widget,
		.blitter = blitter,
		.event = event,
		.value = value
	};
}

static void draw_track_border(const struct Blitter* blitter, int x, int y)
{

	// Top
	blit(blitter, x, y, 29);
	blit(blitter, x + 8, y, 30);
	blit(blitter, x + 16, y, 30);
	blit(blitter, x + 24, y, 31);

	// Sides
	blit(blitter, x, y + 8, 45);
	blit(blitter, x + 24, y + 8, 47);
	blit(blitter, x, y + 16, 45);
	blit(blitter, x + 24, y + 16, 47);
	blit(blitter, x, y + 24, 45);
	blit(blitter, x + 24, y + 24, 47);
	blit(blitter, x, y + 32, 45);
	blit(blitter, x + 24, y + 32, 47);

	// Bottom
	blit(blitter, x, y + 40, 61);
	blit(blitter, x + 8, y + 40, 62);
	blit(blitter, x + 16, y + 40, 62);
	blit(blitter, x + 24, y + 40, 63);

	// Label
	blit(blitter, x + 2, y + 37, 57);
	blit(blitter, x + 10, y + 37, 58);
	blit(blitter, x + 18, y + 37, 59);
}


static void draw_track(void* instance)
{
	struct Track* track = instance;
	const struct ProgramState* state = track->pstate;

	SDL_Rect tmp_rect;
	SDL_RenderGetViewport(track->blitter->renderer, &tmp_rect);
	SDL_RenderSetViewport(track->blitter->renderer, &track->widget.rect);

	int position; // block position
	int offset; // subblock position

	// Check left border
	if (track->offset < 0) {
		position = 0;
		offset = track->offset;
		draw_track_border(track->blitter, -(offset + 32), 8);
	} else {
		position = track->offset / 8;
		offset = track->offset % 8;
	}

	int pattern_index = position / state->measure; // pattern to start with
	int block_index = position % state->measure; // block to start with

	for (int i = 0; i < BLOCKS + 1; i++) {

		// Actual x in px
		int x = i * 8 - offset;

		// Check if on right border, draw and break
		if (pattern_index == state->patterns) {
			draw_track_border(track->blitter, x, 8);
			break;
		}

		// Draw pattern number
		if (block_index == 0)
			blit(track->blitter, x, 0, pattern_index);

		// Draw arrow on bottom
		long _position = state->counter % (state->patterns * state->measure);
		if (_position == position + i)
			blit(track->blitter, x, 56, 56);

		// Draw blocks
		for (int j = 0; j < CHANNELS; j++) {
			int block_value = state->blocks[pattern_index][j][block_index];

			// Determine correct index value for blit fn
			int index;
			if (state->muted[j] == SDL_FALSE) {
				if (block_value == 0) {

					// Empty
					index = block_index % 4 == 0 ? 23 : 24;
				} else {

					// Filled
					index = 22;
				}
			} else {

				// Muted
				if (block_value == 0) {
					index = 38;
				} else {
					index = 39;
				}
			}
			blit(track->blitter, x, j * 8 + 8, index);
		}

		// Move to next block
		block_index++;
		block_index %= state->measure;

		// Move to next pattern
		if (block_index == 0) pattern_index++;
	}

	// Find block under mouse and redraw it
	if (track->widget.mouse.over == SDL_TRUE) {
		int mouse_x = track->widget.mouse.x - track->widget.rect.x + track->offset;
		int mouse_y = track->widget.mouse.y - track->widget.rect.y;
		int max_x = state->patterns* state->measure * 8;

		if (mouse_y >= 8 && mouse_y < 56 && mouse_x >= 0 && mouse_x < max_x) {
			int index = track->widget.mouse.down == SDL_TRUE ? 22 : 40;
			blit(track->blitter, (int) (mouse_x / 8) * 8 - track->offset, (int) (mouse_y / 8) * 8, index);
		}
	}

	SDL_RenderSetViewport(track->blitter->renderer, &tmp_rect);
}

static void click_track(void* instance)
{
	static struct TrackFeedback feedback;

	struct Track* track = instance;
	int mouse_x = track->widget.mouse.x - track->widget.rect.x + track->offset;
	int mouse_y = track->widget.mouse.y - track->widget.rect.y;
	int max_x = track->pstate->patterns* track->pstate->measure * 8;

	if (mouse_y >= 8 && mouse_y < 56 && mouse_x >= 0 && mouse_x < max_x) {
		int position = mouse_x / 8;
		int channel = (mouse_y / 8) - 1;

		feedback.position = position;
		feedback.channel = channel;

		SDL_Event event;
		event.type = track->event;
		event.user.data1 = &feedback;
		SDL_PushEvent(&event);
	}
}

static void drag_track(void* instance, int last_x, int last_y)
{
	struct Track* track = instance;
	
	if (track->pstate->locked == SDL_TRUE) return;
	
	track->offset += last_x - track->widget.mouse.x;

	// Left border
	if (track->offset < -32) // 32 is the size of track border
		track->offset = -32;

	// Right border
	int max_x = track->pstate->patterns * track->pstate->measure * 8 - 96;
	max_x = max_x < 0 ? 0 : max_x; // not really used in track but just in case
	if (track->offset > max_x)
		track->offset = max_x;
}

struct Track track_init(const struct Blitter* blitter, Uint32 event, const struct ProgramState* pstate)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {24, 72, 128, 64};
	widget.draw = draw_track;
	widget.click = click_track;
	widget.drag = drag_track;

	return (struct Track) {
		.widget = widget,
		.blitter = blitter,
		.event = event,
		.pstate = pstate,
		.offset = 0
	};
}

void track_set_offset(struct Track* track, int offset)
{
	// Left border
	if (offset < -32) // 32 is the size of track border
		offset = -32;

	// Right border
	int max_x = track->pstate->patterns * track->pstate->measure * 8 - 96;
	max_x = max_x < 0 ? 0 : max_x; // not really used in track but just in case
	if (offset > max_x)
		offset = max_x;

	track->offset = offset;
}

static void draw_pattern_border(const struct Blitter* blitter, int x, int y)
{

	// Top
	blit(blitter, x, y, 29);
	blit(blitter, x + 8, y, 30);
	blit(blitter, x + 16, y, 30);
	blit(blitter, x + 24, y, 31);

	// Bottom
	blit(blitter, x, y + 8, 61);
	blit(blitter, x + 8, y + 8, 62);
	blit(blitter, x + 16, y + 8, 62);
	blit(blitter, x + 24, y + 8, 63);

	// Label
	blit(blitter, x + 2, y + 5, 78);
	blit(blitter, x + 2 + 8, y + 5, 79);
}

static void draw_pattern(void* instance)
{
	struct Pattern* pat = instance;

	SDL_Rect tmp_rect;
	SDL_RenderGetViewport(pat->blitter->renderer, &tmp_rect);
	SDL_RenderSetViewport(pat->blitter->renderer, &pat->widget.rect);

	int position; // block position
	int offset; // subblock position

	// Check left border
	if (pat->offset < 0) {
		position = 0;
		offset = pat->offset;
		draw_pattern_border(pat->blitter, -(offset + 32), 0);
	} else {
		position = pat->offset / 16;
		offset = pat->offset % 16;
	}

	for (int i = 0; i < 16; i++) {

		int x = (i * 16) - offset;

		if (pat->pstate->patterns == i + position) {
			draw_pattern_border(pat->blitter, x, 0);
			break;
		}

		int pattern_index = (pat->pstate->counter % (pat->pstate->patterns * 16)) / 16;

		if (pattern_index == i + position) {

			// Like mouse over
			blit_ex(pat->blitter, x, 0, 4, 27, 28, 43, 44);
			blit(pat->blitter, x + 4, 4, i + position);
		} else {

			// Default
			blit_ex(pat->blitter, x, 0, 4, 25, 26, 41, 42);
			blit(pat->blitter, x + 4, 4, i + position + 144);
		}
	}

	if (pat->widget.mouse.over == SDL_TRUE) {
		int mouse_x = pat->widget.mouse.x - pat->widget.rect.x + pat->offset;
		int max_x = pat->pstate->patterns * 16;

		if (mouse_x >= 0 && mouse_x < max_x) {
			int x = (int) (mouse_x / 16) * 16 - pat->offset;
			if (pat->widget.mouse.down == SDL_TRUE) {

				// Mouse down
				blit_ex(pat->blitter, x, 0, 4, 238, 239, 254, 255); // must be empty space
				blit(pat->blitter, x + 4, 4, mouse_x / 16);
			} else {

				// Mouse over
				blit_ex(pat->blitter, x, 0, 4, 27, 28, 43, 44);
				blit(pat->blitter, x + 4, 4, mouse_x / 16);
			}
		}
	}

	SDL_RenderSetViewport(pat->blitter->renderer, &tmp_rect);
}

static void click_pattern(void* instance)
{
	static int position;

	struct Pattern* pat = instance;
	int mouse_x = pat->widget.mouse.x - pat->widget.rect.x + pat->offset;
	int max_x = pat->pstate->patterns * 16;

	if (mouse_x >=0 && mouse_x < max_x) {
		position = mouse_x / 16;

		SDL_Event event;
		event.type = pat->event;
		event.user.data1 = &position;
		SDL_PushEvent(&event);
	}
}

static void drag_pattern(void* instance, int last_x, int last_y)
{
	struct Pattern* pat = instance;
	pat->offset += last_x - pat->widget.mouse.x;

	// Left border
	if (pat->offset < -32) // 32 is the size of track border
		pat->offset = -32;

	// Right border
	int max_x = pat->pstate->patterns * 16 - 96;
	max_x = max_x < 0 ? 0 : max_x;
	if (pat->offset > max_x)
		pat->offset = max_x;
}

struct Pattern pattern_init(const struct Blitter* blitter, Uint32 event, const struct ProgramState* pstate)
{
	Widget widget = widget_init();
	widget.rect = (SDL_Rect) {24, 40, 128, 16};
	widget.draw = draw_pattern;
	widget.click = click_pattern;
	widget.drag = drag_pattern;

	return (struct Pattern) {
		.widget = widget,
		.blitter = blitter,
		.event = event,
		.pstate = pstate,
		.offset = 0
	};

}

