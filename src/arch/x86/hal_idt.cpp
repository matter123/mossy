/*
    Copyright 2013-2014 Matthew Fosdick

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
#include <x86_64/int.h>
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

extern "C" void lidt(x86::IDTR *idtr);
bool in_int;
bool *in_interrupt=&in_int;
uint32_t c_int;
uint32_t *interrupt_count=&c_int;

bool IS_SET(uint8_t arr[],uint index) {
	return (arr[index/8]&(1<<index%8))==(1<<index%8);
}

namespace hal {
	x86::IDTR idtr;
	x86::IDT idt;
	uint8_t used[32];
	uint8_t resv[32];
#define SET(a,i) do {a[i/8]|=(1<<i%8);}while(0)
#define UN_SET(a,i) do {a[i/8]&=~(1<<i%8);}while(0)
	bool init_idt() {
		memset(static_cast<void *>(&idt),0,sizeof(idt));
		idtr.base=&idt;
		idtr.limit=static_cast<uint16_t>(sizeof(idt));
		lidt(&idtr);
		for(int i=0; i<256; i++) {
			register_asm_sub_int(i,(uintptr_t)exc_arr[i],NON_REENTRANT,false);
		}
		//allow exceptions to be called again
		for(int i=0; i<256; i++) {
			UN_SET(used,i);
		}
		return true;
	}
	uint16_t get_max_interrupt() {
		return 255;
	}

	int_callback callbacks[256];
	void register_int(uint16_t int_num,int_callback callback,int type,bool user) {
		register_asm_sub_int(int_num,(uintptr_t)exc_arr[int_num],type,user);
		hal::cout<<"registering interrupt "<<hal::dec<<int_num<<" at "<<hal::address<<
		         (uintptr_t)callback<<endl;
		callbacks[int_num]=callback;
		x86::trampoline *tramp=(x86::trampoline *)exc_arr[int_num];
		if(tramp->begin[0]==0x6A&&tramp->begin[1]==0x00&&tramp->begin[2]==0x6A) {
			tramp=(x86::trampoline *)((pointer)exc_arr[int_num]+2);
		}
		int32_t diff=(((uintptr_t)callback)-tramp->abs_jmp);
		//reset the rel_jmp and abs_jmp
		tramp->rel_jmp+=diff;
		tramp->abs_jmp=((uintptr_t)callback);
	}

	void register_asm_sub_int(uint16_t int_num, uintptr_t addr, int type, bool user) {
		hal::cout<<"registering interrupt "<<hal::dec<<int_num<<" at "<<hal::address<<addr<<endl;
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
		ent.code_segment=0x8;
		ent.offset_low=static_cast<uint16_t>(addr&0xFFFF);
		ent.offset_med=static_cast<uint16_t>((addr>>16)&0xFFFF);
		idt.entries[int_num]=ent;
		SET(used,int_num);
	}

	idt_range reserve_range(uint16_t count, uint16_t min_count, bool aligned) {
		idt_range r;
		r.start=0;
		r.len=0;
		size_t i=32;
		size_t j=0;
		while(i<(256-count)) {
			j=0;
			while(j<count) {
				if(IS_SET(used,i+j)) {
					if(j>=min_count) {
						goto exit_t;
					}
					break;
				}
				j++;
			}
			if(j==count) {
				goto exit_t;
			}
			i+=(aligned?count:1);
		}
		return r;
	exit_t:
		r.start=i;
		r.len=j;
		return r;
	}

	bool release_range(idt_range range) {
		return false;
	}

	void wait_for_int() {
		asm volatile("hlt");
	}
	void call_handle(cpu_state *state) {
		if(!IS_SET(used,get_int_num(state))) {
			dump_regs(state);
			halt(true);
		} else {
			callbacks[get_int_num(state)](state);
		}
	}

	void fail_fast(cpu_state *state) {
		kernel::panic("fail fast");
	}
}
#endif
