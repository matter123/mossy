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

#include "exceptions.h"
#include "idt.h"
#include "../../monitor.h"

extern void * exception0;
extern void * exception1;
extern void * exception2;
extern void * exception3;
extern void * exception4;
extern void * exception5;
extern void * exception6;
extern void * exception7;
extern void * exception8;
extern void * exception9;
extern void * exception10;
extern void * exception11;
extern void * exception12;
extern void * exception13;
extern void * exception14;
extern void * exception15;
extern void * exception16;
extern void * exception17;
extern void * exception18;
extern void * exception19;
extern void * exception20;
extern void * exception21;
extern void * exception22;
extern void * exception23;
extern void * exception24;
extern void * exception25;
extern void * exception26;
extern void * exception27;
extern void * exception28;
extern void * exception29;
extern void * exception30;
extern void * exception31;
namespace x86 {
	void init_exceptions() {
		install_idt(&exception0,0,IDT_TYPE_INTRPT,0);
		install_idt(&exception1,1,IDT_TYPE_INTRPT,0);
		install_idt(&exception2,2,IDT_TYPE_INTRPT,0);
		install_idt(&exception3,3,IDT_TYPE_INTRPT,0);
		install_idt(&exception4,4,IDT_TYPE_INTRPT,0);
		install_idt(&exception5,5,IDT_TYPE_INTRPT,0);
		install_idt(&exception6,6,IDT_TYPE_INTRPT,0);
		install_idt(&exception7,6,IDT_TYPE_INTRPT,0);
		install_idt(&exception8,8,IDT_TYPE_INTRPT,0);
		install_idt(&exception9,9,IDT_TYPE_INTRPT,0);
		install_idt(&exception10,10,IDT_TYPE_INTRPT,0);
		install_idt(&exception11,11,IDT_TYPE_INTRPT,0);
		install_idt(&exception12,12,IDT_TYPE_INTRPT,0);
		install_idt(&exception13,13,IDT_TYPE_INTRPT,0);
		install_idt(&exception14,14,IDT_TYPE_INTRPT,0);
		install_idt(&exception15,15,IDT_TYPE_INTRPT,0);

		install_idt(&exception16,16,IDT_TYPE_INTRPT,0);
		install_idt(&exception17,17,IDT_TYPE_INTRPT,0);
		install_idt(&exception18,18,IDT_TYPE_INTRPT,0);
		install_idt(&exception19,19,IDT_TYPE_INTRPT,0);
		install_idt(&exception20,20,IDT_TYPE_INTRPT,0);
		install_idt(&exception21,21,IDT_TYPE_INTRPT,0);
		install_idt(&exception22,22,IDT_TYPE_INTRPT,0);
		install_idt(&exception23,23,IDT_TYPE_INTRPT,0);
		install_idt(&exception24,24,IDT_TYPE_INTRPT,0);
		install_idt(&exception25,25,IDT_TYPE_INTRPT,0);
		install_idt(&exception26,26,IDT_TYPE_INTRPT,0);
		install_idt(&exception27,27,IDT_TYPE_INTRPT,0);
		install_idt(&exception28,28,IDT_TYPE_INTRPT,0);
		install_idt(&exception29,29,IDT_TYPE_INTRPT,0);
		install_idt(&exception30,30,IDT_TYPE_INTRPT,0);
		install_idt(&exception31,31,IDT_TYPE_INTRPT,0);
	}
	extern "C" void except_handler(exc_stack_t s) {
		//print("exception");
		uint32_t code;
		asm volatile("mov %%cr2, %0":"=r"(code));
		std::cout<<std::hex<<"exception"<<s.ext<<" "<<s.eip<<std::endl;
	}

	extern "C" void resv_except_handler() {
		//print("reserved exception oh no");
		asm("cli\nhlt");
	}
}
