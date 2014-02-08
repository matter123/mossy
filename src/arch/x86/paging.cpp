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

#include "paging.h"
#include "../../monitor.h"
#include <cstring>
#include "mmap.h"
#include <stdint.h>
#include "../../panic.h"
#include "../../hhalf.h"
namespace x86 {
	namespace paging {
		page_dir *k_page_dir;
		uint8_t *phys=(uint8_t*)0x000000;
		bool paging_active=false;

		void * begin_alloc_a() {
			void *alloc=(void *)0;
			return std::memset(alloc,0,0x1000);
		}

		void init_paging() {
			k_page_dir=(page_dir *)begin_alloc_a();
			//map 3 megabytes starteding at the start of bottom of memory
			uint8_t *virt=(uint8_t *)0xC0000000;
			for(int i=0;i<0x300;i++) {
				map_virt(k_page_dir,virt,&begin_alloc_a);
				virt+=0x1000;
			}
		}
		void map_virt(page_dir * d,void *virt,void *(*alloc_a)()) {
			while(!(kernel::is_valid_mem(phys))) {
				phys+=0x1000;
			}
			map_addr(d,phys,virt,alloc_a);
			phys+=0x1000;
		}
		void map_addr(page_dir * d,void *physadrr,void *virt,void *(*alloc_a)()) {
			uint32_t virti=(uint32_t)virt;
			int pde=virti/0x400000;
			page_table *pt=0;
			if(d->entriesi[pde]) {
				int pti=(uint32_t)d->entriesi[pde];
				pti&=0xFFFFF000;
				pt=(page_table *)pti;
			}else {
				pt=(page_table *)alloc_a();
				uint32_t pti=(uint32_t)pt;
				pti-=HIGH_HALF_BASE_ADDR;
				page_dir_e pdee=d->entries[pde];
				pdee.addr=(pti>>12);
				pdee.present=true;
				pdee.read_write=true;
				d->entries[pde]=pdee;
			}
			int pte=(virti%0x400000)/0x1000;
			int physi=((uint32_t)physadrr)>>12;
			page_table_e ptee=pt->entries[pte];
			ptee.addr=physi;
			ptee.present=true;
			ptee.read_write=true;
			pt->entries[pte]=ptee;
		}
	}
}
