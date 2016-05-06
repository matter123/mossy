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
#include <sys/malloc.h>
#include <sys/pfa.h>
#include <sys/scheduler.h>
#include <sys/threadstacks.h>
#include <hal/commandline.h>
#include <hal/memmap.h>
#include <hal/multiboot.h>
#include <arch/int.h>
#include <arch/paging.h>
#include <acpi/acpi.h>
#include <cpuid.h>
#include <logger.h>
#include <stdlib.h>
#include <vga_text.h>
void test_task(char *letter) {
	while(true) {
		puts(letter);
		yield();
	}
}
extern "C" thread_info *init_exec(hal::multiboot_header *mboot) {
#define PORT 0x3f8        /* COM1 */
	outb(PORT + 1, 0x00); // Disable all interrupts
	outb(PORT + 3, 0x80); // Enable DLAB (set baud rate divisor)
	outb(PORT + 0, 0x03); // Set divisor to 3 (lo byte) 38400 baud
	outb(PORT + 1, 0x00); //                  (hi byte)
	outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
	outb(PORT + 2, 0xC7); // Enable FIFO, clear them, with 14-byte threshold
	outb(PORT + 4, 0x0B); // IRQs enabled, RTS/DSR set
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
	thread_info *info = get_next_stack(); // get task for main thread to reside in
	return info;
}
extern "C" void exec() {
	Log(LOG_DEBUG, "[INIT  ]", "reached exec");
	setup_kernel_thread_info();
	AcpiInitializeSubsystem();
	AcpiInitializeTables(nullptr, 0, true);
	init_scheduler();
	add_task((void *)test_task, (void *)"A");
	add_task((void *)test_task, (void *)"B");
	add_task((void *)test_task, (void *)"C");
	add_task((void *)test_task, (void *)"D");
	add_task((void *)test_task, (void *)"E");
	add_task((void *)test_task, (void *)"F");
	add_task((void *)test_task, (void *)"G");
	add_task((void *)test_task, (void *)"H");
	add_task((void *)test_task, (void *)"I");
	add_task((void *)test_task, (void *)"J");
	add_task((void *)test_task, (void *)"K");
	add_task((void *)test_task, (void *)"L");
	add_task((void *)test_task, (void *)"M");
	add_task((void *)test_task, (void *)"N");
	add_task((void *)test_task, (void *)"O");
	add_task((void *)test_task, (void *)"P");
	add_task((void *)test_task, (void *)"Q");
	add_task((void *)test_task, (void *)"R");
	add_task((void *)test_task, (void *)"S");
	add_task((void *)test_task, (void *)"T");
	add_task((void *)test_task, (void *)"U");
	add_task((void *)test_task, (void *)"V");
	add_task((void *)test_task, (void *)"W");
	add_task((void *)test_task, (void *)"X");
	add_task((void *)test_task, (void *)"Y");
	add_task((void *)test_task, (void *)"Z");
	asm("xchg %bx, %bx");
	while(true) yield();
	panic("reached end");
}
