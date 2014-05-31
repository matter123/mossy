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
#include <arch.h>
#ifdef X86
#include "hhalf.h"
#include <stdint.h>
uint32_t pagedira[1024] __attribute__((aligned(4096)));
uint32_t pagetablea[1024] __attribute__((aligned(4096)));
uint32_t pagetablean[1024] __attribute__((aligned(4096)));


struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uintptr_t base;
} __attribute__((packed));

struct gdt_entry gdt[5];
struct gdt_ptr gp;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access,
                  uint8_t gran) {
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

extern "C" void gdt_flush();

void init_higher_half() {
	//enable temporay paging
	//only maps first 4mb
	//if the kernel becomes bigger than that,
	// we have a serious problem
	uintptr_t pagedir, pagetable=0;
	pagedir=(uintptr_t)pagedira;
	pagetable=(uintptr_t)pagetablea;
	pagedir-=HIGH_HALF_BASE_ADDR;
	pagetable-=HIGH_HALF_BASE_ADDR;
	for(int i=0; i<1024; i++) {
		pagedira[i]=0;
		pagetablea[i]=(i*0x1000)|0x3;
	}
	pagedira[0]=pagetable|0x3;
	//768
	pagedira[HIGH_HALF_BASE_ADDR>>22]=pagetable|0x3;
	pagedira[1022]=pagedir|0x3;//set up recursive paging 4mb below normal
	asm volatile("mov %0, %%eax\n"
	             "mov %%eax, %%cr3\n"
	             "mov %%cr0, %%eax\n"
	             "orl $0x80000000, %%eax\n"
	             "mov %%eax, %%cr0\n" :: "m"(pagedir));
	asm volatile("mov %%cr4, %%eax\n"
	             "orl $0x80, %%eax\n"
	             "mov %%eax, %%cr4"::);
	//disables funky gdt
	gp.limit = (sizeof(struct gdt_entry) * 5) - 1;
	gp.base = (uintptr_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0); // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // (User) Code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // (User) Data segment
	gdt_flush();
}


void *em_page_max() {
	return reinterpret_cast<void *>(HIGH_HALF_BASE_ADDR+4*1024*1024);
}
void em_page(void *virt,void *phys) {
	uintptr_t pagetablen=(uintptr_t)pagetablean;
	pagetablen-=HIGH_HALF_BASE_ADDR;
	uintptr_t v=reinterpret_cast<uintptr_t>(virt);
	uintptr_t v1=v<<10;
	v1=v1>>22;
	pagetablean[v1]=(reinterpret_cast<uintptr_t>(phys))|0x3;
	pagedira[v>>22]=pagetablen|0x3;
	asm volatile("invlpg (%0)" ::"r"(virt) : "memory");
}
#endif