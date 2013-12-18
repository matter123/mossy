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

#ifndef __PIC_H
#define __PIC_H
#include <stdint.h>
namespace x86 {
	struct stack_t{
		uint32_t ds;
		uint32_t edi;
		uint32_t esi;
		uint32_t ebp;
		uint32_t ebx;
		uint32_t edx;
		uint32_t ecx;
		uint32_t eax;
		uint32_t pic;
		uint32_t eip;
		uint32_t cs;
		uint32_t eflags;
		uint32_t useresp;
		uint32_t ss;
	};

	typedef void (*pic_handle_t)(stack_t s);
	void install_pic_idt();

	void init_pic();

	void mask(int intrpt);

	void unmask(int intrpt);

	void request_irq(int irq,pic_handle_t handler);
}
#endif
