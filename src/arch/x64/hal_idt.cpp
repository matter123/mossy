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
#include <struct.h>
#include "../../hal/int.h"

namespace x64 {
	struct idt_entry {
		uint16_t offset_low;
		uint16_t code_segment;
		int IST:3;
		int resv:5;
		int type:4;
		int zero:1;
		int DPL:3;
		bool present:1;
		uint16_t offset_med;
		uint32_t offset_high;
		uint32_t resv_1;
	}PACKED;

	struct IDT {
		idt_entry entries[256];
	}PACKED;

	struct IDTR {
		IDT *base;
		uint16_t limit;
	}PACKED;

	IDTR idtr;
	IDT idt;
	uint8_t used[32];
	uint8_t resv[32];
	#define IS_SET(a,i) (a[i/8]&(1<<i%8)==(1<<i%8))
	#define SET(a,i) do {a[i/8]|=(1<<i%8);}while(0)
	#define UN_SET(a,i) do {a[i/8]&=~(1<<i%8);}while(0)
	void init_IDT() {
	}

	uint16_t hal::get_max_interrupt() {
		return 255;
	}

	void hal::register_only_int(uint16_t int_num,hal::int_callback callback,hal::interrupt_type type,bool user) {

	}

	void hal::register_chain_int(uint16_t int_num,hal::int_callback callback,hal::interrupt_type type,bool user) {
		return; //don't really want to work on this right now
	}

	void hal::register_asm_sub_int(uint16_t int_num,uintptr_t addr,hal::interrupt_type type,bool user) {
		//callee assumes all responsibility of handling the interrupt
		if(int_num>256||IS_SET(used,int_num))return;//fail silently, we cant fail loudly
		idt_entry ent=idt.entries[int_num];
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
		ent.IST=0;//don't really know what this does but AMD manual says zero is ignore
		ent.code_segment=0x8;//see boot64.inc for this magic number

		ent.offset_low=static_cast<uint16_t>(addr&0xFFFF);
		ent.offset_med=static_cast<uint16_t>((addr>>16)&0xFFFF);
		ent.offset_high=static_cast<uint32_t>(addr>>32);
		SET(used,int_num);
	}

	hal::idt_range reserve_range(uint16_t count, uint16_t min_count, bool aligned) {
		size_t i,j;
		for(i=32;i<256;i+=(aligned?count:1)) {
			for(j=0;j<count;j++) {
				if(!IS_SET(used,i+j)||!IS_SET(resv,i+j))continue;
				if(j<min_count)break;
				break exit_t;
			}
		}
		return static_cast<hal::idt_range>({0,0});
		exit_t:
		for(size_t k=0;k<j;k++)SET(resv,k);
		return return static_cast<hal::idt_range>({i,j});
	}

	bool release_range(hal::idt_range range) {
		return false;
	}

	void wait_for_int() {
		asm volatile ("hlt");
	}
}
#endif