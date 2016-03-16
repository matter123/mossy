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
#include <arch/int.h>
#include <sys/scheduler.h>
#include <hal/multiboot.h>
#include <hal/memmap.h>
#include <vga_text.h>
#include <stdlib.h>

cpu_state *task;
extern pointer sys_stack2;
void task_b() {
	putc('B');
	yield();
	putc('b');
	asm("xchg %bx, %bx");
	yield();
}
void foo(cpu_state *s, void *sse_save, bool *in_use) {
	asm("xchg %bx, %bx");
	printf("hey its a terminal: %x",0x40);
	while(true){}
}
char buf[20];
extern "C"
void init_exec(hal::multiboot_header *mboot) {
	init_mboot(mboot);
	hal::physmem.init();
	hal::virtmem.init();
	task = reinterpret_cast<cpu_state *>(&sys_stack2-sizeof(cpu_state));
	task->rip=reinterpret_cast<uint64_t>(&task_b);
	task->rsp=reinterpret_cast<uint64_t>(&sys_stack2);
	task->rflags=0x200086;
	task->cs=0x8;
	task->ss=0x10;
	itoa(40,buf,10);
	puts(buf);
	install_interrupts();
	install_single_interrupt(0x40,foo);
	asm("int $0x40");
	init_scheduler();
	add_task(task);
	putc('A');
	yield();
	putc('a');
	yield();
	panic("reached end");
}
