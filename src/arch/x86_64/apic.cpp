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
#ifdef X86_64
#include <stdint.h>
#include <hal/int.h>
#include "cpu_stuff.h"
namespace x86_64 {
	bool _pres;
	uintptr_t APIC_BASE;
	bool present() {
		if(!_pres) {
			_pres=(cpuid(0x1,3)&(1<<9))==(1<<9);
		}
		return _pres;
	}

	uintptr_t get_base() {
		if(!APIC_BASE) {
			uint32_t eax=cpuid(0x1,0);
			uint family=(eax>>8)&0x15;
			APIC_BASE=0xFEE00000
			if(family>5) {
				uint64_t addr=rdmsr(0x1B);
				addr|=0x800;
				wrmsr(0x1B,addr)
				APIC_BASE=rdmsr(0x1B)
			}
		}
		return APIC_BASE;

	}
	bool timer_init(uint32_t freq) {
		if(present()) {
			init_apic_timer(freq);
		} else {
			init_pit(freq);
		}
	}
	bool enable_lapic() {
		if(present()) {
			get_base(); //enable if disable

		}
		outb(0xF0,inb(0xF0)|0x100); //switch from legacy to lapic
		return _pres;
	}
	blank_isr() {
		*(APIC_BASE+)
	}
	spurious_isr() {
	}
	void init_apic_timer(uint32_t freq) {

	}
	uintreg_t bus_freq;
	uintreg_t get_busfreq() {

	}
}
#endif