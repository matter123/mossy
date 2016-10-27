/*
	Copyright 2016 Matthew Fosdick

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#include <ansi.h>
#include <cpuid.h>
#include <string.h>
#include <tty.h>
namespace tty {
#define SCROLL_BUFFER_ROWS 1000

uint16_t scroll_buffer[SCROLL_BUFFER_ROWS][80];
// uint16_t back_buffer[25][80];
static volatile uint16_t *vga_mem = reinterpret_cast<uint16_t *>(0xB8000);
/*
uint16_t back_buffer[100][80];

clear(clear_mode mode);
clear_line(clear_mode mode);
fill(clear_mode mode, char fill);
fill_line(clear_mode mode, char fill);

clear_mode
----------
CLEAR_ALL: 0
CLEAR_FWD: 1
CLEAR_BWD: 2
 */
void move_cur(int x, int y);
static uint8_t fix[]{
    0, // black
    4, // red
    2, // green
    6, // yellow
    1, // blue
    5, // magenta
    3, // cyan
    7, // white
};

uint16_t state_t::getattr() {
	uint8_t back = fix[background];
	uint8_t fore = fix[foreground];
	if(bold) fore += 8;
	if(inverse) {
		uint8_t tmp = fore;
		fore = back;
		back = tmp;
	}
	if(conceal) { fore = back; }
	return (back << 4 | fore) << 8;
}
void state_t::move_cur(int x, int y) {
	this->cur_x = (x == 0 ? this->cur_x : x);
	this->cur_y = (y == 0 ? this->cur_y : y);
	if(this->cursor_visable) { move_cur(this->cur_x, this->cur_y); }
}
void state_t::move_cur_relative(int x, int y) {
	this->cur_x += x;
	this->cur_y += y;
	if(this->cur_x < 1) this->cur_x = 1;
	if(this->cur_y < 1) this->cur_y = 1;
	if(this->cur_x > 80) this->cur_x = 80;
	if(this->cur_y > 25) this->cur_y = 25;
	if(this->cursor_visable) { move_cur(this->cur_x, this->cur_y); }
}
void state_t::show_cursor(bool show) {
	this->cursor_visable = show;
	if(show) {
		move_cur(this->cur_x, this->cur_y);
	} else {
		move_cur(81, 25);
	}
}
state_t state;
uint8_t default_forground = 0x7;
uint8_t default_background = 0x0;

void flush() {
	if(state.scroll_pos + 25 < SCROLL_BUFFER_ROWS)
		memcpy((void *)vga_mem, scroll_buffer[state.scroll_pos], 2 * 25 * 80);
	else {
		memcpy((void *)vga_mem, scroll_buffer[state.scroll_pos], 2 * (SCROLL_BUFFER_ROWS - state.scroll_pos) * 80);
	}
}

RUN_ONCE BEFORE_INIT void initalize_tty() {
	// TODO: seems to be broken, figure out why
	/*inb(0x3D4); // fix up 0x3C0
	outb(0x3C0, 0x10);
	uint8_t val = inb(0x3C1);
	inb(0x3D4);
	val &= ~8;
	outb(0x3C0, 0x10);
	outb(0x3C0, val);
	// move cursor offscreen to disable it
	move_cur(81, 25);*/
}
void move_cur(int x, int y) {
	uint16_t loc = (y - 1) * 80 + (x - 1);
	outb(0x3D4, 0x14);
	outb(0x3D5, loc >> 8);
	outb(0x3D4, 0x15);
	outb(0x3D5, loc & 0xF);
}
static inline void *get_offset_addr(int offset_y, int offset_x = 0) {
	return &scroll_buffer[(state.scroll_pos + offset_y) % SCROLL_BUFFER_ROWS][offset_x];
}
void clear_line(int line) {
	memset(get_offset_addr(line - 1), state.getattr() | 0x20, 80);
	if(state.cur_y == line) state.move_cur(1);
	flush();
}
void clear() {}
void putc(char c) {
	if(state.cur_x > 80 || c == '\n') {
		state.cur_x = 1;
		state.cur_y++;
		// clear buffer if y>25 and reset y
		if(state.cur_y == 26) {
			memset16(&scroll_buffer[(state.scroll_pos + state.cur_y) % SCROLL_BUFFER_ROWS][0], state.getattr() | 0x20,
			         80);
			state.scroll_pos++;
			state.cur_y = 25;
		}
	}
	if(c >= 0x1F) {
		uint16_t ch = state.getattr() | c;
		scroll_buffer[(state.scroll_pos + state.cur_y - 1) % SCROLL_BUFFER_ROWS][state.cur_x - 1] = ch;
		vga_mem[(state.cur_y - 1) * 80 + (state.cur_x)] = ch;
		state.cur_x++;
	}
	if(state.cursor_visable) move_cur(state.cur_x, state.cur_y);
}
void puts(char *s) {
	while(*s) {
		if(*s == '\x1b') {
			s = parse_escape(s);
		} else {
			putc(*s++);
		}
	}
}
}
