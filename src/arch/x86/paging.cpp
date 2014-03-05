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
#include "pfa.h"
#include <stdint.h>
#include "cstring"
#include "../../monitor.h"
#include "../../panic.h"

namespace x86 {
	namespace paging {
		void inv_page(uintptr_t virt) {
			//assume 486 or better
			asm volatile("invlpg (%0)" ::"r"(virt&(~0xFFF)) : "memory");
		}
		void inv_page(void *virt) {
			inv_page(reinterpret_cast<uintptr_t>(virt));
		}
		class rec_page_manager {
			private:
				uintptr_t rec_start;
				page_dir *cur_dir;
				page_table *tables;
				bool is_page_table_present(uint32_t pt_index) {
					return cur_dir->entries[pt_index].present;
				}
				int from_config(page_config pg) {
					return (pg.present<<0)|(pg.read_write<<1)|(pg.all<<2)|(pg.global<<8);
				}
			public:
				rec_page_manager(uintptr_t rec_start_addr,int pde_index) {
					this->rec_start=rec_start_addr;
					this->cur_dir=reinterpret_cast<page_dir *>(rec_start+(0x1000*pde_index));
					this->tables=reinterpret_cast<page_table *>(rec_start);
				}
				void map(void *virt,page_config pg) {
					map_phys(virt,get_frame(),pg);
				}
				void map_phys(void *virt,void *phys,page_config pg) {
					uint32_t pde=reinterpret_cast<uint32_t>(virt)>>22;
					if(!is_page_table_present(pde)) {
						void *pde_phys=get_frame();
						pg.present=1;
						int config=0x7;
						cur_dir->entriesi[pde]=reinterpret_cast<uintptr_t>(pde_phys)|config;
					}
					inv_page(rec_start*0x1000*pde);
					uint32_t pte=(reinterpret_cast<uint32_t>(virt)<<10)>>22;
					if(tables[pde].entries[pte].present) {
						free_frame(reinterpret_cast<void *>(tables[pde].entries[pte].addr<<12));
					}
					tables[pde].entriesi[pte]=(reinterpret_cast<uintptr_t>(phys)&0xFFFFF000)|from_config(pg);
					inv_page(virt);
				}
				void unmap(void *virt) {
					uint32_t pde=reinterpret_cast<uint32_t>(virt)>>22;
					uint32_t pte=(reinterpret_cast<uint32_t>(virt)<<10)>>22;
					free_frame(reinterpret_cast<void *>(tables[pde].entries[pte].addr<<12));
					tables[pde].entries[pte].present=false;
					bool free=true;
					for(int i=0;i<1024;i++) {
						if(tables[pde].entries[i].present) {
							free=false;
							break;
						}
					}
					if(free) {
						free_frame(reinterpret_cast<void *>(cur_dir->entries[pte].addr<<12));
						cur_dir->entries[pte].present=false;
					}
				}
		};
		uintptr_t rec_start=0xFFC00000;
		uintptr_t old_rec = 0xFF800000;
		uintptr_t zero_phys_ptr;
		uintptr_t pdt_phys;
		rec_page_manager pm(rec_start,1023);
		bool pe=false;
		bool paging_enabled() {
			return pe;
		}
		void enable_paging() {
			pdt_phys=reinterpret_cast<uintptr_t>(get_frame());
			//map current page dir to rec_start
			*(reinterpret_cast<uintptr_t *>(rec_start-0x1004))=pdt_phys|0x3;
			inv_page(rec_start-0x1004);
			inv_page(0xFFFFF000);
			std::memmove(reinterpret_cast<void *>(0xFFFFF000),reinterpret_cast<void *>(rec_start-0x2000),sizeof(page_dir));
			asm volatile ("mov %0, %%eax\n"
				"mov %%eax, %%cr3\n"
				:: "m" (pdt_phys));
			pe=true;
		}

		void handle_pf(uint32_t exc_code) {
			if(!pe) {
				std::cout<<"page fault while enabling paging, can not continue booting"<<std::endl;
				asm("cli\nhlt");
			}
		}
	}
}