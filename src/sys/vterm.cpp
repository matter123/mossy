/*
    Copyright 2014 Matthew Fosdick

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
#include <hal/console.h>
#include <string.h>
#include <hal/hal.h>
#include <utf8.h>
#include <sys/fb.h>
#include <sys/text_render.h>
#include <io.h>
#define TEXT_HEIGHT 47
#define TEXT_WIDTH 113
namespace hal {
	static int wx=0,wy=0;
	int state_begin=0;
	static bool is_ready=false;
	uint32_t last_back=0;
	void ready() {
		is_ready=true;
		wx = 0;
		wy = 0;
		hal::print_boot_msg("Init vterm", true,true);
	}
	void scroll(int lines) {
		if(is_ready) {
			kernel::copy_rect(0,lines*16+2,kernel::get_w(),kernel::get_h()-(lines*16+2),0,2);
		}
		wy-=lines;
	}
	void printc(uint32_t back_color,uint32_t text_color,const char *glyph) {
		last_back=(back_color&(0xFF<<24))?0:back_color;
		uint32_t cp = unicode::utf8::decode_char(glyph);
		outb(0xE9,cp&0x7F);
		if(wx>=TEXT_WIDTH) {
			wx=0;
			wy++;
		}
		if(wy>=TEXT_HEIGHT) {
			scroll(wy-(TEXT_HEIGHT-1));
		}
		if(cp=='\n') {
			wx=0;
			wy++;
		} else if(cp=='\t') {
			wx=(wx+4)&(~3);
		} else {
			if(is_ready) {
				if(!(back_color&(0xFF<<24))) {
					kernel::fill_rect(wx*9+2,wy*16+2,9,16,back_color);
				}
				kernel::draw_char_at(wx*9+2,wy*16+2,cp,text_color&0xFFFFFF);
			}
			wx++;
		}
	}
	void cls() {
		if(is_ready) {
			kernel::reset_fb();
		}
		state_begin=0;
	}
	int width() {
		return TEXT_WIDTH;
	}
	int height() {
		return TEXT_WIDTH;
	}
	int get_x() {
		return wx;
	}
	int get_y() {
		return wy;
	}
	const char *endl="\n";
	ios_base address(16,2,2,sizeof(uintreg_t)*2);
}