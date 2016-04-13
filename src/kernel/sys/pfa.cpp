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
#include <sys/pfa.h>
#include <arch/paging.h>
#include <hal/memmap.h>
#include <hal/commandline.h>
#include <numconv.h>
#include <linker.h>
static volatile uintptr_t *stack=reinterpret_cast<uintptr_t *>(K_DATA_END);
static size_t count=0;

void add_stack_region(hal::memmap *mem) {
	uintptr_t maxram=std::strtonum(get_arg("MaxRam"),20*1024)*1024*1024;
	uintptr_t usedspace=(maxram/0x1000)*4;
	hal::mem_type type;
	type.free_page_info=true;
	mem->add_region(K_DATA_END, K_DATA_END+usedspace, type);
}
static hal::region_hook rhook(hal::virtmem, &add_stack_region);
void pfa_init() {
	for(int i=0;i<hal::physmem.region_count();i++) {
		hal::mem_region *reg=hal::physmem.get_region(i);
		if(reg->type.avil&&!reg->type.firmware&&!reg->type.resv_mem) {
			uintptr_t start=reg->start;
			while(start<reg->end) {
				add_free_page(start);
				start+=0x1000;
			}
		}
	}
	Log(LOG_INFO, "[PFA   ]", "end pfa stack addr: %p",stack);
}

uintptr_t get_page() {
	if(count==0) {
		panic("out of memory");
	}
	uintptr_t ret=*stack;
	stack--;
	count--;
	return ret;
}
void add_free_page(uintptr_t page) {
	if(((uintptr_t)stack&0xFFF)==0x1000-sizeof(uintptr_t)) {
		if(!ismapped(reinterpret_cast<uintptr_t>(stack+1))) {
			map(*stack,reinterpret_cast<uintptr_t>(stack+1),PAGE_WRITE|PAGE_EXEC);
			stack--;
			count--;
		}
	}
	stack++;
	*stack=page;
	count++;
}
