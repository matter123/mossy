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
#include <hal/workspace.h>
extern "C" uintptr_t k_start;
extern "C" uintptr_t k_data_end;
namespace hal {
	static mem_regs regs;
	static void fix_mmap();
	bool init_virt_mem() {
		regs.tag_count=1;
		regs.regions=reinterpret_cast<mem_region *>(
		                 w_malloc(sizeof(mem_region) * regs.tag_count, 16));
		add_virt_mem_arch();
		add_virt_mem_vendor();
		//add kernel
		add_virt_region(1);
		mem_type kern;
		memset(&kern, 0, sizeof(mem_type));
		kern.kernel=true;
		uintptr_t start=((uint64_t) &k_start);
		uintptr_t end=(((uint64_t) &k_data_end)-start);
		add_virt_region(start,end,kern);
		fix_mmap();
		return true;
	}
	static int add_reg_count=0;
	bool add_virt_region(uint64_t start, uint64_t len, mem_type type) {
		if(add_reg_count) {
			regs.regions[regs.tag_count].start=start;
			regs.regions[regs.tag_count].end=start+len;
			regs.regions[regs.tag_count++].type=type;
			add_reg_count--;
			return true;
		}
		return false;
	}
	void add_virt_region(int count) {
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
		regs=*remove_invalid(split(sort(remove_invalid(&regs))));
		//regs=*remove_invalid(&regs);
		memmove((void *)old,regs.regions,sizeof(mem_region)*regs.tag_count);
		regs.regions=reinterpret_cast<mem_region *>(old);
		wksp_begin((pointer)old+sizeof(mem_region)*regs.tag_count);
	}
	mem_region *get_virt_mem_region(int index) {
		if(index<regs.tag_count) {
			return &regs.regions[index];
		}
		return NULL;
	}

	int get_virt_mem_regions() {
		return regs.tag_count;
	}
}
