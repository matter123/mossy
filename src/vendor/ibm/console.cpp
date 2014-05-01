/*
    Copyright 2013 Matthew Fosdick

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
#include <vendor.h>
#ifdef IBM
#include <hal/console.h>
#include <string.h>
#include <hal/hal.h>
namespace hal {
	uint16_t *mon=reinterpret_cast<uint16_t *>(get_page_offset_addr()+0xB8000);
	uint16_t x,y;
	void printc(ConsoleColor c,char let) {
		if(let=='\n') {
			x=0;
			y++;
			return;
		}
		if(let=='\t') {
			x=(x+4)&(~3);
			return;
		}
		mon[y*80+x++]=let|c.getColor()<<8;
		if(x>=80) {
			x=0;
			y++;
		}
		if(y>=25) {
			scroll(y-24);
			y=24;
		}
	}

	const char *endl="\n";

	void scroll(int lines) {
		if(y<lines) {
			cls();
			return;
		}
		for(int i=0; i<=(80-lines); i++) {
			std::memcpy(
			    reinterpret_cast<void *>(mon+160*(i-1)),
			    reinterpret_cast<void *>(mon+160*i),
			    160);
		}
		std::memset32(
		    reinterpret_cast<uint32_t *>(mon+160*(80-lines)),
		    0,
		    40*(80-lines));
		y-=lines;
	}

	void cls() {
		std::memset32(
		    reinterpret_cast<uint32_t *>(mon),
		    0,
		    1000); //2000 16 bit chars
		x=y=0;
	}

	int width() {
		return 80;
	}
	int height() {
		return 25;
	}
	int get_x() {
		return x;
	}
	int get_y() {
		return y;
	}
	ios_base address(16,2,2,sizeof(uintreg_t)*2);
}
#endif