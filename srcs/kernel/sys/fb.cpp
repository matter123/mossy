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
namespace kernel {
	buffer *front_buffer;
	void init_system_fb() {
		//find the multiboot framebuffer tag
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
			hal::print_boot_msg("Init VGA", false,true);
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

		front_buffer=buffer.create_buffer(fb_info->width, fb_info->height, fb_info->stride, XRGB, WRITE_ONLY);
		front_buffer->fill(color::yellow)
	}
}
