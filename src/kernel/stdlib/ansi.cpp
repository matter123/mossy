#include <ansi.h>
#include <ctype.h>
#include <numconv.h>
#include <stdio/page.h>
#include <string.h>
#include <tty.h>
using namespace stdlib;
void csi_cur_up(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur_relative(0, num);
}
void csi_cur_down(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur_relative(0, -num);
}
void csi_cur_foward(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur_relative(num, 0);
}
void csi_cur_back(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur_relative(-num, 0);
}
void csi_cur_nextline(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur_relative(-80, num); // cheap trick to make it default to 1
}
void csi_cur_prevline(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur_relative(-80, -num);
}
void csi_cur_horizonatal(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	p->move_cur(num);
}
void csi_cur_pos(page *p, const char *data) {
	int x = std::strtonum(data, 1);
	while(isdigit(*data)) data++;
	if(*data == ';') data++;
	int y = std::strtonum(data, 1);
	p->move_cur(x, y);
}
void csi_clear(page *p, const char *data) {
	int mode = std::strtonum(data, 0);
	if(mode == 0) {
		// clear foward
	} else if(mode == 1) {
		// clear back
	} else if(mode == 2) {
		// clear all
		//	tty::clear();
	}
}
void csi_clearline(page *p, const char *data) {
	int mode = std::strtonum(data, 0);
	if(mode == 0) {
		// memset16(&scroll_buffer[(tty_state.scroll_pos + tty_state.cur_y) % SCROLL_BUFFER_ROWS][tty_state.cur_x],
		//         tty_state.getattr() | 0x20, 80 - tty_state.cur_x);
	} else if(mode == 1) {
		// memset16(&scroll_buffer[(tty_state.scroll_pos + tty_state.cur_y) % SCROLL_BUFFER_ROWS][0],
		//         tty_state.getattr() | 0x20, tty_state.cur_x);
	} else if(mode == 2) {
		//	tty::clear_line(tty::state.cur_y);
	}
}
void csi_scroll_up(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	// tty::state.scroll_pos -= num;
	// tty::state.move_cur_relative(0, num);
	// tty::flush();
}
void csi_scroll_down(page *p, const char *data) {
	int num = std::strtonum(data, 1);
	// tty::state.scroll_pos += num;
	// tty::state.move_cur_relative(0, -num);
	// tty::flush();
}
void csi_show_cur(page *p, const char *data) {
	// if(memcmp((void *)data, (void *)"?25", 3) == 0) { tty::state.show_cursor(true); }
}
void csi_hide_cur(page *p, const char *data) {
	// if(memcmp((void *)data, (void *)"?25", 3) == 0) { tty::state.show_cursor(false); }
}
void csi_sgr(page *p, const char *data) {
	const char *d = data;
	while(!isalpha(*data)) {
		int num = std::strtonum(data, 0);
		switch(num) {
		case 0:
			//	tty::state.bold = false;
			//	tty::state.inverse = false;
			//	tty::state.conceal = false;
			//	tty::state.foreground = tty::default_forground;
			//	tty::state.background = tty::default_background;
			break;
		case 1:
			//	tty::state.bold = true;
			break;
		case 7:
			//	tty::state.inverse = true;
			break;
		case 8:
			//	tty::state.conceal = true;
			break;
		case 22:
			//	tty::state.bold = false;
			break;
		case 27:
			//	tty::state.inverse = false;
			break;
		case 28:
			//	tty::state.conceal = false;
			break;
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
			//	tty::state.foreground = num - 30;
			break;
		case 39:
			//	tty::state.foreground = tty::default_forground;
			break;
		case 40:
		case 41:
		case 42:
		case 43:
		case 44:
		case 45:
		case 46:
		case 47:
			//	tty::state.background = num - 40;
			break;
		case 49:
			//	tty::state.background = tty::default_background;
			break;
		}
		while(isdigit(*data)) data++;
		if(*data == ';') data++;
	}
	if(data == d) {
		// immediadatly hit the end
		//	tty::state.bold = false;
		//	tty::state.inverse = false;
		//	tty::state.conceal = false;
		//	tty::state.foreground = tty::default_forground;
		//	tty::state.background = tty::default_background;
	}
}
void csi_save_cur(page *p, const char *data) {
	// tty::state.save_x = tty::state.cur_x;
	// tty::state.save_y = tty::state.cur_y;
}
void csi_restore_cur(page *p, const char *data) {
	// tty::state.cur_x = tty::state.save_x;
	// tty::state.cur_y = tty::state.save_y;
	// tty::state.move_cur();
}
void (*csi_handle[])(page *p, const char *){
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
    nullptr,             // U
    nullptr,             // V
    nullptr,             // W
    nullptr,             // X
    nullptr,             // Y
    nullptr,             // Z
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

const char *parse_escape(page *p, const char *data) {
	data++;
	if(*data == '[') return parse_csi(p, ++data);
	return ++data; // skip over the character
}
const char *parse_csi(page *p, const char *data) {
	const char *d = data;
	while(!isalpha(*d)) d++;
	// Log(LOG_DEBUG, "[TTY   ]", "%c", *d);
	if(csi_handle[(*d - '@')]) csi_handle[(*d - '@')](p, data);
	return ++d;
}
