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

#include <sys/vesa.h>
#include <vesa/screens.h>
#include <hal/multiboot.h>
#include <hal/console.h>
#include <hal/paging.h>
#include <hal/mmap.h>
#include <hal/hal.h>
namespace kernel {
	graphics::screen *screen=NULL;
	void init_vesa() {
		//find the multiboot framebuffer tag
		hal::multiboot_fb *fb_info=NULL;
		hal::multiboot_vbe *vbe_info=NULL;
		for(int i=0; i<hal::get_tag_count(); i++) {
			if(hal::get_tag(i)==NULL) {
				break;
			}
			hal::multiboot_tag *tag=hal::get_tag(i);
			if(tag->type==7) {
				vbe_info=reinterpret_cast<hal::multiboot_vbe *>(tag);
			}
			if(tag->type==8) {
				fb_info=reinterpret_cast<hal::multiboot_fb *>(tag);
			}
		}
		if(fb_info==NULL||vbe_info==NULL) {
			hal::print_boot_msg("WARN", "VESA","no VESA framebuffer present");
			return;
		}
		//resolve later
		if(fb_info->addr>=UINT32_MAX) {
			hal::print_boot_msg("Init VESA", false,true);
		}
		uintptr_t addr=static_cast<uintptr_t>(fb_info->addr);
		//find vmem system fb region to map to
		hal::mem_region *region=NULL;
		for(int i=0; i<hal::get_virt_mem_regions(); i++) {
			hal::mem_region *reg=hal::get_virt_mem_region(i);
			if(reg->type.videobuffer) {
				region=reg;
				break;
			}
		}
		if(!region) {
			hal::print_boot_msg("Init VESA", false,true);
			return;
		}
		//perform a page mapping over the fb space
		uintptr_t map_addrp=addr&~(0xFFF);
		uintptr_t map_addrv=(uintptr_t)region->start;
		uint32_t space_req=fb_info->pitch*fb_info->height+(addr&0xFFF);
		while(space_req>0) {
			hal::map_phys_to_virt_cur(map_addrv,map_addrp, {false,false,true});
			map_addrv+=0x1000;
			map_addrp+=0x1000;
			space_req-=0x1000;
		}
		addr=(uintptr_t)(region->start+(addr&0xFFF));
		if(fb_info->type!=1) {
			hal::print_boot_msg("ERROR", "VESA","framebuffer is not RGB");
			hal::print_boot_msg("Init VESA", false,true);
			return;
		}
		if(fb_info->pitch!=(fb_info->width*(fb_info->bpp/8))) {
			hal::print_boot_msg("ERROR", "VESA","framebuffer has stride");
			hal::print_boot_msg("Init VESA", false,true);
		}
		vesa_mode_info *mode_info=reinterpret_cast<vesa_mode_info *>(vbe_info->vbe_mode_info);
		hal::cout<<hal::dec<<mode_info->red_mask<<" "<<mode_info->green_mask<<" "<<mode_info->blue_mask<<hal::endl;
		hal::cout<<hal::dec<<mode_info->red_pos<<" "<<mode_info->green_pos<<" "<<mode_info->blue_pos<<hal::endl;
		hal::magic_break();
		if(mode_info->red_mask==8&&mode_info->green_mask==8&&mode_info->blue_mask==8) {
			if(fb_info->bpp==32) {
				//XRGB, RGBX, XBGR, BGRX
				switch(mode_info->blue_pos) {
					case 0 : screen=new screen_32BPPXRGB(fb_info, addr);break;
					case 8 : screen=new screen_32BPPRGBX(fb_info, addr);break;
					case 16: screen=new screen_32BPPXBGR(fb_info, addr);break;
					case 24: screen=new screen_32BPPBGRX(fb_info, addr);break;
				}
			}else if(fb_info->bpp==24) {
				//RGB, BGR
				switch(mode_info->blue_pos) {
					case 0 : screen=new screen_24BPPRGB(fb_info, addr);break;
					case 16: screen=new screen_24BPPBGR(fb_info, addr);break;
				}
			}
		} else if(fb_info->bpp==16) {
			//RGB565, BGR565, XRGB1555, XBGR1555, RGBX5551, BGRX5551
			if(mode_info->green_mask==6) {
				//RGB565, BGR565
				switch(mode_info->blue_pos) {
					case 0 : screen=new screen_16BPPRGB565(fb_info, addr);break;
					case 16: screen=new screen_16BPPBGR565(fb_info, addr);break;
				}
			} else {
				//XRGB1555, XBGR1555, RGBX5551, BGRX5551
				switch(mode_info->blue_pos) {
					case 0 : screen=new screen_16BPPXRGB1555(fb_info, addr);break;
					case 1 : screen=new screen_16BPPRGBX5551(fb_info, addr);break;
					case 10: screen=new screen_16BPPXBGR1555(fb_info, addr);break;
					case 11: screen=new screen_16BPPBGRX5551(fb_info, addr);break;
				}
			}
		} else {
			screen=new screen_default(fb_info, mode_info, addr);
		}
		screen->fill(0xCCCCCC);
		screen->flush();
	}
}
