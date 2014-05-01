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
#include <stdint.h>
#include "../../init/multiboot.h"
#include <x86/idt.h>
#include <x86/hhalf.h>
namespace hal {
	void init_arch(kernel::multiboot_t *mboot) {
		//the 32 bit kernel boots in higher half with a gdt trick
		//this setups up paging and a proper gdt
		init_higher_half();
		//once we have a stable gdt and higheer half page setup we do the rest of system startup
		init_idt();
	}
	uintptr_t get_page_offset_addr() {
		return static_cast<uintptr_t>(HIGH_HALF_BASE_ADDR);
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
#endif