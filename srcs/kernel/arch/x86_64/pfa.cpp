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
#include <arch.h>
#ifdef X86_64
#include "pfa.h"
#include <panic.h>
#include <string.h>
#include <hal/mmap.h>
#include <hal/hal.h>
#include <hal/paging.h>
#include <hal/console.h>
#include <math.hpp>
#include <x86/paging.h>
#include <x64/paging.h>
extern "C" uintptr_t k_data_end;
extern "C" uintptr_t k_stack_end;
namespace x86_64 {
	static uintptr_t *stack=&k_data_end;
	static uintptr_t *end=0;
	static uintptr_t count=0;
	bool init_pfa() {
		hal::mem_type avil;
		memset(&avil,0,sizeof(hal::mem_type));
		avil.avil=true;
		size_t max=hal::get_phys_mem_regions();
		for(size_t s=0; s<max; s++) {
			hal::mem_region *reg=hal::get_phys_mem_region(s);
			if(avil==reg->type) {
				uintptr_t free_page=reg->start;
				while(free_page<reg->end) {
					add_free_page(free_page);
					free_page+=0x1000;
				}
			}
		}
		return true;
	}
	uintptr_t get_free_page() {
		if(count) {
			uintptr_t page=*stack;
			stack--;
			count--;
			return page;
		}
		kernel::panic("Out of Memory");
		return 0x1;//an invalid page
	}
	void add_free_page(uintptr_t page) {
		if(((uintptr_t)stack&0xFFF)==(0x1000-sizeof(uintptr_t))) {
			*(stack+1)=0; //poke it with a stick
		}
		if(stack==&k_stack_end) {
			#ifdef X86
			using namespace x86;
			#elif defined X64
			using namespace x64;
			#endif
			if(!paging_ready()) {
				hal::print_boot_msg("Init paging",init_paging(),true);
			}
			uintptr_t _end=hal::get_page_offset_addr()+0x400000;
			uintptr_t start=reinterpret_cast<uintptr_t>(&k_stack_end)+0x1000*8;
			//hal::magic_break();
			while(start<_end) {
				stack++;
				uintptr_t temp=hal::unmap_virt_phys_cur(start);
				*stack = temp;
				count++;
				start+=0x1000;
			}
		}
		stack++;
		*stack=page;
		count++;
		end=std::max(end,stack);
	}
	uintptr_t get_end() {
		return reinterpret_cast<uintptr_t>(end);
	}
}
#endif