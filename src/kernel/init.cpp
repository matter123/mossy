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
#include <acpi/acpi.h>
#include <arch/int.h>
#include <arch/paging.h>
#include <sys/pfa.h>
#include <sys/scheduler.h>
#include <sys/threadstacks.h>
#include <sys/malloc.h>
#include <hal/multiboot.h>
#include <hal/memmap.h>
#include <hal/commandline.h>
#include <logger.h>
#include <vga_text.h>
#include <stdlib.h>
#include <cpuid.h>
cpu_state *task;
extern pointer sys_stack2;
void task_b() {
	putc('B');
	yield();
	putc('b');
	yield();
}
extern "C"
thread_info *init_exec(hal::multiboot_header *mboot) {
	#define PORT 0x3f8   /* COM1 */
	outb(PORT + 1, 0x00);    // Disable all interrupts
	outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00);    //                  (hi byte)
	outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
	init_mboot(mboot);
	command_line_init();
	logger_init();
	thread_stack_size_init();
	hal::physmem.init();
	hal::virtmem.init();
	install_interrupts();
	pre_paging_init();
	pfa_init();
	paging_init();
	malloc_init();
	thread_stacks_init();
	thread_info *info=get_next_stack();
	return info;
}
extern "C"
void exec() {
	Log(LOG_DEBUG,"[INIT  ]","reached exec");
	setup_kernel_thread_info();
	AcpiInitializeSubsystem();
	AcpiInitializeTables(nullptr, 0, true);
	task = reinterpret_cast<cpu_state *>(&sys_stack2-sizeof(cpu_state));
	task->rip=reinterpret_cast<uint64_t>(&task_b);
	task->rsp=reinterpret_cast<uint64_t>(&sys_stack2);
	task->rflags=0x200086;
	task->cs=0x8;
	task->ss=0x10;
	init_scheduler();
	add_task(task);
	putc('A');
	yield();
	putc('a');
	yield();
	while(true);
	panic("reached end");
}
