/*
	Copyright 2015 Matthew Fosdick

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
#define INT_CALL_GATE 0xC
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

struct def_interrupt {
//	bool (*default_interrupt)(cpu_state *);
	def_interrupt * next;
};

void install_interrupts();
