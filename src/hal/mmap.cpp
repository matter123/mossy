/*
    Copyright 2013 Matthew Fosdick

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
#include <stddef.h>
#include <stdint.h>
#include <hal/hal.h>
#include <hal/mmap.h>
#include <hal/console.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>

namespace hal {
	static size_t tag_count=0;
	static mem_type types[5];
	static mem_region_init *regionsi;
	void init_type();
	bool init_mem() {
		multiboot_mmap *mmap_tag=NULL;
		for(size_t s=0; s<get_tag_count(); s++) {
			if(get_tag(s)->type==6) {
				mmap_tag=reinterpret_cast<multiboot_mmap *>(get_tag(s));
				break;
			}
		}
		if(!mmap_tag) {
			return false;
		}
		hal::cout<<(void *)mmap_tag<<hal::endl;
		tag_count=(mmap_tag->head.size-sizeof(mmap_tag))/(mmap_tag->entry_size);
		regionsi=reinterpret_cast<mem_region_init *>(
		             w_malloc(sizeof(mem_region_init)*tag_count),16);
		for(size_t s=0; s<tag_count; s++) {
			void *tptr=reinterpret_cast<void *>(mmap_tag);
			tptr+=sizeof(multiboot_mmap);
			tptr+=(s*mmap_tag->entry_size);
			multiboot_mmap_ent *ent
			    =reinterpret_cast<multiboot_mmap_ent *>(tptr);
			regionsi[s].start=ent->addr;
			regionsi[s].end=regionsi[s].start+ent->len;
			regionsi[s].type=types[ent->type];
		}
		for(size_t s=0; s<tag_count; s++) {
			hal::cout<<hal::dec<<"R "<<s<<": "<<hal::address<<regionsi[s].start
			         <<" "<<regionsi[s].end<<" "<<hal::endl;
		}
		return true;
	}

	void init_type() {
		types[0].resv_mem=true;

		types[1].avil=true;

		types[3].avil=true;
		types[3].firmware=true;

		types[4].save_on_hib=true;
	}
}