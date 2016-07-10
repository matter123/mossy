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
#include <hal/hal.h>
#include <hal/memmap.h>
#include <hal/multiboot.h>
#include <arch/int.h>
#include <arch/lapic.h>
#include <arch/paging.h>
#include <acpi/acpi.h>
#include <cpuid.h>
#include <logger.h>
#include <memory>
#include <stdlib.h>
#include <vga_text.h>
void test_task(char *letter) {
	while(true) {
		puts(letter);
		// yield();
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
extern void *C1_handler;

void shared_ptr_test() {
	class test {
	  public:
		test() { printf("%s\n", "test constructed"); }
		~test() { printf("%s\n", "test deconstructed"); }
		void print() {
			static int num = 0;
			printf("test print: %d\n", num++);
		}
	};
	std::shared_ptr<test> test_ptr(new test());
	test_ptr->print();
	(*test_ptr).print();
	// should fall out of scope now
}

extern "C" void exec() {
	Log(LOG_DEBUG, "[INIT  ]", "reached exec");
	setup_kernel_thread_info();
	// AcpiInitializeSubsystem();
	AcpiOsInitialize();
	AcpiInitializeTables(nullptr, 0, true);
	initalize_lapic();
	init_scheduler();
	install_JT1(0xC1, &C1_handler);
	set_lapic_timer(LAPIC_TIMER_MODE_PERIODIC, LAPIC_TIMER_DIVIDE_16_VALUE, 0xC1, 32 * 128);
	hal::enable_interrupts();
	Log(LOG_ERROR, "[INIT  ]", "reached end");
}
