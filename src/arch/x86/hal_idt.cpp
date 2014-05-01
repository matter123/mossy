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
#include <stddef.h>
#include <struct.h>
#include <string.h>
#include <hal/int.h>
#include <hal/hal.h>
#include <hal/console.h>
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
}

extern "C" void lidt(x86::IDTR *idtr);

extern "C" void *exc0; extern "C" void *exc1; extern "C" void *exc2; extern "C" void *exc3;
extern "C" void *exc4; extern "C" void *exc5; extern "C" void *exc6; extern "C" void *exc7;
extern "C" void *exc8; extern "C" void *exc9; extern "C" void *exc10; extern "C" void *exc11;
extern "C" void *exc12; extern "C" void *exc13; extern "C" void *exc14; extern "C" void *exc15;
extern "C" void *exc16; extern "C" void *exc17; extern "C" void *exc18; extern "C" void *exc19;
extern "C" void *exc20; extern "C" void *exc21; extern "C" void *exc22; extern "C" void *exc23;
extern "C" void *exc24; extern "C" void *exc25; extern "C" void *exc26; extern "C" void *exc27;
extern "C" void *exc28; extern "C" void *exc29; extern "C" void *exc30; extern "C" void *exc31;

namespace hal {
	x86::IDTR idtr;
	x86::IDT idt;
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
		register_asm_sub_int(0,(uintptr_t)&exc0,NON_REENTRANT,false);
		register_asm_sub_int(1,(uintptr_t)&exc1,NON_REENTRANT,false);
		register_asm_sub_int(2,(uintptr_t)&exc2,NON_REENTRANT,false);
		register_asm_sub_int(3,(uintptr_t)&exc3,NON_REENTRANT,false);
		register_asm_sub_int(4,(uintptr_t)&exc4,NON_REENTRANT,false);
		register_asm_sub_int(5,(uintptr_t)&exc5,NON_REENTRANT,false);
		register_asm_sub_int(6,(uintptr_t)&exc6,NON_REENTRANT,false);
		register_asm_sub_int(7,(uintptr_t)&exc7,NON_REENTRANT,false);
		register_asm_sub_int(8,(uintptr_t)&exc8,NON_REENTRANT,false);
		register_asm_sub_int(9,(uintptr_t)&exc9,NON_REENTRANT,false);
		register_asm_sub_int(10,(uintptr_t)&exc10,NON_REENTRANT,false);
		register_asm_sub_int(11,(uintptr_t)&exc11,NON_REENTRANT,false);
		register_asm_sub_int(12,(uintptr_t)&exc12,NON_REENTRANT,false);
		register_asm_sub_int(13,(uintptr_t)&exc13,NON_REENTRANT,false);
		register_asm_sub_int(14,(uintptr_t)&exc14,NON_REENTRANT,false);
		register_asm_sub_int(15,(uintptr_t)&exc15,NON_REENTRANT,false);
		register_asm_sub_int(16,(uintptr_t)&exc16,NON_REENTRANT,false);
		register_asm_sub_int(17,(uintptr_t)&exc17,NON_REENTRANT,false);
		register_asm_sub_int(18,(uintptr_t)&exc18,NON_REENTRANT,false);
		register_asm_sub_int(19,(uintptr_t)&exc19,NON_REENTRANT,false);
		register_asm_sub_int(20,(uintptr_t)&exc20,NON_REENTRANT,false);
		register_asm_sub_int(21,(uintptr_t)&exc21,NON_REENTRANT,false);
		register_asm_sub_int(22,(uintptr_t)&exc22,NON_REENTRANT,false);
		register_asm_sub_int(23,(uintptr_t)&exc23,NON_REENTRANT,false);
		register_asm_sub_int(24,(uintptr_t)&exc24,NON_REENTRANT,false);
		register_asm_sub_int(25,(uintptr_t)&exc25,NON_REENTRANT,false);
		register_asm_sub_int(26,(uintptr_t)&exc26,NON_REENTRANT,false);
		register_asm_sub_int(27,(uintptr_t)&exc27,NON_REENTRANT,false);
		register_asm_sub_int(28,(uintptr_t)&exc28,NON_REENTRANT,false);
		register_asm_sub_int(29,(uintptr_t)&exc29,NON_REENTRANT,false);
		register_asm_sub_int(30,(uintptr_t)&exc30,NON_REENTRANT,false);
		register_asm_sub_int(31,(uintptr_t)&exc31,NON_REENTRANT,false);
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
		//callee assumes all responsibility of handling the interrupt
		if(int_num>256||IS_SET(used,int_num)) {
			return;
		}
		x86::idt_entry ent=idt.entries[int_num];
		ent.DPL=(user?3:0);
		switch(type) {
			case NON_REENTRANT:
				//interrupt gate
				ent.type=0xE;
				break;
			case REENTRANT:
				//trap gate
				ent.type=0xF;
				break;
		}
		ent.present=true;
		ent.code_segment=0x8;//see hhalf.cpp for this magic number

		ent.offset_low=static_cast<uint16_t>(addr&0xFFFF);
		ent.offset_med=static_cast<uint16_t>((addr>>16)&0xFFFF);
		idt.entries[int_num]=ent;
		SET(used,int_num);
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
		asm volatile("hlt");
	}
	void call_handle(cpu_state *state) {
		dump_regs(state);
		halt(true);
	}
}
#endif