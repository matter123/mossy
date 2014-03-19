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

#include "idt.h"
#include <stdint.h>
#include <struct.h>
namespace x86 {
	struct IDTdesc {
		uint16_t offset_low;
		uint16_t selector;
		uint8_t  zero;
		int      type:4;
		int      S:1;
		int      DPL:2;
		bool     present:1;
		uint16_t offset_high;
	}PACKED;
	IDTdesc IDT[256];

	void install_idt(void *handlerv, int number, int type, int min_ring) {
		uintptr_t handler=reinterpret_cast<uintptr_t>(handlerv);
		IDT[number].zero=0;
		IDT[number].S=0;
		IDT[number].DPL=min_ring;
		IDT[number].type=type;
		IDT[number].present=true;
		IDT[number].selector=0x08;//offset into GDT, 0x08 for kernel
		IDT[number].offset_low=static_cast<uint16_t>(handler&0xFFFF);
		IDT[number].offset_high=static_cast<uint16_t>((handler>>16)&0xFFFF);
	}

	struct LIDT {
		uint16_t len;
		void *base;
	}PACKED;
}
x86::LIDT idtp;
namespace x86 {
	extern "C" void IDT_flush();

	void enable_idt() {
		idtp.len=(256*8)-1;
		idtp.base=&IDT;
		IDT_flush();
	}
}
