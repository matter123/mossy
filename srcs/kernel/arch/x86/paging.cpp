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
#ifdef X86
#include <x86_64/pfa.h>
#include "paging.h"
#include <hal/paging.h>
#include <hal/int.h>
#include <hal/hal.h>
#include <hal/console.h>
#include <string.h>
#include <panic.h>
extern "C" uint32_t pd;
namespace x86 {
	static void pre_init_handle(cpu_state *s);
	static void handle(cpu_state *s);
	static bool init=false;
	static recursive_paging p;
	bool init_paging() {
		static bool pre_init=true;
		if(pre_init) {
			//to solve chicken of egg problem of needed free pages to implement
			//paging and needing paging to implement free page allocator, do a
			//to step init, first install a page handler that begins actual
			//paging init then actually handle the page fault
			hal::register_int(14,&pre_init_handle,NON_REENTRANT,false);
			pre_init=false;
			return true;
		} else {
			hal::register_int(14,&handle,NON_REENTRANT|NO_SCHEDULER,false);
			p.init(x86_64::get_free_page);
			init=true;
			return true;
		}
	}
	bool paging_ready() {
		return init;
	}
	void pre_init_handle(cpu_state *s) {
		if(!init) {
			hal::print_boot_msg("Init paging",init_paging(),true);
		}
		//cpu will resume executing, causing another page fault that will be
		//handled by the real page fault handler
	}
	void handle(cpu_state *s) {
		//for now assume page fault was caused by an unmapped page in kernel
		p.map(x86_64::get_free_page(), get_creg(s,2), {true,false,true});
	}
	void invlpage(uintptr_t addr) {
		asm volatile("invlpg (%0)" ::"r"(addr) : "memory");
	}
	void recursive_paging::init(uintptr_t (*a_func)()) {
		this->alloc_func=a_func;
		//4GiB - 4Mib
		this->pmap=reinterpret_cast<page_map *>(0xFFC00000);
	}
	void recursive_paging::map(uintptr_t phys,uintptr_t virt,hal::page_opt options) {
		if(virt>=0xFFC00000) {
			kernel::panic("page fault inside pmap");
		}
		//page align so even if someone is lazy(CR2) this works
		virt&=~0xFFF;
		phys&=~0xFFF;
		/*
		    +----------+----------+--------+
		    | 31 - 22  | 21 - 12  | 11 - 0 |
		    +----------+----------+--------+
		    | page dir | page tbl | offset |
		    +----------+----------+--------+
		*/
		uint16_t tbl=(virt>>12)&0x3FF;
		uint16_t dir=(virt>>22)&0x3FF;
		if(!pmap->master.entries[dir].present) {
			//page dir dosent exist, create it
			pmap->master.entriesi[dir]=alloc_func()|0x3;
			invlpage((uintptr_t)&pmap->tables[dir]);
			memset(&pmap->tables[dir],0,0x1000);
		}
		pmap->tables[dir].entriesi[tbl]=phys|0x1;
		pmap->tables[dir].entries[tbl].user=options.user;
		pmap->tables[dir].entries[tbl].read_write=options.write;
		//exec not supported on pure x86 non PAE paging
		invlpage(virt);
	}
	uintptr_t recursive_paging::unmap(uintptr_t virt) {
		virt&=~0xFFF;
		uint16_t tbl=(virt>>12)&0x3FF;
		uint16_t dir=(virt>>22)&0x3FF;
		if(!pmap->master.entries[dir].present) {
			return 0x1;
		}
		pmap->tables[dir].entries[tbl].present=false;
		invlpage(virt);
		return pmap->tables[dir].entriesi[tbl]&~0xFFF;
	}
	uintptr_t recursive_paging::locate(uintptr_t virt) {
		virt&=~0xFFF;
		uint16_t tbl=(virt>>12)&0x3FF;
		uint16_t dir=(virt>>22)&0x3FF;
		if(!pmap->master.entries[dir].present) {
			return 0x1;
		}
		return pmap->tables[dir].entriesi[tbl]&~0xFFF;
	}

}
namespace hal {
	void map_free_to_virt_cur(uintptr_t virt, page_opt options) {
		x86::p.map(x86_64::get_free_page(),virt,options);
	}
	void map_phys_to_virt_cur(uintptr_t virt, uint64_t phys, page_opt options) {
		x86::p.map(phys,virt,options);
	}
	void unmap_virt_cur(uintptr_t virt) {
		uintptr_t phys=x86::p.unmap(virt);
		if(phys!=0x1) {
			x86_64::add_free_page(phys);
		}
	}
	uintptr_t unmap_virt_phys_cur(uintptr_t virt) {
		return x86::p.unmap(virt);
	}
	uintptr_t phys_from_virt(uintptr_t virt) {
		return x86::p.locate(virt);
	}
}
#endif
