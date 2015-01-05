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
#include <sys/text_render.h>
#include <sys/fb.h>
#include <hal/multiboot.h>
#include <struct.h>
#include <utf8.h>
#include <hal/console.h>
namespace kernel {
	struct glyph_int {
		uint32_t codepoint;
		uint8_t row[16];
		uint8_t col[2];
		uint16_t pad;
	} PACKED *glyphs;
	struct mbf_head {
		uint32_t magic;
		uint32_t num_glyphs;
		uint32_t resv;
		uint32_t min_cp;
		uint32_t max_cp;
		uint32_t def_cp_idx;
	} *head;
	void init_tr() {
		hal::multiboot_module *module=NULL;
		for(int i=0; i<hal::get_tag_count(); i++) {
			if(hal::get_tag(i)==NULL) {
				break;
			}
			hal::multiboot_tag *tag=hal::get_tag(i);
			if(tag->type!=3) {
				continue;
			}
			module=reinterpret_cast<hal::multiboot_module *>(tag);
			hal::cout<<module->mod_start;
			if(*((uint32_t *)module->mod_start)==0x12345678) {
				break;
			} else {
				module=NULL;
			}
		}
		if(!module) {
			return;
		}
		head=(mbf_head *)(module->mod_start);
		glyphs=(glyph_int *)(module->mod_start+sizeof(mbf_head));
		hal::print_boot_msg("Init Text Render", true,true);
	}
	glyph_int *get_glyph(uint32_t codepoint) {
		if(codepoint<head->min_cp||codepoint>head->max_cp) {
			return glyphs+head->def_cp_idx;
		}
		for(int i=0; i<head->num_glyphs; i++) {
			if((glyphs+i)->codepoint==codepoint) {
				return glyphs+i;
			} else if((glyphs+i)->codepoint>codepoint) {
				return glyphs+head->def_cp_idx;
			}
		}
		return glyphs+head->def_cp_idx;
	}
	void draw_char_at(int x,int y,uint32_t codepoint,uint32_t color) {
		glyph_int *glyph=get_glyph(codepoint);
		uint32_t buf[9*32];
		for(int _y=0; _y<16; _y++) {
			uint8_t row=glyph->row[_y];
			for(int _x=0; _x<8; _x++) {
				buf[_y*9+_x]=(row&128?color&0xFFFFFF:0);
				buf[(_y+16)*9+_x]=(row&128?0:0xFFFFFF);
				row=row<<1;
			}
		}
		uint8_t colp1=glyph->col[0];
		uint8_t colp2=glyph->col[1];
		for(int _y=0; _y<8; _y++) {
			buf[_y*9+8]=(colp1&128?color&0xFFFFFF:0);
			buf[(_y+8)*9+8]=(colp2&128?color&0xFFFFFF:0);
			buf[(_y+16)*9+8]=(colp1&128?0:0xFFFFFF);
			buf[(_y+24)*9+8]=(colp2&128?0:0xFFFFFF);
			colp1=colp1<<1;
			colp2=colp2<<1;
		}
		bit_blit(x,y,9,16,buf+(9*16),AND);
		bit_blit(x,y,9,16,buf,OR);
	}
	void draw_string_at(int x,int y,const char *s,uint32_t color) {
		if(s==NULL) {
			return draw_string_at(x,y,"--NULL POINTER--",0xFFFFFF);
		}
		while(*s) {
			draw_char_at(x,y,unicode::utf8::decode_char(s),color);
			x+=9;
			s+=unicode::utf8::get_char_len(s);
		}
	}
}
