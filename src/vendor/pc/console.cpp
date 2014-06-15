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
#ifdef PC
#include <hal/console.h>
#include <string.h>
#include <hal/hal.h>
#include <utf8.h>
namespace hal {
	uint16_t *mon=reinterpret_cast<uint16_t *>(get_page_offset_addr()+0xB8000);
	uint16_t x,y;
	void printc(ConsoleColor c,unsigned char let) {
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
#define NON_CODE 19
	unsigned char latin_extend[] = {
		255,173,155,156,NON_CODE,157,NON_CODE,21, //A7
		NON_CODE,NON_CODE,166,174,170,45,NON_CODE,NON_CODE, //AF
		248,241,253,NON_CODE,NON_CODE,230,20,250, //B7
		NON_CODE,NON_CODE,167,175,172,171,NON_CODE,168, //BF
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,142,143,146,128, //C7
		NON_CODE,144,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE, //CF
		NON_CODE,165,NON_CODE,NON_CODE,NON_CODE,NON_CODE,153,NON_CODE, //D7
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,154,NON_CODE,NON_CODE,225, //DF
		133,160,131,NON_CODE,132,134,145,135, //E7
		138,130,136,137,141,161,140,139, //EF
		235,164,149,162,147,NON_CODE,148,246, //F7
		237,151,163,150,129,NON_CODE,NON_CODE,152, //FF
	};
	unsigned char greek[] = {
		NON_CODE,NON_CODE,NON_CODE,226,NON_CODE,NON_CODE,NON_CODE,NON_CODE,//397
		233,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,//39F
		227,NON_CODE,NON_CODE,228,NON_CODE,NON_CODE,232,NON_CODE, //3A7
		NON_CODE,234,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,//3AF
		NON_CODE,224,225,NON_CODE,235,238,NON_CODE,NON_CODE, //3B7
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,230,NON_CODE,NON_CODE,NON_CODE,//3BF
		227,NON_CODE,NON_CODE,229,231,NON_CODE,237,NON_CODE, //3C7
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,237,NON_CODE,NON_CODE,//3D7

	};
	unsigned char arrows[] = {
		27,24,26,25,18,NON_CODE,NON_CODE, //2197
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,
		NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,
		23,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,NON_CODE,//21AF
	};

	void printc(ConsoleColor c,const char *glyph) {
		uint32_t cp=get_code_point(glyph);
		if(cp<=0x7F) {
			return printc(c,cp);
		}
		if(cp>=0xA0&&cp<=0xFF) {
			return printc(c,latin_extend[cp-0xA0]);
		}
		if(cp==0x0192) {
			return printc(c,159);
		}
		if(cp>=0x0390&&cp<=0x03D7) {
			return printc(c,greek[cp-0x0390]);
		}
		if(cp==0x2022) {
			return printc(c,7);
		}
		if(cp==0x207F) {
			return printc(c,252);
		}
		if(cp==0x20A7) {
			return printc(c,158);
		}
		if(cp==0x2126) {
			return printc(c,234);
		}
		if(cp>=0x2190&&cp<=0x21AF) {
			return printc(c,arrows[cp-0x21AF]);
		}
		return printc(c,NON_CODE);
	}

	const char *endl="\n";

	void scroll(int lines) {
		scroll((uint)lines);
	}
	void scroll(uint lines) {
		if(y<lines) {
			cls();
			return;
		}
		for(int i=0; i<=(80-lines); i++) {
			memcpy(
			    reinterpret_cast<void *>(mon+160*(i-1)),
			    reinterpret_cast<void *>(mon+160*i),
			    160);
		}
		memset32(
		    reinterpret_cast<uint32_t *>(mon+160*(80-lines)),
		    0,
		    40*(80-lines));
		y-=lines;
	}

	void cls() {
		memset32(
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