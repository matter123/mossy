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
#include <sys/kstacks.h>
#include <hal/mmap.h>
#include <string.h>
namespace kernel {
	static size_t stacks_taken;
	static uint8_t *bitmap;
	static size_t total_stacks;
	uintptr_t stack_start;
	bool init_kstacks() {
		hal::mem_region *region=NULL;
		for(int i=0; i<hal::get_virt_mem_regions(); i++) {
			hal::mem_region *reg=hal::get_virt_mem_region(i);
			if(reg->type.kthread_stacks) {
				//for sanity only assume one section
				region=reg;
				break;
			}
		}
		if(!region) {
			return false;
		}
		uintptr_t len=region->end-region->start;
		total_stacks=len/(4*1024);//4KiB stacks
		uintptr_t temp=total_stacks;
		//0x8000 = 8 bits to a byte 16KiB stack means 0x8000 entries in a
		// bitmap before you need to use a new stack
		if(temp%0x8000) {
			temp+=(0x8000-(temp%0x8000));
		}
		stacks_taken=temp/0x8000;
		total_stacks-=stacks_taken;
		bitmap=reinterpret_cast<uint8_t *>(region->start);
		stack_start=region->start+0x1000*(stacks_taken+1);//1 is because stacks grow down
		temp=total_stacks/8;
		if(total_stacks%8) {
			temp++;
		}
		memset(bitmap,0,temp*0x1000); //PAGE FAULTS INCOMING
		return true;
	}

	size_t get_max_kstacks() {
		return total_stacks;
	}

	uintptr_t get_new_stack() {
		for(size_t s=0; s<total_stacks; s++) {
			if(!(bitmap[s/8]&(1<<s%8))) {
				bitmap[s/8]|=(1<<s%8);
				return stack_start+0x1000*s;
			}
		}
		return 0x0;
	}

	uintptr_t get_stack(size_t stack) {
		return stack_start+0x1000*stack;
	}

	void free_stack(uintptr_t ptr_in_stack) {
		intptr_t ps=ptr_in_stack-stack_start;
		if(ps<0) {
			bitmap[0]&=~(1<<0);
		}
		ps/=0x1000;
		ps++;
		bitmap[ps/8]&=~(1<<ps%8);
	}
}
