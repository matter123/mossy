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
#include <stdint.h>
#include <hal/multiboot.h>
#include <x86/idt.h>
#include <hal/console.h>
#include <x86_64/pfa.h>
#include <hal/mmap.h>
#include "paging.h"

namespace hal {
	void init_arch() {
		print_boot_msg("Init IDT",init_idt(),true);
		print_boot_msg("Init pre-paging",x86::init_paging(),true);
		print_boot_msg("Init PFA",x86_64::init_pfa(),true);
		print_boot_msg("Init VMMAP",init_virt_mem(),true);
		if(!x86::paging_ready()) {
			print_boot_msg("Init paging",x86::init_paging(),true);
		}
	}
	void magic_break() {
		asm volatile("xchg %bx, %bx");
	}
	uintptr_t get_page_offset_addr() {
		return static_cast<uintptr_t>(0xC0000000);
	}

	void enable_interrupts() {
		asm volatile("sti");
	}

	void disable_interrupts() {
		asm volatile("cli");
	}

	void halt(bool inter) {
		if(inter) {
			disable_interrupts();
		}
		while(1) {
			asm("hlt");
		}
	}
	static mem_type types[6];
	void add_virt_mem_arch() {
		types[0].userspace=true;
		types[1].paging_struct=true;
		types[2].heap=true;
		types[3].kthread_stacks=true;
		types[4].firmware=true;
		types[5].videobuffer=true;
		add_virt_region(6);
		add_virt_region(0x0,0xB0FFFFFF,types[0]);
		add_virt_region(0xB1000000,0x3FFFFF,types[4]);
		add_virt_region(0xB1400000,0xFD2000,types[5]);
		add_virt_region(0xFFC00000,0x3FFFFF,types[1]);
		uint32_t pfa_end=(x86_64::get_end()&(0xFFF==0)?x86_64::get_end():(x86_64::get_end()&~
		                  (0xFFF))+0x1000);
		add_virt_region(pfa_end,0x7FFFFFF,types[3]);
		hal::cout<<hal::address<<(pfa_end+0x8000000);
		add_virt_region(pfa_end+0x8000000,0xFFBFFFFF-(pfa_end+0x8000000),types[2]);
	}
	void add_phys_mem_arch() {
		//i dont know anything
	}
}
uint8_t get_reg_count() {
	return 6;//eax, ebx, ecx, edx, esi, edi
}
uint8_t get_creg_count() {
	return 5; //cr0 ,cr1(0), cr2, cr3, cr4
}
uintreg_t get_reg(cpu_state *s, uint8_t reg) {
	switch(reg) {
		case 0x0:
			return s->eax;
		case 0x1:
			return s->ebx;
		case 0x2:
			return s->ecx;
		case 0x3:
			return s->edx;
		case 0x4:
			return s->esi;
		case 0x5:
			return s->edi;
		default:
			return 0;
	}
}
uintreg_t get_creg(cpu_state *s, uint8_t creg) {
	switch(creg) {
		case 0x0:
			return s->cr0;
		case 0x2:
			return s->cr2;
		case 0x3:
			return s->cr3;
		case 0x4:
			return s->cr4;
		default:
			return 0;
	}
}
uint16_t get_int_num(cpu_state *s) {
	return s->int_num;
}
char *get_mnemonic(cpu_state *s) {
	return (char *)s->mnemonic;
}
uintreg_t get_err_code(cpu_state *s) {
	return s->code;
}
int get_byte_order() {
	return 1; //LSB
}
uintptr_t get_instruction_pointer(cpu_state *s) {
	return s->eip;
}
hal::stack_frame *get_frame(cpu_state *s) {
	return reinterpret_cast<hal::stack_frame *>(s->ebp);
}
#endif
