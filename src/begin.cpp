/*
    Copyright 2013-2014 Matthew Fosdick

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

#include <hal/multiboot.h>
#include <hal/hal.h>
#include <hal/console.h>
#include <build_info.h>
#include <time.h>
#include <text_color.h>
#include <sys/kstacks.h>
#include <sys/heap.h>
#include <sys/tasks.h>
#include <sys/scheduler.h>
#include <sys/synchronization.h>
#include <unicode.h>
#include <string.h>
#include <ctype.h>
#include <acpi.h>
#include <acpi_os.h>
#include <tables/rsdp.h>
#include <tables/sdt.h>
#include <test.h>
#include <sys/fb.h>
#include <sys/text_render.h>
namespace hal {
	void ready();
}
namespace kernel {
	extern "C" uintptr_t sys_stack;
	extern "C"
	void init_exec(hal::multiboot_header *mboot) {
		//inital setup
		srand(BUILD_UNIX_TIME);
		hal::init_mboot(mboot);
		hal::init_arch();
		//framebuffer and vterm
		init_fb();
		init_tr();
		hal::ready();
		//heap
		hal::print_boot_msg("Init heap",heap_init(),true);
		//testing
		#if TEST
		test();
		#endif
		//acpi
		//acpi::init_tables();
		//hal::print_boot_msg("Init ACPI",acpi::has_acpi(),false);
		//
		hal::init_vendor();
		//hal::print_boot_msg("Init kstacks",init_kstacks(),true);
		//hal::print_boot_msg("Init scheduler",init_scheduler((thread_info *)sys_stack),true);
		//done with setup
		time_t bt=BUILD_UNIX_TIME;
		hal::cout<<"Built on: "<<std::TC::GREEN<<asctime(gmtime(&bt))<<std::TC::RESET
		         <<" By: "     <<std::TC::GREEN<<BUILD_USERNAME      <<std::TC::RESET
		         <<" From: "   <<std::TC::GREEN<<BUILD_GIT_BRANCH    <<std::TC::RESET<<hal::endl;
		while(1);
	}
}
