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
	screen_16BPPRGB565::screen_16BPPRGB565(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 16BPPRGB565");
	}
	void screen_16BPPRGB565::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=2) {
			if(this->buf[i]!=this->otherbuf[i]||this->buf[i+1]!=this->otherbuf[i+1]) {
				uint32_t c1=this->buf[i];
				uint32_t c2=this->buf[i+1];
				//XRGB to RGB565
				uint16_t C1=(c1&0xF8)>>3|(c1&0xFC00)>>5|(c1&0xF80000)>>8;
				uint16_t C2=(c2&0xF8)>>3|(c2&0xFC00)>>5|(c2&0xF80000)>>8;
				v[i/2]=(C1<<16)|C2;
				this->otherbuf[i]=this->buf[i];
				this->otherbuf[i+1]=this->buf[i+1];
			}
		}
	}
	screen_16BPPBGR565::screen_16BPPBGR565(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 16BPPBGR565");
	}
	void screen_16BPPBGR565::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=2) {
			if(this->buf[i]!=this->otherbuf[i]||this->buf[i+1]!=this->otherbuf[i+1]) {
				uint32_t c1=this->buf[i];
				uint32_t c2=this->buf[i+1];
				//XRGB to BGR565
				uint16_t C1=(c1&0xF8)<<8|(c1&0xFC00)>>5|(c1&0xF80000)>>18;
				uint16_t C2=(c2&0xF8)<<8|(c2&0xFC00)>>5|(c2&0xF80000)>>18;
				v[i/2]=(C1<<16)|C2;
				this->otherbuf[i]=this->buf[i];
				this->otherbuf[i+1]=this->buf[i+1];
			}
		}
	}
	screen_16BPPXRGB1555::screen_16BPPXRGB1555(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 16BPPXRGB1555");
	}
	void screen_16BPPXRGB1555::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=2) {
			if(this->buf[i]!=this->otherbuf[i]||this->buf[i+1]!=this->otherbuf[i+1]) {
				uint32_t c1=this->buf[i];
				uint32_t c2=this->buf[i+1];
				//XRGB to XRGB1555
				uint16_t C1=(c1&0xF8)>>3|(c1&0xF800)>>6|(c1&0xF80000)>>9;
				uint16_t C2=(c2&0xF8)>>3|(c2&0xF800)>>6|(c2&0xF80000)>>9;
				v[i/2]=(C1<<16)|C2;
				this->otherbuf[i]=this->buf[i];
				this->otherbuf[i+1]=this->buf[i+1];
			}
		}
	}
	screen_16BPPRGBX5551::screen_16BPPRGBX5551(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 16BPPRGBX5551");
	}
	void screen_16BPPRGBX5551::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=2) {
			if(this->buf[i]!=this->otherbuf[i]||this->buf[i+1]!=this->otherbuf[i+1]) {
				uint32_t c1=this->buf[i];
				uint32_t c2=this->buf[i+1];
				//XRGB to RGBX5551
				uint16_t C1=((c1&0xF8)>>3|(c1&0xF800)>>6|(c1&0xF80000)>>9)<<1;
				uint16_t C2=((c2&0xF8)>>3|(c2&0xF800)>>6|(c2&0xF80000)>>9)<<1;
				v[i/2]=(C1<<16)|C2;
				this->otherbuf[i]=this->buf[i];
				this->otherbuf[i+1]=this->buf[i+1];
			}
		}
	}
	screen_16BPPXBGR1555::screen_16BPPXBGR1555(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 16BPPXBGR1555");
	}
	void screen_16BPPXBGR1555::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=2) {
			if(this->buf[i]!=this->otherbuf[i]||this->buf[i+1]!=this->otherbuf[i+1]) {
				uint32_t c1=this->buf[i];
				uint32_t c2=this->buf[i+1];
				//XRGB to XBGR1555
				uint16_t C1=(c1&0xF8)<<8|(c1&0xF800)>>6|(c1&0xF80000)>>19;
				uint16_t C2=(c2&0xF8)<<8|(c2&0xF800)>>6|(c2&0xF80000)>>19;
				v[i/2]=(C1<<16)|C2;
				this->otherbuf[i]=this->buf[i];
				this->otherbuf[i+1]=this->buf[i+1];
			}
		}
	}
	screen_16BPPBGRX5551::screen_16BPPBGRX5551(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 16BPPBGRX5551");
	}
	void screen_16BPPBGRX5551::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=2) {
			if(this->buf[i]!=this->otherbuf[i]||this->buf[i+1]!=this->otherbuf[i+1]) {
				uint32_t c1=this->buf[i];
				uint32_t c2=this->buf[i+1];
				//XRGB to BGRX5551
				uint16_t C1=((c1&0xF8)<<8|(c1&0xF800)>>6|(c1&0xF80000)>>19)<<1;
				uint16_t C2=((c2&0xF8)<<8|(c2&0xF800)>>6|(c2&0xF80000)>>19)<<1;
				v[i/2]=(C1<<16)|C2;
				this->otherbuf[i]=this->buf[i];
				this->otherbuf[i+1]=this->buf[i+1];
			}
		}
	}
}
