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
#include <hal/mmap.h>
#include <hal/paging.h>
#include <stddef.h>
#include <stdlib.h>
namespace acpi {
	namespace os {
#define IS_SET(a,i)    (a[(i)/8]&(1<<(i)%8))
#define SET(a,i)    do {a[(i)/8]|= (1<<((i)%8));}while(0)
#define UN_SET(a,i) do {a[(i)/8]&=~(1<<((i)%8));}while(0)
		uint8_t bitmap[128];
		uintptr_t start_loc;

		bool init_acpi_os() {
			hal::mem_region *region=NULL;
			for(int i=0; i<hal::get_virt_mem_regions(); i++) {
				hal::mem_region *reg=hal::get_virt_mem_region(i);
				if(reg->type.firmware) {
					region=reg;
					break;
				}
			}
			if(!region) {
				return false;
			}
			start_loc=(uintptr_t)region->start;
			//assume 4MiB firmware area
			return true;
		}
		uintptr_t get_virt_phys(uintptr_t phys, uintptr_t len, uintptr_t *alloc_len) {
			uintptr_t pa_phys=phys&~(0xFFF);
			uintptr_t pa_len=(len+(0x1000-len%0x1000))/0x1000+(phys%0x1000
			                                                   +len%0x1000)/0x1000;
			for(size_t s=0; s<1024; s++) {
				for(size_t i=0; i<pa_len; i++) {
					if(IS_SET(bitmap,s+i)) {
						goto skip;
					}
				}
				for(size_t i=0; i<pa_len; i++) {
					SET(bitmap,s+i);
					hal::map_phys_to_virt_cur(start_loc+(s+i)*0x1000,pa_phys+i*0x1000,
					{false,false,true});
				}
				if(alloc_len) {
					*alloc_len=(pa_len*0x1000)-(phys&0xFFF);
				}
				return (phys&0xFFF)+start_loc+s*0x1000;
			skip:;
			}
			return 0;
		}
		void unget_phys(uintptr_t phys, uintptr_t len, uintptr_t virt) {
			virt-=phys&0xFFF;
			size_t s=(virt-start_loc)/0x1000;
			uintptr_t pa_len=(len+(0x1000-len%0x1000))/0x1000+(phys%0x1000
			                                                   +len%0x1000)/0x1000;
			for(size_t i=0; i<pa_len; i++) {
				hal::unmap_virt_phys_cur(virt+i*0x1000);
				UN_SET(bitmap,s+i);
			}
		}
	}
}
