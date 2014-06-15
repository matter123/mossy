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
#include <string.h>
#include <hal/hal.h>
#include <hal/mmap.h>
#include <hal/console.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
extern "C" uint32_t k_start;
extern "C" uint32_t k_data_end;
namespace hal {
	static size_t tag_count=0;
	static mem_type types[5];
	static mem_region *regionsi;
	void init_type();
	bool init_mem() {
		init_type();
		multiboot_mmap *mmap_tag=NULL;
		for(size_t s=0; s<get_tag_count(); s++) {
			if(get_tag(s)->type == 6) {
				mmap_tag=reinterpret_cast<multiboot_mmap *>(get_tag(s));
				break;
			}
		}

		if(!mmap_tag) {
			return false;
		}
		tag_count=(mmap_tag->head.size-sizeof(mmap_tag)) / (mmap_tag->entry_size);
		regionsi=reinterpret_cast<mem_region *>(
		             w_malloc(sizeof(mem_region) * tag_count, 16));
		for(size_t s=0; s<tag_count; s++) {
			void *tptr=reinterpret_cast<void *>(mmap_tag);
			tptr+=sizeof(multiboot_mmap);
			tptr+=(s*mmap_tag->entry_size);
			multiboot_mmap_ent *ent=reinterpret_cast<multiboot_mmap_ent *>(tptr);
			regionsi[s].start=ent->addr;
			regionsi[s].end=regionsi[s].start + ent->len;
			regionsi[s].type=types[ent->type];
		}
		//allow the arch and vendor add custom regions
		hal::cout<<tag_count<<hal::endl;
		add_special_mem_arch();
		add_special_mem_vendor();

		//add kernel
		add_region(1);
		uintptr_t start=((uint64_t) &k_start)-get_page_offset_addr();
		uintptr_t end=(((uint64_t) &k_data_end)-start)-get_page_offset_addr();
		add_region(start,end,types[0]);
		//print regions
		for(size_t s=0; s<tag_count; s++) {
			hal::cout<<hal::dec<<"R "<<s<<": "<<hal::address
			         <<regionsi[s].start<<" "<<regionsi[s].end<<"\t"<<hal::hex
			         <<(uint64_t)(regionsi[s].type.to_u64())<<hal::endl;
		}

		return true;
	}
	//multiboot defines 4 main mem_types here they are
	void init_type() {
		types[0].kernel=true;
		types[0].resv_mem=true;

		types[1].avil=true;

		types[2].resv_mem=true;

		types[3].avil=true;
		types[3].firmware=true;

		types[4].save_on_hib=true;
	}
	int add_reg_count=0;
	bool add_region(uint64_t start, uint64_t len, mem_type type) {
		//hal::cout<<add_reg_count<<hal::endl;
		if(add_reg_count) {
			//hal::cout<<tag_count<<hal::endl;
			regionsi[tag_count].start=start;
			regionsi[tag_count].end=start+len;
			regionsi[tag_count++].type=type;
			add_reg_count--;
			return true;
		}
		return false;
	}
	void add_region(int count) {
		if((count-add_reg_count)>0) {
			hal::cout<<count<<" "<<tag_count<<hal::endl;
			// need to alloc more space
			// QnD impl of realloc
			void *space=w_malloc((tag_count+count)*sizeof(mem_region), 16);
			hal::cout<<(void *)regionsi+(tag_count *sizeof(mem_region))<<hal::endl;
			hal::cout<<space<<hal::endl;
			memcpy(space, (void *)regionsi, tag_count*sizeof(mem_region));
			regionsi=reinterpret_cast<mem_region *>(space);
			// w_malloc does not have w_free()
		}
		add_reg_count=count;
	}
}
