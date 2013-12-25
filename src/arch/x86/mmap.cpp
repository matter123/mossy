/*
 * Copyright 2013 Matthew Fosdick

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

#include "mmap.h"
#include "../../monitor.h"
#include <cstring>
#include "../../panic.h"
#include "../../hhalf.h"


extern "C" uint32_t k_end;
extern "C" uint32_t k_start;
namespace kernel {
	uint32_t k_data_end;
	mmap_field_t *mmap;
	int mmap_count;

	void *fix(void *ptr);

	multiboot_t *fix_tables(multiboot_t *mboot) {
		mboot=(multiboot_t *)fix(mboot);
		mboot=(multiboot_t *)std::memmove(&k_end,mboot,sizeof(multiboot_t));
		uint8_t *pos=(uint8_t *)&k_end+sizeof(multiboot_t);
		mboot->mods_ptr=std::memmove(pos,fix(mboot->mods_ptr),16*mboot->mods_count);
		pos+=16*mboot->mods_count;
		mboot->mmap_ptr=std::memmove(pos,fix(mboot->mmap_ptr),mboot->mmap_length);
		pos+=mboot->mmap_length;
		mboot->drives_ptr=std::memmove(pos,fix(mboot->drives_ptr),mboot->drives_length);
		pos+=mboot->drives_length;
		mboot->cmdline_ptr=std::memmove(pos,fix(mboot->cmdline_ptr),std::strlen((std::c_cstring)mboot->cmdline_ptr));
		pos+=std::strlen((std::c_cstring)mboot->cmdline_ptr);
		mboot->vbe_control_info_ptr=std::memmove(pos,fix(mboot->vbe_control_info_ptr),512);
		pos+=512;
		mboot->vbe_mode_info_ptr=std::memmove(pos,fix(mboot->vbe_mode_info_ptr),256);
		pos+=256;
		k_data_end=(uint32_t)pos+0x1000;
		return mboot;
	}

	void parse_mboot_mmap(multiboot_t *mboot) {
		mmap=(mmap_field_t *)mboot->mmap_ptr;
		mmap_count=mboot->mmap_length/sizeof(mmap_field_t);

		//fix memory maps
		for(int i=0;i<mmap_count;i++) {
			//at boot end is really length, this corrects it
			mmap[i].end=mmap[i].start+mmap[i].end;

			//make sure there page aligned
			//this slides them towards the inside
			mmap[i].end&=0xFFFFF000;
			if(mmap[i].start&0xFFF) {
				mmap[i].start=(mmap[i].start&0xFFFFF000)+0x1000;
			}
			if(mmap[i].type!=1)mmap[i].type=0;
		}
	}

	bool is_valid_mem(void *addr) {
		uint32_t addri=(uint32_t)addr;
		addri&=0xFFFFF000;
		bool valid=false;
		for(int i=0;i<mmap_count;i++) {
			if(addri>=mmap[i].start&&addri<mmap[i].end) {
				if(mmap[i].type) valid=true;
				else return false;
			}
		}
		return valid;
	}

	void *fix(void *ptr) {
		if((uintptr_t)ptr<HIGH_HALF_BASE_ADDR)ptr=(void *)(((uintptr_t)ptr)+HIGH_HALF_BASE_ADDR);
		return ptr;
	}

	void * get_next_aligned() {
		k_data_end&=0xFFFFF000;
		k_data_end+=0x1000;
		return (void *)k_data_end;
	}
}
