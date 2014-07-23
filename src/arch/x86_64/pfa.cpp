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
#include <string.h>
#include <hal/mmap.h>
#include <hal/hal.h>
#include <hal/console.h>
extern "C" uintptr_t k_data_end;
namespace x86_64 {
	static uintptr_t *stack=reinterpret_cast<uintptr_t *>(&k_data_end);
	static uintptr_t count=0;
	bool init_pfa() {
		hal::mem_type avil;
		memset(&avil,0,sizeof(hal::mem_type));
		avil.avil=true;
		size_t max=hal::get_mem_regions();
		for(size_t s=0; s<max; s++) {
			hal::mem_region *reg=hal::get_mem_region(s);
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
		return 0x1;//an invalid page
	}
	void add_free_page(uintptr_t page) {
		if(((uintptr_t)stack&0xFFF)==(0x1000-sizeof(uintptr_t))) {
			//hal::cout<<stack<<" ";
			//hal::magic_break();
			*(stack+1)=0; //poke it with a stick
		}
		stack++;
		*stack=page;
		count++;
	}
}
#endif