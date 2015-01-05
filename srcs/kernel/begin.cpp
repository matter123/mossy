/*
    Copyright 2014 Matthew Fosdick

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
#include <utf8.h>
#include <string.h>
#include <ctype.h>
#include <acpi.h>
#include <acpi_os.h>
#include <tables/rsdp.h>
#include <tables/sdt.h>
#include <test.h>
#include <sys/fb.h>
#include <sys/text_render.h>
#include <memory>
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
		hal::cout<<std::TC::BACKCOLOR<<std::TC::BLACK<<
		         "    |00|01|02|03|04|05|06|07|08|09|0A|0B|0C|0D|0E|0F|10|11|12|13|14|15|16|17|18|19|1A|1B|1C|1D|1E|1F|"
		         <<hal::endl<<
		         "    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+"
		         <<hal::endl<<hal::ios_base(16,2,1,4)<<std::TC::GREEN;
		char array[5]= {0};
		uintptr_t line_start=0x0020;
		while(line_start<=0x04E0) {
			if(line_start==0x0080) {
				hal::cout<<std::TC::RED;
			}
			if(line_start==0x00A0) {
				hal::cout<<std::TC::WHITE;
			}
			if(line_start==0x0100) {
				hal::cout<<std::TC::YELLOW;
			}
			if(line_start==0x0180) {
				hal::cout<<std::TC::BLUE;
			}
			if(line_start==0x0300) {
				hal::cout<<std::TC::DCYAN;
			}
			if(line_start==0x0400) {
				hal::cout<<std::TC::DMAGENTA;
			}
			hal::cout<<line_start<<"|";
			for(int i=0; i<0x20; i++) {
				if((line_start+i)==0x0250) {
					hal::cout<<std::TC::CYAN;
				}
				if((line_start+i)==0x02B0) {
					hal::cout<<std::TC::MAGENTA;
				}
				if((line_start+i)==0x02B0) {
					hal::cout<<std::TC::LGRAY;
				}
				unicode::utf8::encode_char(line_start+i,array);
				hal::cout<<" "<<array<<(i!=0x1F?" ":"|");
			}
			line_start+=0x20;
			hal::cout<<hal::endl;
		}
		while(true);
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
		int num=0;
		while(1) {
			hal::cout<<hal::dec<<num++<<hal::endl;
		}
	}
}
