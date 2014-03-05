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

#include "init/multiboot.h"
#include "arch/x86/idt.h"
#include "arch/x86/exceptions.h"
#include "arch/x86/pic.h"
#include "arch/x86/pit.h"
#include "arch/x86/mmap.h"
#include "arch/x86/paging.h"
#include "monitor.h"
#include "hhalf.h"
#include <conv.hpp>

namespace kernel {
	void init_system(multiboot_t *mboot);
	extern "C"
	void init_exec(multiboot_t *mboot) {
		init_system(mboot);
		std::cout <<"hello setup kernel";
		//asm("int $32");
		while(1);
	}
	void init_system(multiboot_t *mboot) {
		//GDT, basic paging
		init_higher_half();
		x86::init_exceptions();
		x86::init_pic();
		x86::install_pic_idt();
		x86::enable_idt();
		mboot=fix_tables(mboot);
		parse_mboot_mmap(mboot);

		x86::paging::enable_paging();
		//IDT setup
		x86::init_pit(x86::PIT_FREQ_10MS);
		asm("sti");
	}
}
