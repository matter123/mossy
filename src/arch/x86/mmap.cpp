/*
 * Copyright 2014 Matthew Fosdick

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

#include <stdint.h>
#include "mmap.h"
#include "../../monitor.h"
#include <cstring>
#include "../../panic.h"
#include "../../hhalf.h"
#include <math.hpp>

#define K128 131072

extern "C" uint32_t k_end;//we have 128kb after this reserved
extern "C" uint32_t k_start;
extern "C" uint32_t k_data_end; /*end of working space, PFA and paging
								  needs to be up by then so we don't run out
								  of room*/
namespace kernel {


	uint32_t k_start_data_end;
	mmap_field_t *mmap;
	int mmap_count;
	void *workspace_alloc_ptr;

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
		k_start_data_end=(uint32_t)pos;
		workspace_alloc_ptr=(void *)k_start_data_end;
		return mboot;
	}

	void parse_mboot_mmap(multiboot_t *mboot) {
		mmap=(mmap_field_t *)mboot->mmap_ptr;
		mmap_count=mboot->mmap_length/sizeof(mmap_field_t);

		//fix memory maps
		for(int i=0;i<mmap_count;i++) {
			//at boot end is really length, this corrects it
			mmap[i].end=mmap[i].start+mmap[i].end-1;
			//e820 has 2 and 3 swapped this fixes it
			if(mmap[i].type==2)mmap[i].type=3;
			else if(mmap[i].type==3)mmap[i].type=2;
			if(mmap[i].start>=mmap[i].end)mmap[i].type|=(1<<3);
		}
		init_page_frame_alloc();
	}
	void *workspce_alloc(size_t size) {
		void *ptr=workspace_alloc_ptr;
		workspace_alloc_ptr=(void *)((uintptr_t)workspace_alloc_ptr+size);
		return ptr;
	}
	void remove_invalid() {
		int entrycount=0;
		for(int i=0;i<mmap_count;i++) {
			//check if bit is not set
			if(~(mmap[i].type&(1<<3)))entrycount++;
		}
		mmap_field_t *mmap_p=(mmap_field_t *)workspce_alloc(sizeof(mmap_field_t)*entrycount);
		int mmap_counter=0;
		while(mmap_counter<entrycount) {
			for(int i=0;i<mmap_count;i++) {
				//check if bit is not set
				if(~(mmap[i].type&(1<<3))) {
					std::memmove(&mmap_p[mmap_counter++],&mmap[i],sizeof(mmap_field_t));
				}
			}
		}
		mmap=mmap_p;
		mmap_count=entrycount;
	}

	void sort() {
		//bubble sort, I'm lazy
		mmap_field_t *temp=(mmap_field_t *)workspce_alloc(sizeof(mmap_field_t));
		int passes;
		do {
			passes=0;
			for(int i=0;i<mmap_count-1;i++) {
				if(mmap[i].start>mmap[i+1].start) {
					//swap
					passes++;
					std::memmove(&temp,&mmap[i],sizeof(mmap_field_t));
					std::memmove(&mmap[i],&mmap[i+1],sizeof(mmap_field_t));
					std::memmove(&mmap[i+1],&temp,sizeof(mmap_field_t));
				}
			}
		}while(passes>0);
	}
	void merge() {
		//ill fix pointer later
		mmap_field_t *temp=(mmap_field_t *)workspce_alloc(sizeof(mmap_field_t));
		int entrycount=0;
		for(int i=1;i<mmap_count;i++) {
			if(entrycount==0)std::memmove(&temp[entrycount++],&mmap[i-1],sizeof(mmap_field_t));
			//there are 3 cases
			//1. no overlap
			//2. intersection
			//3. contained

			//case 1
			if(temp[entrycount-1].end<mmap[i].start) {
				std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
				continue;
			}

			//case 2
			if(temp[entrycount-1].end+1<mmap[i].end) {
				//temp gets overlap
				if(temp[entrycount-1].type>mmap[i].type) {
					mmap[i].start=temp[entrycount-1].end+1;
					//check for an invalid entry
					if(mmap[i].start==mmap[i].end)continue;
					std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
					continue;
				}
				//mmap gets overlapping area
				temp[entrycount-1].end=mmap[i].start-1;
				if(temp[entrycount-1].start==temp[entrycount-1].end) {
					std::memmove(&temp[entrycount-1],&mmap[i],sizeof(mmap_field_t));
					continue;
				}
				std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
			}

			//case 3
			if(temp[entrycount-1].type>=mmap[i].type) continue;
			uint64_t tend=temp[entrycount-1].end;
			temp[entrycount-1].end=mmap[i].start+1;
			if(temp[entrycount-1].start==temp[entrycount-1].end) {
				std::memmove(&temp[entrycount-1],&mmap[i],sizeof(mmap_field_t));
				continue;
			}
			std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
			//reserve entry
			entrycount++;
			temp[entrycount-1].end=tend;
			temp[entrycount-1].start=mmap[i].end+1;
			temp[entrycount-1].type=temp[entrycount-3].type;
		}
		mmap=temp;
		mmap_count=entrycount;
		workspace_alloc_ptr=(void *)((uintptr_t)(workspace_alloc_ptr)+(entrycount-1)*sizeof(mmap_field_t));
	}
	void fill() {
		mmap_field_t *temp=(mmap_field_t *)workspce_alloc(sizeof(mmap_field_t));
		int entrycount=0;
		for(int i=1;i<mmap_count;i++) {
			if(entrycount==0)std::memmove(&temp[entrycount++],&mmap[i-1],sizeof(mmap_field_t));
			if(mmap[i].start-temp[entrycount-1].end>3) {
				entrycount++;
				temp[entrycount-1].end=mmap[i].start-1;
				temp[entrycount-1].start=temp[entrycount-2].end+1;
				temp[entrycount-1].type=7;
				std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
				continue;
			}else if(mmap[i].start-temp[entrycount-1].end>1) {
				//most restrictive gets it
				if(temp[entrycount-1].type>mmap[i].type) {
					temp[entrycount-1].end=mmap[i].start-1;
				}else {
					mmap[i].start=temp[entrycount-1].end+1;
				}
				std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
				continue;
			}
			std::memmove(&temp[entrycount++],&mmap[i],sizeof(mmap_field_t));
			continue;
		}
		mmap=temp;
		mmap_count=entrycount;
		workspace_alloc_ptr=(void *)((uintptr_t)(workspace_alloc_ptr)+(entrycount-1)*sizeof(mmap_field_t));
	}
	void align() {
		mmap_field_t *temp=(mmap_field_t *)workspce_alloc(sizeof(mmap_field_t)*mmap_count);
		for(int i=1;i<mmap_count;i++) {
			if(i==1)std::memmove(&temp[0],&mmap[0],sizeof(mmap_field_t));
			if((temp[i-1].end&0xFFF)==0xFFF) {
				std::memmove(&temp[i],&mmap[i],sizeof(mmap_field_t));
				continue;
			}
			if(temp[i-1].type>=mmap[i].type) {
				//temp gets page
				temp[i-1].end|=0xFFF;
				mmap[i].start=temp[i-1].end+1;
				std::memmove(&temp[i],&mmap[i],sizeof(mmap_field_t));
				continue;
			}
			//mmap gets page
			mmap[i].start&=0xFFFFF000;
			temp[i-1].end=mmap[i].start-1;
			std::memmove(&temp[i],&mmap[i],sizeof(mmap_field_t));
		}
		mmap=temp;
	}
	void init_page_frame_alloc() {
		remove_invalid();
		sort();
		merge();
		fill();
		align();
		for (int i=0;i<mmap_count;i++) {
			std::cout<<std::dec<<i<<". "<<hex32<<(uint32_t)mmap[i].start<<" "<<(uint32_t)mmap[i].end<<" "<<(mmap[i].type&0x7)<<std::endl;
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
}
