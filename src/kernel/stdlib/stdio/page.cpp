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
#include <stdio/page.h>
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
namespace stdlib {
uint16_t page::attr_t::getattr() {
	uint8_t back = fix[this->background];
	uint8_t fore = fix[this->foreground];
	if(bold) fore += 8;
	if(inverse) {
		uint8_t tmp = fore;
		fore = back;
		back = tmp;
	}
	if(conceal) { fore = back; }
	return (back << 4 | fore) << 8;
}
const uint16_t *page::data() { return static_cast<uint16_t *>(this->page_data[0]); }
bool page::can_write() { return this->cur_x > 80 || this->cur_y > 25; }
int page::write(char c) {
	if(can_write()) {
		this->page_data[cur_y - 1][cur_x - 1] = attributes.getattr() | c;
		cur_x++;
		newline();
		return 1;
	}
	return 0;
}
bool page::newline(bool force) {
	if(cur_x > 80 || force) {
		cur_y++;
		cur_x = 0;
		if(cur_y > 25) return true;
	}
	return false;
}
int page::write(const char *s) {
	char c;
	const char *orig = s;
	while((c = *s++)) {
		switch(c) {
		case '\n':
			if(cur_y > 25) goto ret_fail;
			if(newline(true)) goto ret_pass;
			break;
		case '\t':
			// sub to multiple of 4 then add 4
			cur_x = (cur_x & ~3) + 4;
			if(cur_x > 80) {
				if(newline()) goto ret_pass;
				// if still more to write add a wordwrap continue
				if(*s) { write(">> "); }
			}
			break;
		case '\f': // formfeed new page
			goto ret_pass;
		case '\x1b':
			s = parse_escape(this, s--);
			continue;
		default:
			if(can_write()) {
				this->page_data[cur_y - 1][cur_x - 1] = attributes.getattr() | c;
				cur_x++;
				newline();
				if(cur_x == 0 && *s) { write(">> "); }
			} else
				goto ret_fail;
		}
	}
ret_fail: // ret_fail is when c failed to be processed so backup s by one before returning the diference
	s--;
ret_pass:
	return s - orig;
}
}
