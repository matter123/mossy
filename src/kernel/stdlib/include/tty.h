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
#ifndef TTY_H
#define TTY_H
namespace tty {
void putc(char c);
void puts(char *s);
void clear_line(int line);
void clear();
void flush();
extern uint8_t default_forground, default_background;
struct state_t {
	bool bold;
	bool inverse;
	bool conceal;
	bool cursor_visable;
	int cur_x = 1, cur_y = 1;
	int save_x, save_y;
	unsigned int scroll_pos = 0;
	uint8_t background = default_background;
	uint8_t foreground = default_forground;
	uint16_t getattr();
	void move_cur(int x = 0, int y = 0);
	void move_cur_relative(int x = 0, int y = 0);
	void show_cursor(bool show = true);
};
extern state_t state;
}
#endif
