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
	} fb;
	void set_background(int x, int y,uintptr_t addr);
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
			hal::map_phys_to_virt_cur(map_addrv,map_addrp, {false,false,true});
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
			if(module->mod_end-module->mod_start==3145728) {
				break;
			} else {
				module=NULL;
			}
		}
		if(!module) {
			return;
		}
		addr=module->mod_start;
		uintptr_t vaddr=map_addrv+(addr&0xFFF);
		map_addrp=addr&~(0xFFF);
		space_req=1024*768*4+(addr&0xFFF);
		while(space_req>0) {
			hal::map_phys_to_virt_cur(map_addrv,map_addrp, {false,false,true});
			map_addrv+=0x1000;
			map_addrp+=0x1000;
			space_req-=0x1000;
		}
		copy_rect(0,0,1024,768,(void *)vaddr);
		hal::print_boot_msg("Init VGA", true,true);
	}
	void put_pixel(int x, int y, uint32_t color) {
		uint32_t *addr=(uint32_t *)(fb.addr+fb.stride*y+fb.bpp*x);
		*addr=color;
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
		for(int i=0; i<h; i++) {
			memcpy((void *)addr,rc,fb.bpp*w);
			addr+=fb.stride;
			rc+=fb.bpp*w;
		}
	}

	void copy_rect(int src_x,int src_y,
	               int w,int h,
	               int dest_x, int dest_y) {
		uintptr_t src_addr=fb.addr+fb.stride*src_y+fb.bpp*src_x;
		uintptr_t dest_addr=fb.addr+fb.stride*dest_y+fb.bpp*dest_x;
		for(int i=0; i<h; i++) {
			memcpy((void *)dest_addr,(void *)src_addr,fb.bpp*w);
			src_addr+=fb.stride;
			dest_addr+=fb.stride;
		}
	}

	void bit_blit(int w,int h,
	              void *dest_buf,void *src_buf,
	              bit_blit_op op) {
		//XXX: may be slow if clang doesn't perform comparison/loop inversion
		pointer sb=(pointer)src_buf;
		pointer addr=(pointer)dest_buf;
		for(int y=0; y<h; y++) {
			for(int x=0; x<w; x++) {
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
		for(int y=0; y<h; y++) {
			for(int x=0; x<w; x++) {
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
		for(int i=0; i<h; i++) {
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
	uint32_t light[] = {0xDDDB97,0xDCDA9C,0xDBD9A0,0xDAD7A3,0xD9D6A7,0xD7D4AB,0xD6D3AF,0xD6D2B3};
	uint32_t dark[] =  {0xB9B95C,0xB7B65C,0xB4B35C,0xB2B15B,0xAFAE5B,0xADAB5B,0xAAA85B,0xA7A55B};

	uint32_t n(int x, int y) {
		return (x^0x123)*(x^0x421)-y+((y^0x307)*(y^0x715));
	}
	uint32_t noise(int x,int y) {
		return n(x,y)+(n(x-1,y-1)%0x630)+(n(x-3,y-4)%0x125);
	}

	void set_background(int x, int y,uintptr_t addr) {
		uint8_t select = noise(x,y)%7;
		switch(select) {
			case 0:
			case 1:
			case 2:
			case 3:
				*((uint32_t *)addr)=light[rand()%8];
				break;
			case 4:
			case 5:
				*((uint32_t *)addr)=dark[rand()%8];
				break;
			case 6:
				*((uint32_t *)addr)=0x7B4214;
				break;
		}
		//*((uint32_t *)addr)=(rand()%2?light[rand()%8]:dark[rand()%8]);
		//*((uint32_t *)addr)=(rand()%0xAF)<<16|(rand()%0x8F)<<8|(rand()%0x7F)<<0;
	}
}