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
#ifndef STDLIB_STDIO_ANSI_H
#define STDLIB_STDIO_ANSI_H
#include <tty.h>
namespace stdlib {
class page {
	uint16_t page_data[25][80];
	struct attr_t {
		bool bold;
		bool inverse;
		bool conceal;
		uint8_t background = default_background;
		uint8_t foreground = default_forground;
		uint16_t getattr();
	} attributes;
	int cur_x = 1, cur_y = 1;
	int save_x, save_y;
	bool newline(bool force = false);
	page *_prev, *_next;

  public:
	page();
	page(page &p);

	bool bold();
	void bold(bool bold);
	bool inverse();
	void inverse(bool inverse);
	bool conceal();
	void conceal(bool conceal);
	const uint16_t *data();
	page &prev();
	page &next();
	void move_cur(int x = 0, int y = 0);
	void move_cur_relative(int x = 0, int y = 0);
	void save_cur();
	void load_cur();
	bool can_write();
	int write(const char c);
	int write(const char *s);
};
}
#endif
