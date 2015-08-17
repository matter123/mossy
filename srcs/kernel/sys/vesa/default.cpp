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
#include <hal/multiboot.h>
#include <vesa/screens.h>
#include <hal/console.h>
namespace kernel {
	screen_default::screen_default(hal::multiboot_fb *fbinfo,
	                               vesa_mode_info *mode_info,
	                               uintptr_t addr):screen((void *)addr, fbinfo->width,fbinfo->height){
		hal::print_boot_msg("WARN", "VESA","using default");
		this->fb_info=fbinfo;
		this->mi=mode_info;
		if(fb_info->bpp==8) {
			pointer v=(pointer)addr;
			uint8_t color=0xFF&((1<<mi->red_mask)-1)<<mi->red_pos;
			for(int i=0;i<fb_info->width*fb_info->height;i++) {
				v[i]=color; //fill with red hopefully
			}
		}else if(fb_info->bpp==16) {
			uint16_t *v=(uint16_t *)addr;
			uint16_t color=0xFF&((1<<mi->green_mask)-1)<<mi->green_pos;
			for(int i=0;i<fb_info->width*fb_info->height;i++) {
				v[i]=color; //fill with green hopefully
			}
		}else {
			pointer v=(pointer)addr;
			for(int i=0;i<fb_info->pitch*fb_info->height;i++) {
				v[i]=0xFF; //fill with white hopefully
			}
		}
	}
	void screen_default::flush() {
		//so not doing it now
	}
}
