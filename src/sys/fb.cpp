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
#include <sys/fb.h>
#include <hal/multiboot.h>
#include <hal/console.h>
#include <hal/paging.h>
#include <hal/mmap.h>
#include <hal/hal.h>
#include <string.h>
namespace kernel {
	struct fb_info {
		uintptr_t addr;
		uint32_t width;
		uint32_t height;
		uint32_t bpp;
		uint32_t stride;
		bool safe_memcpy;
	}fb;
	void init_fb() {
		hal::multiboot_fb *fb_info=NULL;
		for(int i=0; i<hal::get_tag_count(); i++) {
			if(hal::get_tag(i)==NULL) {
				break;
			}
			hal::multiboot_tag *tag=hal::get_tag(i);
			if(tag->type!=8) {
				continue;
			}
			fb_info=reinterpret_cast<hal::multiboot_fb *>(tag);
		}
		if(fb_info==NULL) {
			hal::print_boot_msg("WARN", "CONSOLE","FB not found");
			return;
		}
		if(fb_info->addr>=UINT32_MAX) {
			hal::print_boot_msg("Init VGA", false,true);
		}
		if(fb_info->bpp!=32) {
			hal::print_boot_msg("WARN", "CONSOLE","FB bpp is not 32");
			hal::print_boot_msg("Init VGA", false,true);
		}
		uintptr_t addr=static_cast<uintptr_t>(fb_info->addr);
		hal::mem_region *region=NULL;
		for(int i=0; i<hal::get_virt_mem_regions(); i++) {
			hal::mem_region *reg=hal::get_virt_mem_region(i);
			if(reg->type.videobuffer) {
				region=reg;
				break;
			}
		}
		if(!region) {
			hal::print_boot_msg("Init VGA", false,true);
			return;
		}
		uintptr_t map_addrp=addr&~(0xFFF);
		uintptr_t map_addrv=(uintptr_t)region->start;
		uint32_t space_req=fb_info->pitch*fb_info->height+(addr&0xFFF);
		while(space_req>0) {
			hal::map_phys_to_virt_cur(map_addrv,map_addrp,{false,false,true});
			map_addrv+=0x1000;
			map_addrp+=0x1000;
			space_req-=0x1000;
		}
		fb.addr=(uintptr_t)(region->start+(addr&0xFFF));
		fb.stride=fb_info->pitch;
		fb.width=fb_info->width;
		fb.height=fb_info->height;
		fb.bpp=fb_info->bpp/8;
		fb.safe_memcpy=(fb.stride==(fb.width*(fb.bpp/8)));
		hal::cout<<hal::address<<fb.addr<<" "<<hal::dec<<fb.width
		         <<" "<<fb.height<<" "<<fb.bpp<<" "<<fb.safe_memcpy<<hal::endl;
	}

	void copy_rect(int x,int y,
	               int w,int h,
	               void *rect_buf) {
		if(x==0&&w==fb.width&&fb.safe_memcpy) {
			memcpy((void *)(fb.addr+fb.stride*y+fb.bpp*x),rect_buf,w*h*fb.bpp);
			return;
		}
		uintptr_t addr=fb.addr+fb.stride*y+fb.bpp*x;
		pointer rc=(pointer)rect_buf;
		for(int i=0;i<h;i++) {
			memcpy((void *)addr,rc,fb.bpp*w);
			addr+=fb.stride;
			rc+=fb.bpp*w;
		}
	}

	void bit_blit(int w,int h,
	              void *dest_buf,void *src_buf,
	              bit_blit_op op) {
		//XXX: may be slow if clang doesn't perform comparison/loop inversion
		pointer sb=(pointer)src_buf;
		pointer addr=(pointer)dest_buf;
		for(int y=0;y<h;y++) {
			for(int x=0;x<w;x++) {
				switch(op) {
					case AND:
						*((uint32_t *)addr)&=*((uint32_t *)sb);
						break;
					case OR:
						*((uint32_t *)addr)|=*((uint32_t *)sb);
						break;
					case XOR:
						*((uint32_t *)addr)^=*((uint32_t *)sb);
						break;
					case NOT:
						*((uint32_t *)addr)=~*((uint32_t *)sb);
						break;
				}
				addr+=fb.bpp;
				sb+=fb.bpp;
			}
		}
	}

	void bit_blit(int dest_x,int dest_y,
	              int w,int h,
	              void *src_buf,bit_blit_op op) {
		//XXX: may be slow if clang doesn't perform comparison/loop inversion
		pointer sb=(pointer)src_buf;
		uintptr_t addr=fb.addr+fb.stride*dest_y+fb.bpp*dest_x;
		for(int y=0;y<h;y++) {
			for(int x=0;x<w;x++) {
				switch(op) {
					case AND:
						*((uint32_t *)addr)&=*((uint32_t *)sb);
						break;
					case OR:
						*((uint32_t *)addr)|=*((uint32_t *)sb);
						break;
					case XOR:
						*((uint32_t *)addr)^=*((uint32_t *)sb);
						break;
					case NOT:
						*((uint32_t *)addr)=~*((uint32_t *)sb);
						break;
				}
				addr+=fb.bpp;
				sb+=fb.bpp;
			}
			addr+=fb.stride-(fb.bpp*w);
		}
	}

	uint32_t pack_rgb(uint8_t r,uint8_t g,uint8_t b) {
		return r<<16|g<<8|b;
	}

	void fill_rect(int x,int y,int w,int h, uint32_t packed_color) {
		if(x==0&&w==fb.width&&fb.safe_memcpy) {
			memset32((uint32_t *)(fb.addr+fb.stride*y),packed_color,w*h);
			return;
		}
		uintptr_t addr=fb.addr+fb.stride*y+fb.bpp*x;
		for(int i=0;i<h;i++) {
			memset32((uint32_t *)addr,packed_color,w);
			addr+=fb.stride;
		}
	}
	void fill_hline(int x,int y,int w,uint32_t packed_color) {
		uintptr_t addr=fb.addr+fb.stride*y+fb.bpp*x;
		memset32((uint32_t *)addr,packed_color,w);

	}
	void fill_vline(int x,int y,int h,uint32_t packed_color) {
		uintptr_t addr=fb.addr+fb.stride*y+fb.bpp*x;
		while(h--) {
			*((uint32_t *)addr)=packed_color;
			addr+=fb.stride;
		}
	}

	int get_w() {
		return fb.width;
	}
	int get_h() {
		return fb.height;
	}
	int get_bpp() {
		return fb.bpp;
	}
}