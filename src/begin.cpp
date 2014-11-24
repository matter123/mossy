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
	void setup_vga();
}
namespace kernel {
	extern "C" uintptr_t sys_stack;
	extern "C"
	void init_exec(hal::multiboot_header *mboot) {
		//hal::magic_break();
		hal::init_mboot(mboot);
		hal::init_arch();
		hal::print_boot_msg("Init heap",heap_init(),true);
		#if TEST
		test();
		#endif
		init_fb();
		init_tr();
		srand(BUILD_UNIX_TIME);
		while(1) {
			int x=(rand()%(get_w()-130))+20;
			int y=(rand()%(get_h()-59))+20;
			draw_string_at(x,y,"lowercase Whoo~!");
		}
		acpi::init_tables();
		hal::print_boot_msg("Init ACPI",acpi::has_acpi(),false);
		char test[9];
		memcpy(test,acpi::tables::rsdp_ptr->begin.sig,8);
		test[8]='\0';
		//hal::setup_vga();
		hal::init_vendor();
		//hal::print_boot_msg("Init kstacks",init_kstacks(),true);
		//hal::print_boot_msg("Init scheduler",init_scheduler((thread_info *)sys_stack),true);
		time_t bt=BUILD_UNIX_TIME;
		hal::cout<<"Built on: "<<std::TC::GREEN<<asctime(gmtime(&bt))<<std::TC::WHITE
		         <<" By: "     <<std::TC::GREEN<<BUILD_USERNAME      <<std::TC::WHITE
		         <<" From: "   <<std::TC::GREEN<<BUILD_GIT_BRANCH    <<std::TC::WHITE<<hal::endl;
		while(1);
	}
}
