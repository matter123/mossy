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
#include <stddef.h>
#include <struct.h>
#include <string.h>
#include <hal/int.h>
 namespace x64 {
	struct idt_entry {
		uint16_t offset_low;
		uint16_t code_segment;
		int IST:3;
		int resv:5;
		int type:4;
		int zero:1;
		int DPL:2;
		bool present:1;
		uint16_t offset_med;
		uint32_t offset_high;
		uint32_t resv_1;
	}PACKED;

	struct IDT {
		idt_entry entries[256];
	}PACKED;

	struct IDTR {
		uint16_t limit;
		IDT *base;
	}PACKED;
}

extern "C" void lidt(x64::IDTR *idtr);
namespace hal {
	x64::IDTR idtr;
	x64::IDT idt;
	uint8_t used[32];
	uint8_t resv[32];
	#define IS_SET(a,i) ((a[i/8]&(1<<i%8))==(1<<i%8))
	#define SET(a,i) do {a[i/8]|=(1<<i%8);}while(0)
	#define UN_SET(a,i) do {a[i/8]&=~(1<<i%8);}while(0)
	void init_idt() {
		std::memset(static_cast<void *>(&idt),0,sizeof(idt));
		idtr.base=&idt;
		idtr.limit=static_cast<uint16_t>(sizeof(idt));
		lidt(&idtr);
	}

	uint16_t get_max_interrupt() {
		return 255;
	}

	void register_only_int(uint16_t int_num,int_callback callback,hal::interrupt_type type,bool user) {

	}

	void register_chain_int(uint16_t int_num,int_callback callback,hal::interrupt_type type,bool user) {
		return; //don't really want to work on this right now
	}

	void register_asm_sub_int(uint16_t int_num, uintptr_t addr, interrupt_type type,bool user) {

	}

	idt_range reserve_range(uint16_t count, uint16_t min_count, bool aligned) {
		idt_range r;
		r.start=0;
		r.len=0;
		return r;
	}

	bool release_range(idt_range range) {
		return false;
	}

	void wait_for_int() {
		asm volatile ("hlt");
	}
}
#endif