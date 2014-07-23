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
#ifdef X64
#include <x86_64/pfa.h>
#include "paging.h"
#include <hal/paging.h>
#include <hal/int.h>
#include <hal/hal.h>
#include <hal/console.h>
#include <string.h>
#include "../../panic.h"
namespace x64 {
	static bool pre_init_handle(int level, cpu_state *s);
	static bool handle(int level, cpu_state *s);
	static bool init=false;
	static recursive_paging p;
	bool init_paging() {
		static bool pre_paging=true;
		if(pre_paging) {
			//chicken-egg solving
			hal::register_int(14,&pre_init_handle,hal::NON_REENTRANT,false);
			pre_paging=false;
			return true;
		}
		p.init(x86_64::get_free_page);
		hal::register_int(14,&handle,hal::NON_REENTRANT,false);
		init=true;
		return true;
	}
	bool paging_ready() {
		return init;
	}
	bool pre_init_handle(int level, cpu_state *s) {
		hal::print_boot_msg("Init paging",init_paging(),true);
		return true;
		//cpu will resume executing, causing another page fault that will be
		//handled by the real page fault handler
	}
	bool handle(int level, cpu_state *s) {
		//for now assume page fault was caused by an unmapped page in kernel
		p.map(x86_64::get_free_page(), get_creg(s,2), {true,false,true});
		return true;
	}
	void invlpage(uintptr_t addr) {
		asm volatile("invlpg (%0)" ::"r"(addr) : "memory");
	}
	void recursive_paging::init(uintptr_t (*a_func)()) {
		this->base=PML4_OFF(510)+0xFFFF000000000000/*sign extend*/;
		this->alloc_func=a_func;
	}
	void recursive_paging::map(uintptr_t phys,uintptr_t virt,hal::page_opt options) {
		//page align so even if someone is lazy(CR2) this works
		virt&=~0xFFF;
		phys&=~0xFFF;
		/*
		    LAYOUT
		    +----------------+------------------+----------+----------+--------+
		    |    47 - 39     |     38 - 30      | 29 - 21  | 20 - 12  | 11 - 0 |
		    +----------------+------------------+----------+----------+--------+
		    | page map lvl 4 | page dir ptr tbl | page dir | page tbl | offset |
		    +----------------+------------------+----------+----------+--------+
		*/
		uint16_t pml4i=(virt>>39)&0x1FF;
		uint16_t pdpti=(virt>>30)&0x1FF;
		uint16_t pdi=(virt>>21)&0x1FF;
		uint16_t pti=(virt>>12)&0x1FF;
		//bad thaings can happen if we handle our own page faults
		if(pml4i==510) {
			kernel::panic("page fault inside pmap");
		}
		//if any intermediate tables don't exist, create them
		pml4 *PML4=((pml4 *)(base+PDPT_OFF(510)+PD_OFF(510)+PT_OFF(510)));
		pdpt *PDPT=((pdpt *)(base+PDPT_OFF(510)+PD_OFF(510)+PT_OFF(pml4i)));
		pd *PD=((pd *)(base+PDPT_OFF(510)+PD_OFF(pml4i)+PT_OFF(pdpti)));
		pt *PT=((pt *)(base+PDPT_OFF(pml4i)+PD_OFF(pdpti)+PT_OFF(pdi)));
		if(!PML4->entries[pml4i].present) {
			PML4->entriesi[pml4i]=(alloc_func()|0x1)&0x0000FFFFFFFFFFFF/*only set first 48 bits*/;
			invlpage((uintptr_t)PDPT);
			memset(PDPT,0,0x1000);
		}
		if(!PDPT->entries[pdpti].present) {
			PDPT->entriesi[pdpti]=(alloc_func()|0x1)&0x0000FFFFFFFFFFFF;
			invlpage((uintptr_t)PD);
			memset(PD,0,0x1000);
		}
		if(!PD->entries[pdi].present) {
			PD->entriesi[pdi]=(alloc_func()|0x1)&0x0000FFFFFFFFFFFF;
			invlpage((uintptr_t)PT);
			memset(PT,0,0x1000);
		}
		//finally map with permissions
		PT->entriesi[pti]=(phys|0x1)&0x0000FFFFFFFFFFFF;
		PT->entries[pti].user=options.user;
		PT->entries[pti].read_write=options.write;
		PT->entries[pti].nx=!options.exec;
		invlpage(virt);
	}
	uintptr_t recursive_paging::unmap(uintptr_t virt) {
		virt&=~0xFFF;
		uint16_t pml4i=(virt>>39)&0x1FF;
		uint16_t pdpti=(virt>>30)&0x1FF;
		uint16_t pdi=(virt>>21)&0x1FF;
		uint16_t pti=(virt>>12)&0x1FF;
		//bad thaings can happen if we handle our own page faults
		if(pml4i==510) {
			kernel::panic("page fault inside pmap");
		}
		//if any intermediate tables don't exist, return 0x1
		pml4 *PML4=((pml4 *)(base+PDPT_OFF(510)+PD_OFF(510)+PT_OFF(510)));
		pdpt *PDPT=((pdpt *)(base+PDPT_OFF(510)+PD_OFF(510)+PT_OFF(pml4i)));
		pd *PD=((pd *)(base+PDPT_OFF(510)+PD_OFF(pml4i)+PT_OFF(pdpti)));
		pt *PT=((pt *)(base+PDPT_OFF(pml4i)+PD_OFF(pdpti)+PT_OFF(pdi)));
		if(!PML4->entries[pml4i].present) {
			return 0x1;
		}
		if(!PDPT->entries[pdpti].present) {
			return 0x1;
		}
		if(!PD->entries[pdi].present) {
			return 0x1;
		}
		uintptr_t phys=PT->entriesi[pti]&0x0000FFFFFFFFF000;
		PT->entries[pti].present=false;
		invlpage(virt);
		return phys;
	}
	uintptr_t recursive_paging::locate(uintptr_t virt) {
		virt&=~0xFFF;
		uint16_t pml4i=(virt>>39)&0x1FF;
		uint16_t pdpti=(virt>>30)&0x1FF;
		uint16_t pdi=(virt>>21)&0x1FF;
		uint16_t pti=(virt>>12)&0x1FF;
		//bad thaings can happen if we handle our own page faults
		if(pml4i==510) {
			kernel::panic("page fault inside pmap");
		}
		//if any intermediate tables don't exist, return 0x1
		pml4 *PML4=((pml4 *)(base+PDPT_OFF(510)+PD_OFF(510)+PT_OFF(510)));
		pdpt *PDPT=((pdpt *)(base+PDPT_OFF(510)+PD_OFF(510)+PT_OFF(pml4i)));
		pd *PD=((pd *)(base+PDPT_OFF(510)+PD_OFF(pml4i)+PT_OFF(pdpti)));
		pt *PT=((pt *)(base+PDPT_OFF(pml4i)+PD_OFF(pdpti)+PT_OFF(pdi)));
		if(!PML4->entries[pml4i].present) {
			return 0x1;
		}
		if(!PDPT->entries[pdpti].present) {
			return 0x1;
		}
		if(!PD->entries[pdi].present) {
			return 0x1;
		}
		return PT->entriesi[pti]&0x0000FFFFFFFFF000;
	}
}
namespace hal {
	void map_free_to_virt_cur(uintptr_t virt, page_opt options) {
		x64::p.map(x86_64::get_free_page(),virt,options);
	}
	void map_phys_to_virt_cur(uintptr_t virt, uint64_t phys, page_opt options) {
		x64::p.map(phys,virt,options);
	}
	void unmap_virt_cur(uintptr_t virt) {
		x86_64::add_free_page(x64::p.unmap(virt));
	}
	uintptr_t unmap_virt_phys_cur(uintptr_t virt) {
		return x64::p.unmap(virt);
	}
	uintptr_t phys_from_virt(uintptr_t virt) {
		return x64::p.locate(virt);
	}
}
#endif