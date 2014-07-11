/*
    Copyright 2013 Matthew Fosdick

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
extern "C" uint32_t k_start;
extern "C" uint32_t k_data_end;
namespace hal {
	static size_t tag_count=0;
	static mem_type types[6];
	static mem_region *regions;
	void print_regions();
	void init_type();
	void fix_mmap();
	bool init_mem() {
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
		tag_count=(mmap_tag->head.size-sizeof(mmap_tag)) / (mmap_tag->entry_size);
		regions=reinterpret_cast<mem_region *>(
		            w_malloc(sizeof(mem_region) * tag_count, 16));
		//convert from multiboot_mmap_ent to mem_region
		for(size_t s=0; s<tag_count; s++) {
			void *tptr=reinterpret_cast<void *>(mmap_tag);
			tptr+=sizeof(multiboot_mmap);
			tptr+=(s*mmap_tag->entry_size);
			multiboot_mmap_ent *ent=reinterpret_cast<multiboot_mmap_ent *>(tptr);
			regions[s].start=ent->addr;
			regions[s].end=regions[s].start + ent->len;
			regions[s].type=types[ent->type];
		}

		add_special_mem_arch();
		add_special_mem_vendor();

		//add kernel
		add_region(1);
		uintptr_t start=((uint64_t) &k_start)-get_page_offset_addr();
		uintptr_t end=(((uint64_t) &k_data_end)-start)-get_page_offset_addr();
		add_region(start,end,types[0]);

		//print_regions();
		//hal::cout<<hal::endl;
		fix_mmap();
		print_regions();
		hal::halt(true);

		return true;
	}

	void init_type() {
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
	bool add_region(uint64_t start, uint64_t len, mem_type type) {
		if(add_reg_count) {
			regions[tag_count].start=start;
			regions[tag_count].end=start+len;
			regions[tag_count++].type=type;
			add_reg_count--;
			return true;
		}
		return false;
	}
	void add_region(int count) {
		if((count-add_reg_count)>0) {
			// need to alloc more space
			// QnD impl of realloc
			void *space=w_malloc((tag_count+count)*sizeof(mem_region), 16);
			memcpy(space, (void *)regions, tag_count*sizeof(mem_region));
			regions=reinterpret_cast<mem_region *>(space);
			// w_malloc does not have w_free()
		}
		add_reg_count=count;
	}
	void print_regions() {
		hal::ios_base typeb(16,true,true,3);
		hal::ios_base r(10,true,true,3);
		if(sizeof(uintptr_t)==4) {
			hal::cout<<"REGION START      END           TYPE"<<hal::endl;
		} else if(sizeof(uintptr_t)==8) {
			hal::cout<<"REGION START              END                   TYPE"
			         <<hal::endl;
		}
		for(size_t s=0; s<tag_count; s++) {
			hal::cout<<r<<"R "<<s<<": "<<hal::address
			         <<regions[s].start<<" "<<regions[s].end<<"\t"
			         <<typeb<<(regions[s].type.to_u64())<<"\t"
			         //<<typeb<<&(regions[s].type)
			         <<hal::endl;
		}
		hal::cout<<sizeof(mem_type);
	}

	void remove_invalid() {
		int entrycount=0;
		for(size_t s=0; s<tag_count; s++) {
			if(regions[s].end>regions[s].start
			        &&regions[s].type.to_u64()!=0) {
				entrycount++;
			}
		}
		mem_region *mmap_p=reinterpret_cast<mem_region *>(w_malloc(sizeof(
		                       mem_region)*entrycount));
		int mc=0;
		while(mc<entrycount) {
			for(size_t s=0; s<tag_count; s++) {
				//check if bit is not set
				if(regions[s].end>regions[s].start
				        &&regions[s].type.to_u64()!=0) {
					memcpy(&mmap_p[mc++],&regions[s],sizeof(mem_region));
				}
			}
		}
		regions=mmap_p;
		tag_count=entrycount;
	}
	void sort() {
		mem_region temp;
		bool has_swap;
		do {
			has_swap=false;
			for(int i=1; i<tag_count; i++) {
				if(regions[i-1].start>regions[i].start) {
					memcpy(&temp,&regions[i],sizeof(mem_region));
					memcpy(&regions[i],&regions[i-1],sizeof(mem_region));
					memcpy(&regions[i-1],&temp,sizeof(mem_region));
					has_swap=true;
				}
				if(regions[i-1].start==regions[i].start) {
					if(regions[i-1].end>regions[i].end) {
						memcpy(&temp,&regions[i],sizeof(mem_region));
						memcpy(&regions[i],&regions[i-1],sizeof(mem_region));
						memcpy(&regions[i-1],&temp,sizeof(mem_region));
						has_swap=true;
					}
				}
			}
		} while(has_swap);
	}
	void split() {
		mem_region *t_regions=(mem_region *)w_malloc(sizeof(mem_region));
		int ecount=0;
		for(int i=1; i<tag_count; i++) {
			if(ecount==0) {
				memcpy(&t_regions[ecount++],&regions[0],sizeof(mem_region));
			}
			//case 1
			if(t_regions[ecount-1].end<regions[i].start) {
				memcpy(&t_regions[ecount++],&regions[i],sizeof(mem_region));
				continue;
			}
			//case 2
			if(t_regions[ecount-1].end<=regions[i].end) {
				//left takes conflict
				if(t_regions[ecount-1].type.is_more_restrictive(regions[i].type)) {
					memcpy(&t_regions[ecount++],&regions[i],sizeof(mem_region));
					t_regions[ecount-1].start=t_regions[ecount-2].end+1;
					if(t_regions[ecount-1].end<=t_regions[ecount-1].start) {
						ecount--;
					}
					continue;
				}
				//right takes conflict
				t_regions[ecount-1].end=regions[i].start-1;
				if(t_regions[ecount-1].end<=t_regions[ecount-1].start) {
					ecount--;
				}
				memcpy(&t_regions[ecount++],&regions[i],sizeof(mem_region));
				continue;
			}
			//case 3
			if(t_regions[ecount-1].type.is_more_restrictive(regions[i].type)) {
				//absorb (skip over and do nothing)
				continue;
			}

			//split
			uint64_t old_end=t_regions[ecount-1].end;
			t_regions[ecount-1].end=regions[i].start-1;
			memcpy(&t_regions[ecount++],&regions[i],sizeof(mem_region));
			t_regions[ecount].start=t_regions[ecount-1].end+1;
			t_regions[ecount].end=old_end;
			t_regions[ecount].type=t_regions[ecount-2].type;
			ecount++;
		}
		regions=t_regions;
		tag_count=ecount;
		wksp_begin((void *)regions+tag_count*sizeof(mem_region));
	}
	void page_align() {
#define START_MASK ~((uintptr_t)0xFFF)
		for(int i=0; i<tag_count; i++) {
			if((regions[i].start & START_MASK) != 0) {
				uint64_t old_start=regions[i].start;
				if(regions[i].type.can_grow()) {
					regions[i].start&=START_MASK;
				} else {
					regions[i].start&=START_MASK;
					regions[i].start+=0x1000;
				}
				if(i && regions[i-1].end==(old_start-1)) {
					regions[i-1].end=(regions[i].start-1);
				}
			}
			if((regions[i].end & 0xFFF) != 0xFFF) {
				if(regions[i].type.can_grow()) {
					regions[i].end=regions[i].end|0xFFF;
				} else {
					regions[i].end&=START_MASK;
					regions[i].end-=1;
				}
				if((i+1)<tag_count && regions[i+1].start<regions[i].end) {
					regions[i+1].start=regions[i].end+1;
				}
			}
		}
	}

	void fill() {
		mem_region *t_regions=(mem_region *)w_malloc(sizeof(mem_region));
		int ecount=0;
		for(int i=1; i<tag_count; i++) {
			if(ecount==0) {
				memcpy(&t_regions[ecount++],&regions[0],sizeof(mem_region));
			}
			if((regions[i].start-1)>t_regions[ecount-1].end) {
				t_regions[ecount].start=t_regions[ecount-1].end+1;
				t_regions[ecount].end=regions[i].start-1;
				t_regions[ecount].type=types[5];
				ecount++;
			}
			memcpy(&t_regions[ecount++],&regions[i],sizeof(mem_region));
		}
		regions=t_regions;
		tag_count=ecount;
		wksp_begin((void *)regions+tag_count*sizeof(mem_region));
	}
	void fix_mmap() {
		remove_invalid();
		sort();
		split();
		page_align();
		remove_invalid();
		fill();
	}
}