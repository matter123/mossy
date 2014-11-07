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
#include <x86/idt.h>

extern "C" void lidt(x86::IDTR *idtr);
extern "C" void exc_stub_sched();


namespace hal {
		bool IS_SET(uint8_t arr[],unsigned int index) {
			return (arr[index/8]&(1<<index%8))==(1<<index%8);
		}
#define SET(a,i)    do {a[(i)/8]|= (1<<((i)%8));}while(0)
#define UN_SET(a,i) do {a[(i)/8]&=~(1<<((i)%8));}while(0)

	x86::IDTR idtr;
	x86::IDT idt;
	uint8_t used[32];
	uint8_t resv[32];
	bool init_idt() {
		memset(static_cast<void *>(&idt),0,sizeof(idt));
		idtr.base=&idt;
		idtr.limit=static_cast<uint16_t>(sizeof(idt));
		lidt(&idtr);
		for(int i=0; i<256; i++) {
			register_int(i,&dump_regs,NON_REENTRANT,false);
		}
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
		register_stub_int(int_num,(uintptr_t)&exc_stub_sched,type,user);
		callbacks[int_num]=callback;
	}
	void register_stub_int(uint16_t int_num,uintptr_t addr,int type,bool user) {
		//re-register so type,user, and interrupt addr match
		register_asm_stub_int(int_num,(uintptr_t)exc_arr[int_num],type,user);
		//patchup the trampoline code
		x86::trampoline *tramp=(x86::trampoline *)exc_arr[int_num];
		if(tramp->begin[0]==0x6A&&tramp->begin[1]==0x00&&tramp->begin[2]==0x6A) {
			tramp=(x86::trampoline *)((pointer)exc_arr[int_num]+2);
		}
		int32_t diff=(addr-tramp->abs_jmp);
		tramp->rel_jmp+=diff;
		tramp->abs_jmp=addr;
	}

	void register_asm_stub_int(uint16_t int_num, uintptr_t addr, int type, bool user) {
		if(int_num>256) {
			return;
		}
		x86::idt_entry ent=idt.entries[int_num];
		ent.DPL=(user?3:0);
		switch(type&0xF) {
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
		for(int i=0;i<range.len;i++) {
			UN_SET(resv,range.start+i);
		}
		return false;
	}

	void wait_for_int() {
		asm volatile("hlt");
	}
	void call_handle(cpu_state *state) {
		callbacks[get_int_num(state)](state);
	}

	void fail_fast(cpu_state *state) {
		kernel::panic("fail fast");
	}
}
#endif
