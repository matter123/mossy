/*
	Copyright 2016 Matthew Fosdick

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
#define INT_INT_GATE  0xE
#define INT_TRAP_GATE 0xF

struct idt_entry {
	uint16_t target_low;
	uint16_t target_cs;
	uint8_t  ist:3;
	uint8_t  ign:5;
	uint8_t  type:4;
	uint8_t  zero:1;
	uint8_t  dpl:2;
	bool  present:1;
	uint16_t target_mid;
	uint32_t target_high;
	uint32_t ign2;
} PACKED;

void set_entry(int int_num,idt_entry *);
void set_entry(int int_num, void *addr,int type,bool userspace);

struct cpu_state {
	uint64_t unused;
	uint64_t cr4, cr3, cr2, cr0;
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rdi, rsi, rdx, rcx, rbx, rax, rbp;
	uint64_t fs, gs;
	uint64_t dbg, int_num, err_code;
	uint64_t rip, cs, rflags, rsp, ss;
};
struct def_interrupt {
	void *context;
	bool (*default_interrupt)(cpu_state *,void *sse_save,bool *in_use, void *context);
	def_interrupt * next;
};
void install_JT1(int int_num,void *target);
void install_JT2(int int_num,void *target);
void install_single_interrupt(int int_num,void (*default_interrupt)(cpu_state *,void *sse_save,bool *in_use));
void install_interrupt(int int_num,def_interrupt *);
void install_interrupts();
