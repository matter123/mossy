/*
	Copyright 2016 Matthew Fosdick
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

#include <string.h>
#include <hal/memmap.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
#include <linker.h>
#include <vga_text.h>
namespace hal {
	memmap virtmem;
	memmap physmem;
	bool memmap::init() {
		multiboot_mmap *mmap_tag=nullptr;
		for(size_t s=0; s<get_tag_count(); s++) {
			if(get_tag(s)->type == t_memory_map) {
				mmap_tag = reinterpret_cast<multiboot_mmap *>(get_tag(s));
				break;
			}
		}
		if(!mmap_tag) {
			Log(LOG_ERROR,"[MEMMAP]","could not find memory map");
			return false;
		}
		//for the kernel
		this->regs.tag_count=1;
		this->regs.regions=reinterpret_cast<mem_region *>(w_malloc(sizeof(mem_region),16));
		mem_type kmem;
		kmem.kernel=true;
		this->regs.regions[0].start=KERNEL_VMA+KERNEL_LMA;
		this->regs.regions[0].end=K_DATA_END;
		this->regs.regions[0].type=kmem;
		if(this==&physmem) {
			this->regs.regions[0].start=KERNEL_LMA;
			this->regs.regions[0].end-=KERNEL_VMA;
		}
		region_hook *current=this->next;
		while(current) {
			current->add_region_hook(this);
			current=current->next;
		}
		uintptr_t old = reinterpret_cast<uintptr_t>(this->regs.regions);
		//preliminary sanity check
		//make address ordered
		//split in that have conflicts
		//page align new regions
		//remove any 0 sized entries
		//fill remianing address space
		this->regs=*remove_invalid(&this->regs);
		this->regs=*sort(&this->regs);
		this->regs=*split(&this->regs);
		this->regs=*page_align(&this->regs);
		this->regs=*remove_invalid(&this->regs);
		this->regs=*fill(&this->regs);
		//reclaim space used by the fixiing
		memmove((void *)old,this->regs.regions,sizeof(mem_region)*this->regs.tag_count);
		this->regs.regions=reinterpret_cast<mem_region *>(old);
		wksp_reset((pointer)old + sizeof(mem_region)*this->regs.tag_count);
		return true;
	}

	int memmap::region_count() {
		return this->regs.tag_count;
	}
	mem_region *memmap::get_region(int index) {
		if(index< 0 || index >= this->regs.tag_count) {
			return nullptr;
		}
		return &this->regs.regions[index];
	}
	void memmap::add_region_hook(region_hook *rhook) {
		rhook->next=this->next;
		this->next=rhook;
	}
	void memmap::add_regions(int count) {
		size_t new_size=(this->regs.tag_count+count+this->add_count)*sizeof(mem_region);
		void *space=w_malloc(new_size,16);
		memcpy(space,(void *)this->regs.regions,new_size);
		this->regs.regions=reinterpret_cast<mem_region *>(space);
		this->add_count+=count;
	}
	bool memmap::add_region(uint64_t start, uint64_t end,mem_type type) {
		if(this->add_count) {
			this->regs.regions[this->regs.tag_count].start=start;
			this->regs.regions[this->regs.tag_count].end=end;
			this->regs.regions[this->regs.tag_count].type=type;
			this->regs.tag_count++;
			this->add_count--;
			return true;
		}
		return false;
	}

	region_hook::region_hook(memmap &mmap, void (*rhook)(memmap *mem)) {
		this->add_region_hook=rhook;
		mmap.add_region_hook(this);
	}

	void add_phys_multiboot(memmap *mem) {
		Log(LOG_DEBUG,"[MEMMAP]","adding physical memory regions");
		multiboot_mmap *mmap_tag=nullptr;
		for(size_t s=0; s<get_tag_count(); s++) {
			if(get_tag(s)->type == t_memory_map) {
				mmap_tag = reinterpret_cast<multiboot_mmap *>(get_tag(s));
				break;
			}
		}
		size_t total_size = mmap_tag->head.size-(sizeof(multiboot_mmap)-mmap_tag->entry_size);
		size_t tag_count = total_size / mmap_tag->entry_size;
		mem_type types[5]={};
		types[0].kernel=true;
		types[0].resv_mem=true;
		types[1].avil=true;
		types[2].resv_mem=true;
		types[3].avil=true;
		types[3].firmware=true;
		types[4].save_on_hib=true;
		mem->add_regions(tag_count);
		//convert from multiboot_mmap_ent to mem_region
		for(size_t s=0; s<tag_count; s++) {
			multiboot_mmap_ent ent=mmap_tag->entries[s];
			//Log(LOG_DEBUG, "[MEMMAP]", "region %.16p %.16p %d",ent.addr,ent.addr + ent.len,ent.type);
			mem->add_region(ent.addr,ent.addr + ent.len,types[ent.type>4?2:ent.type]);
		}
	}
	region_hook rhook(physmem, &add_phys_multiboot);
}
