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

#include <hal/memmap.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
#include <linker.h>
#include <string.h>
#include <vga_text.h>
namespace hal {
memmap virtmem;
memmap physmem;
bool memmap::init() {
	multiboot_mmap *mmap_tag = nullptr;
	for(size_t s = 0; s < get_tag_count(); s++) {
		if(get_tag(s)->type == t_memory_map) {
			mmap_tag = reinterpret_cast<multiboot_mmap *>(get_tag(s));
			break;
		}
	}
	if(!mmap_tag) {
		Log(LOG_ERROR, "[MEMMAP]", "could not find memory map");
		return false;
	}
	// for the kernel
	this->regs.tag_count = 1;
	mem_type kmem;
	kmem.kernel = true;
	this->regs.regions[0].start = KERNEL_VMA + KERNEL_LMA;
	this->regs.regions[0].end = K_DATA_END;
	this->regs.regions[0].type = kmem;
	if(this == &physmem) {
		this->regs.regions[0].start = KERNEL_LMA;
		this->regs.regions[0].end -= KERNEL_VMA;
	}
	region_hook *current = this->next;
	while(current) {
		current->add_region_hook(this);
		current = current->next;
	}
	fix();
	for(size_t s = 0; s < this->regs.tag_count; s++) {
		hal::mem_region *r = &this->regs.regions[s];
		Log(LOG_DEBUG, "[MEMMAP]", "%.16p  %.16p  %#.4X", r->start, r->end, r->type.to_u64());
	}
	done = true;
	return true;
}
void memmap::fix() {
	// preliminary sanity check
	// make address ordered
	// split in that have conflicts
	// page align new regions
	// remove any 0 sized entries
	// fill remianing address space
	this->regs = *remove_invalid(&this->regs);
	this->regs = *sort(&this->regs);
	this->regs = *split(&this->regs);
	this->regs = *page_align(&this->regs);
	this->regs = *remove_invalid(&this->regs);
	this->regs = *fill(&this->regs);
}

int memmap::region_count() {
	return this->regs.tag_count;
}
mem_region *memmap::get_region(int index) {
	if(index < 0 || index >= this->regs.tag_count) {
		return nullptr;
	}
	return &this->regs.regions[index];
}
void memmap::add_region_hook(region_hook *rhook) {
	rhook->next = this->next;
	this->next = rhook;
}
void memmap::add_region(uint64_t start, uint64_t end, mem_type type) {
	this->regs.regions[this->regs.tag_count].start = start;
	this->regs.regions[this->regs.tag_count].end = end;
	this->regs.regions[this->regs.tag_count].type = type;
	this->regs.tag_count++;
	if(done) {
		fix();
	}
}

region_hook::region_hook(memmap &mmap, void (*rhook)(memmap *mem)) {
	this->add_region_hook = rhook;
	mmap.add_region_hook(this);
}

void add_phys_multiboot(memmap *mem) {
	Log(LOG_DEBUG, "[MEMMAP]", "adding physical memory regions");
	multiboot_mmap *mmap_tag = nullptr;
	for(size_t s = 0; s < get_tag_count(); s++) {
		if(get_tag(s)->type == t_memory_map) {
			mmap_tag = reinterpret_cast<multiboot_mmap *>(get_tag(s));
			break;
		}
	}
	size_t total_size = mmap_tag->head.size - (sizeof(multiboot_mmap) - mmap_tag->entry_size);
	size_t tag_count = total_size / mmap_tag->entry_size;
	mem_type types[5] = {};
	types[0].kernel = true;
	types[0].resv_mem = true;
	types[1].avil = true;
	types[2].resv_mem = true;
	types[3].avil = true;
	types[3].firmware = true;
	types[4].save_on_hib = true;
	// convert from multiboot_mmap_ent to mem_region
	for(size_t s = 0; s < tag_count; s++) {
		multiboot_mmap_ent ent = mmap_tag->entries[s];
		// Log(LOG_DEBUG, "[MEMMAP]", "region %.16p %.16p %d",ent.addr,ent.addr + ent.len,ent.type);
		mem->add_region(ent.addr, ent.addr + ent.len, types[ent.type > 4 ? 2 : ent.type]);
	}
}
region_hook rhook(physmem, &add_phys_multiboot);
}
