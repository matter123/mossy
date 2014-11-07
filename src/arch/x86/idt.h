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
#pragma once
#include <arch.h>
#ifdef X86
#include <stdint.h>
namespace hal {
	bool init_idt();
}
namespace x86 {
	struct idt_entry {
		uint16_t offset_low;
		uint16_t code_segment;
		uint8_t resv;
		uint type:4;
		uint zero:1;
		uint DPL:2;
		bool present:1;
		uint16_t offset_med;
	} PACKED;

	struct IDT {
		idt_entry entries[256];
	} PACKED;

	struct IDTR {
		uint16_t limit;
		IDT *base;
	} PACKED;

	struct trampoline {
		char begin[3];
		#ifdef DEBUG
		char *id;
		#else
		char id;
		#endif
		char jmp_byte;
		int32_t rel_jmp;
		uint32_t abs_jmp;
	} PACKED;
}
#endif