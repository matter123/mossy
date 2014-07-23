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
#include <stdint.h>
#include <hal/multiboot.h>
#include <x64/idt.h>
#include <x86_64/pfa.h>
#include <x64/paging.h>
#include <hal/console.h>
namespace hal {
	void init_arch() {
		//an okay gdt is already setup
		//so IDT
		print_boot_msg("Init IDT",init_idt(),true);
		print_boot_msg("Init pre-paging",x64::init_paging(),true);
		print_boot_msg("Init PFA",x86_64::init_pfa(),true);
		if(!x64::paging_ready()) {
			print_boot_msg("Init paging",x64::init_paging(),true);
		}
	}
	void magic_break() {
		asm volatile("xchg %bx, %bx");
	}
	uintptr_t get_page_offset_addr() {
		return static_cast<uintptr_t>(0xFFFFFFFF80000000);
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
	void add_special_mem_arch() {
		//i dont know anything
	}
}
uint8_t get_reg_count() {
	return 14;
}
uint8_t get_creg_count() {
	return 5;
}
uintreg_t get_reg(cpu_state *s, uint8_t reg) {
	switch(reg) {
		case 0x0:
			return s->rax;
		case 0x1:
			return s->rbx;
		case 0x2:
			return s->rcx;
		case 0x3:
			return s->rdx;
		case 0x4:
			return s->rsi;
		case 0x5:
			return s->rdi;
		case 0x6:
			return s->r8;
		case 0x7:
			return s->r9;
		case 0x8:
			return s->r10;
		case 0x9:
			return s->r11;
		case 0xA:
			return s->r12;
		case 0xB:
			return s->r13;
		case 0xC:
			return s->r14;
		case 0xD:
			return s->r15;
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
#endif