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
#include "../arch.h"
#ifdef X64
#include <stdint.h>
#include "../../init/multiboot.h"
#include "idt.h"
namespace hal {
	void init_arch(kernel::multiboot_t *mboot) {
		//an okay gdt is already setup
		//so IDT
		x64::init_idt();
	}
	uintptr_t get_page_offset_addr() {
		return static_cast<uintptr_t>(0xFFFFFFFF80000000);
	}

	void enable_interrupts(){
		asm volatile("sti");
	}

	void disable_interrupts() {
		asm volatile("cli");
	}

	void halt(bool inter) {
		if(inter)disable_interrupts();
		while(1) {
			asm("hlt");
		}
	}
}
#endif