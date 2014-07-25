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
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <hal/hal.h>
#include <hal/mmap.h>
#include <hal/console.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
extern "C" uintptr_t k_start;
extern "C" uintptr_t k_data_end;
namespace hal {
	static mem_type types[6];
	static mem_regs regs;
	static void init_type();
	static void fix_mmap();
	bool init_phys_mem() {
		init_type();
		multiboot_mmap *mmap_tag=NULL;
		//get find mmap_tag
		for(size_t s=0; s<get_tag_count(); s++) {
			if(get_tag(s)->type == 6) {
				mmap_tag=reinterpret_cast<multiboot_mmap *>(get_tag(s));
				break;
			}
		}

		if(!mmap_tag) {
			return false;
		}
		regs.tag_count=(mmap_tag->head.size-sizeof(mmap_tag)) / (mmap_tag->entry_size);
		regs.regions=reinterpret_cast<mem_region *>(
		                 w_malloc(sizeof(mem_region) * regs.tag_count, 16));
		//convert from multiboot_mmap_ent to mem_region
		for(size_t s=0; s<regs.tag_count; s++) {
			void *tptr=reinterpret_cast<void *>(mmap_tag);
			tptr+=sizeof(multiboot_mmap);
			tptr+=(s*mmap_tag->entry_size);
			multiboot_mmap_ent *ent=reinterpret_cast<multiboot_mmap_ent *>(tptr);
			regs.regions[s].start=ent->addr;
			regs.regions[s].end=regs.regions[s].start + ent->len;
			regs.regions[s].type=types[ent->type];
		}

		add_phys_mem_arch();
		add_phys_mem_vendor();

		//add kernel
		add_phys_region(1);
		uintptr_t start=((uint64_t) &k_start)-get_page_offset_addr();
		uintptr_t end=(((uint64_t) &k_data_end)-start)-get_page_offset_addr();
		add_phys_region(start,end,types[0]);

		fix_mmap();
		return true;
	}

	static void init_type() {
		types[0].kernel=true;
		types[0].resv_mem=true;

		types[1].avil=true;

		types[2].resv_mem=true;

		types[3].avil=true;
		types[3].firmware=true;

		types[4].save_on_hib=true;

		types[5].no_exist=true;
	}
	static int add_reg_count=0;
	bool add_phys_region(uint64_t start, uint64_t len, mem_type type) {
		if(add_reg_count) {
			regs.regions[regs.tag_count].start=start;
			regs.regions[regs.tag_count].end=start+len;
			regs.regions[regs.tag_count++].type=type;
			add_reg_count--;
			return true;
		}
		return false;
	}
	void add_phys_region(int count) {
		if((count-add_reg_count)>0) {
			// need to alloc more space
			// QnD impl of realloc
			void *space=w_malloc((regs.tag_count+count)*sizeof(mem_region), 16);
			memcpy(space, (void *)regs.regions, regs.tag_count*sizeof(mem_region));
			regs.regions=reinterpret_cast<mem_region *>(space);
			// w_malloc does not have w_free()
		}
		add_reg_count=count;
	}
	static void fix_mmap() {
		uintptr_t old=reinterpret_cast<uintptr_t>(regs.regions);
		regs=*fill(remove_invalid(page_align(split(sort(remove_invalid(&regs))))));
		memmove((void *)old,regs.regions,sizeof(mem_region)*regs.tag_count);
		regs.regions=reinterpret_cast<mem_region *>(old);
		wksp_begin((void *)old+sizeof(mem_region)*regs.tag_count);
	}
	mem_region *get_phys_mem_region(int index) {
		if(index<regs.tag_count) {
			return &regs.regions[index];
		}
		return NULL;
	}

	int get_phys_mem_regions() {
		return regs.tag_count;
	}
}