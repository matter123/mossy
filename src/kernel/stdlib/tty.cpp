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
#include <cpuid.h>
#include <ctype.h>
#include <numconv.h>
#include <string.h>
#include <tty.h>
#define SCROLL_BUFFER_ROWS 1000

uint16_t scroll_buffer[SCROLL_BUFFER_ROWS][80];
static volatile uint16_t *vga_mem = reinterpret_cast<uint16_t *>(0xB8000);

uint8_t default_forground = 0x7;
uint8_t default_background = 0x0;
char *parse_csi(char *);
char *parse_escape(char *);

void move_cur(int x, int y);
void fix_scroll();
struct tty_state_t {
	bool bold;
	bool inverse;
	bool conceal;
	bool cursor_visable;
	int cur_x = 1, cur_y = 1;
	int save_x, save_y;
	int scroll_pos = 0;
	uint8_t background;
	uint8_t foreground;
	uint16_t getattr() {
		uint8_t back = background;
		uint8_t fore = foreground;
		if(bold) fore += 8;
		if(inverse) {
			uint8_t tmp = fore;
			fore = back;
			back = tmp;
		}
		if(conceal) { fore = back; }
		return (back << 4 | fore) << 8;
	}

} tty_state;

RUN_ONCE BEFORE_INIT void initalize_tty() {
	inb(0x3D4); // fix up 0x3C0
	outb(0x3C0, 0x10);
	uint8_t val = inb(0x3C1);
	inb(0x3D4);
	val &= ~8;
	outb(0x3C0, 0x10);
	outb(0x3C0, val);
	// move cursor offscreen to disable it
	move_cur(81, 25);
}
void csi_cur_up(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.cur_y -= num;
	if(tty_state.cur_y <= 0) tty_state.cur_y = 1;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_down(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.cur_y += num;
	if(tty_state.cur_y >= 25) tty_state.cur_y = 25;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_foward(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.cur_x -= num;
	if(tty_state.cur_x <= 0) tty_state.cur_x = 1;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_back(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.cur_x += num;
	if(tty_state.cur_x >= 81) tty_state.cur_x = 80;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_nextline(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.cur_y += num;
	tty_state.cur_x = 1;
	if(tty_state.cur_y >= 25) tty_state.cur_y = 25;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_prevline(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.cur_y -= num;
	tty_state.cur_x = 1;
	if(tty_state.cur_y <= 0) tty_state.cur_y = 1;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_horizonatal(char *data) {
	int num = std::strtonum(data, 1);
	if(num <= 0) num = 1;
	if(num >= 81) num = 80;
	tty_state.cur_x = num;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_cur_pos(char *data) {
	int x = std::strtonum(data, 1);
	while(isdigit(*data)) data++;
	if(*data == ';') data++;
	int y = std::strtonum(data, 1);
	if(x <= 0) x = 1;
	if(x >= 81) x = 80;
	if(y <= 0) y = 1;
	if(y >= 26) y = 25;
	tty_state.cur_x = x;
	tty_state.cur_y = y;
	if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
}
void csi_clear(char *data) {
	int mode = std::strtonum(data, 0);
	if(mode == 0) {
		// clear foward
	} else if(mode == 1) {
		// clear back
	} else if(mode == 2) {
		// clear all
		tty_state.cur_x = 1;
		tty_state.cur_y = 1;
		if(tty_state.cursor_visable) move_cur(tty_state.cur_x, tty_state.cur_y);
	}
}
void csi_clearline(char *data) {
	int mode = std::strtonum(data, 0);
	if(mode == 0) {
		memset16(&scroll_buffer[(tty_state.scroll_pos + tty_state.cur_y) % SCROLL_BUFFER_ROWS][tty_state.cur_x],
		         tty_state.getattr() | 0x20, 80 - tty_state.cur_x);
	} else if(mode == 1) {
		memset16(&scroll_buffer[(tty_state.scroll_pos + tty_state.cur_y) % SCROLL_BUFFER_ROWS][0],
		         tty_state.getattr() | 0x20, tty_state.cur_x);
	} else if(mode == 2) {
		memset16(&scroll_buffer[(tty_state.scroll_pos + tty_state.cur_y) % SCROLL_BUFFER_ROWS][0],
		         tty_state.getattr() | 0x20, 80);
	}
}
void csi_scroll_up(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.scroll_pos -= num;
	tty_state.cur_y += num;
	if(tty_state.cur_y <= 26) tty_state.cur_y = 25;
	fix_scroll();
}
void csi_scroll_down(char *data) {
	int num = std::strtonum(data, 1);
	tty_state.scroll_pos += num;
	tty_state.cur_y -= num;
	if(tty_state.cur_y <= 0) tty_state.cur_y = 1;
	// clear offending lines
	memset16(&scroll_buffer[(tty_state.scroll_pos + 25 - num) % SCROLL_BUFFER_ROWS][0], tty_state.getattr() | 0x20,
	         80 * num);
	fix_scroll();
}
void csi_show_cur(char *data) {
	if(memcmp((void *)data, (void *)"?25", 3) == 0) {
		tty_state.cursor_visable = true;
		move_cur(tty_state.cur_x, tty_state.cur_y);
	}
}
void csi_hide_cur(char *data) {
	if(memcmp((void *)data, (void *)"?25", 3) == 0) {
		tty_state.cursor_visable = false;
		move_cur(81, 25);
	}
}
void csi_sgr(char *data) {
	while(!isalpha(*data)) {
		int num = std::strtonum(data, 0);
		switch(num) {
		case 0:
			tty_state.bold = false;
			tty_state.inverse = false;
			tty_state.conceal = false;
			tty_state.foreground = default_forground;
			tty_state.background = default_background;
			break;
		case 1:
			tty_state.bold = true;
			break;
		case 7:
			tty_state.inverse = true;
			break;
		case 8:
			tty_state.conceal = true;
		case 22:
			tty_state.bold = false;
			break;
		case 27:
			tty_state.inverse = false;
			break;
		case 28:
			tty_state.conceal = false;
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
			tty_state.foreground = num - 30;
			break;
		case 39:
			tty_state.foreground = default_forground;
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
			tty_state.background = num - 40;
			break;
		case 49:
			tty_state.background = default_forground;
		}
		while(isdigit(*data) || *data == ';') data++;
	}
}
void csi_save_cur(char *data) {
	tty_state.save_x = tty_state.cur_x;
	tty_state.save_y = tty_state.cur_y;
}
void csi_restore_cur(char *data) {
	tty_state.cur_x = tty_state.save_x;
	tty_state.cur_y = tty_state.save_y;
}
void (*csi_handle[])(char *){
    nullptr,             // @
    csi_cur_up,          // A
    csi_cur_down,        // B
    csi_cur_foward,      // C
    csi_cur_back,        // D
    csi_cur_nextline,    // E
    csi_cur_prevline,    // F
    csi_cur_horizonatal, // G
    csi_cur_pos,         // H
    nullptr,             // I
    csi_clear,           // J
    csi_clearline,       // K
    nullptr,             // L
    nullptr,             // M
    nullptr,             // N
    nullptr,             // O
    nullptr,             // P
    nullptr,             // Q
    nullptr,             // R
    csi_scroll_up,       // S
    csi_scroll_down,     // T
    nullptr,             // W
    nullptr,             // X
    nullptr,             // Y
    nullptr,             // [
    nullptr,             // back slash
    nullptr,             // ]
    nullptr,             // ^
    nullptr,             // _
    nullptr,             // `
    nullptr,             // a
    nullptr,             // b
    nullptr,             // c
    nullptr,             // d
    nullptr,             // e
    nullptr,             // f
    nullptr,             // g
    csi_show_cur,        // h
    nullptr,             // i
    nullptr,             // j
    nullptr,             // k
    csi_hide_cur,        // l
    csi_sgr,             // m
    nullptr,             // n
    nullptr,             // o
    nullptr,             // p
    nullptr,             // q
    nullptr,             // r
    csi_save_cur,        // s
    nullptr,             // t
    csi_restore_cur,     // u
};
void move_cur(int x, int y) {
	uint16_t loc = (y - 1) * 80 + (x - 1);
	outb(0x3D4, 0x14);
	outb(0x3D5, loc >> 8);
	outb(0x3D4, 0x15);
	outb(0x3D5, loc & 0xF);
}

char *parse_escape(char *data) {
	data++;
	if(*data == '[') return parse_csi(++data);
	return ++data; // skip over the character
}
char *parse_csi(char *data) {
	char *d = data;
	while(!isalpha(*d)) d++;
	if(csi_handle[(*d - '@')]) csi_handle[(*d - '@')](data);
	return ++d;
}
void fix_scroll() {
	memcpy((void *)vga_mem, &scroll_buffer[(tty_state.scroll_pos) % SCROLL_BUFFER_ROWS][0], 80 * 25 * 2);
}
