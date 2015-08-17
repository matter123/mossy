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
	screen_24BPPRGB::screen_24BPPRGB(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 24BPPRGB");
	}
	void screen_24BPPRGB::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=4) {
			if(this->buf[i+0]!=this->otherbuf[i+0]||
			   this->buf[i+1]!=this->otherbuf[i+1]||
			   this->buf[i+2]!=this->otherbuf[i+2]||
			   this->buf[i+3]!=this->otherbuf[i+3]) {
				uint32_t p0, p1, p2, p3, s0, s1, s2;
				p0=this->buf[i+0]&0xFFFFFF;
				p1=this->buf[i+1]&0xFFFFFF;
				p2=this->buf[i+2]&0xFFFFFF;
				p3=this->buf[i+3]&0xFFFFFF;
				s0=(p0<<8)|((p1>>16)&0xFF); //R8G8B8R8
				s1=(p1<<16)|((p2>>8)&0xFFFF); //G8B8R8G8;
				s2=(p2<<24)|p3; //B8R8G8B8
				v[(i/4)*3+0]=s0;
				v[(i/4)*3+1]=s1;
				v[(i/4)*3+2]=s2;
				this->otherbuf[i+0]=this->buf[i+0];
				this->otherbuf[i+1]=this->buf[i+1];
				this->otherbuf[i+2]=this->buf[i+2];
				this->otherbuf[i+3]=this->buf[i+3];
			}
		}
	}
	screen_24BPPBGR::screen_24BPPBGR(hal::multiboot_fb *fb_info, uintptr_t addr):screen((void *)addr, fb_info->width,fb_info->height){
		hal::print_boot_msg("INFO", "VESA","using 24BPPBGR");
	}
	void screen_24BPPBGR::flush() {
		uint32_t *v=reinterpret_cast<uint32_t *>(this->vram);
		for(int i=0;i<this->height*this->width;i+=4) {
			if(this->buf[i+0]!=this->otherbuf[i+0]||
			   this->buf[i+1]!=this->otherbuf[i+1]||
			   this->buf[i+2]!=this->otherbuf[i+2]||
			   this->buf[i+3]!=this->otherbuf[i+3]) {
				uint32_t p0, p1, p2, p3, s0, s1, s2;
				p0=this->buf[i+0]&0xFFFFFF;
				p1=this->buf[i+1]&0xFFFFFF;
				p2=this->buf[i+2]&0xFFFFFF;
				p3=this->buf[i+3]&0xFFFFFF;
				s0=(p0<<24)|((p0<<16)&0xFF00)|((p0>>16)&0xFF)|(p1<<24); //B8G8R8B8
				s1=((p1<<16)&0xFF00)|((p1>>16)&0xFF)|(p2<<24)|((p2<<16)&0xFF00); //G8R8B8G8
				s2=((p2>>16)&0xFF)|(p3<<24)|((p3<<16)&0xFF00)|((p3>>16)&0xFF); //R8B8G8R8
				v[(i/4)*3+0]=s0;
				v[(i/4)*3+1]=s1;
				v[(i/4)*3+2]=s2;
				this->otherbuf[i+0]=this->buf[i+0];
				this->otherbuf[i+1]=this->buf[i+1];
				this->otherbuf[i+2]=this->buf[i+2];
				this->otherbuf[i+3]=this->buf[i+3];
			}
		}
	}
}
