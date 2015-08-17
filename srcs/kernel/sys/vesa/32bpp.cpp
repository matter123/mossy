/*
    Copyright 2015 Matthew Fosdick

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

#include <graphics/framebuffer.h>
#include <hal/multiboot.h>
#include <vesa/screens.h>
#include <hal/console.h>
namespace kernel {
	screen_32BPPXRGB::screen_32BPPXRGB(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 32BPPXRGB");
	}
	void screen_32BPPXRGB::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int y=0;y<this->height;y++) {
			for(int x=0;x<this->width;x++) {
				if(this->buf[y*this->width+x]!=this->otherbuf[y*this->width+x]) {
					v[y*this->width+x]=this->buf[y*this->width+x];
					this->otherbuf[y*this->width+x]=this->buf[y*this->width+x];
				}
			}
		}
	}
	screen_32BPPRGBX::screen_32BPPRGBX(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 32BPPRGBX");
	}
	void screen_32BPPRGBX::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int y=0;y<this->height;y++) {
			for(int x=0;x<this->width;x++) {
				if(this->buf[y*this->width+x]!=this->otherbuf[y*this->width+x]) {
					v[y*this->width+x]=(this->buf[y*this->width+x])<<8;
					this->otherbuf[y*this->width+x]=this->buf[y*this->width+x];
				}
			}
		}
	}
	screen_32BPPXBGR::screen_32BPPXBGR(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 32BPPXBGR");
	}
	void screen_32BPPXBGR::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int y=0;y<this->height;y++) {
			for(int x=0;x<this->width;x++) {
				if(this->buf[y*this->width+x]!=this->otherbuf[y*this->width+x]) {
					uint32_t color=this->buf[y*this->width+x];
					//XRGB to XBGR conversion
					color=(color&0xFF)<<16|(color&0xFF00)|((color>>16)&0xFF);
					v[y*this->width+x]=color;
					this->otherbuf[y*this->width+x]=this->buf[y*this->width+x];
				}
			}
		}
	}
	screen_32BPPBGRX::screen_32BPPBGRX(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 32BPPBGRX");
	}
	void screen_32BPPBGRX::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int y=0;y<this->height;y++) {
			for(int x=0;x<this->width;x++) {
				if(this->buf[y*this->width+x]!=this->otherbuf[y*this->width+x]) {
					uint32_t color=this->buf[y*this->width+x];
					//XRGB to XBGR conversion
					color=(color&0xFF)<<16|(color&0xFF00)|((color>>16)&0xFF);
					v[y*this->width+x]=color<<8;
					this->otherbuf[y*this->width+x]=this->buf[y*this->width+x];
				}
			}
		}
	}
}
