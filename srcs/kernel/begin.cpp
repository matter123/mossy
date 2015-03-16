/*
    Copyright 2013-15 Matthew Fosdick

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
#include <sys/text_color.h>
#include <sys/kstacks.h>
#include <sys/heap.h>
#include <sys/tasks.h>
#include <sys/scheduler.h>
#include <sys/synchronization.h>
#include <utf8.h>
#include <string.h>
#include <ctype.h>
#include <acpi.h>
#include <acpi/acpi_os.h>
#include <test.h>
#include <sys/fb.h>
#include <sys/text_render.h>
#include <memory.h>
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
		hal::print_boot_msg("Init heap",heap_init(),true);
		//framebuffer and vterm
		init_fb();
		init_tr();
		//munching squares, im bored
		void *spot=malloc(4);
		memset(spot,0xAA,4);
		while(1) {

			int t1=rand()%1024;
			int t=t1;
			while((t-t1)<2000) {
				for(int x=0;x<1024;x++) {
					int y=x^t;
					if(y<768) {
						bit_blit(x,y,1,1,spot,OR);
					}
				}
				t++;
			}
			reset_fb();
		}
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
			hal::cout<<hal::endl<<std::TC::RESET;
		}
		//testing
		#if TEST
		test();
		#endif
		//acpi
		acpi::init_tables();
		hal::cout<<acpi::acpi.SDT_length<<hal::endl;
		for(int i=0;i<acpi::acpi.SDT_length;i++) {
			acpi::SDT *table=acpi::acpi.SDTs[i];
			hal::cout<<table->signature<<hal::endl;
		}
		//hal::print_boot_msg("Init ACPI",acpi::has_acpi(),false);
		//
		hal::init_vendor();
		//hal::print_boot_msg("Init kstacks",init_kstacks(),true);
		//hal::print_boot_msg("Init scheduler",init_scheduler((thread_info *)sys_stack),true);
		//done with setup
		time_t bt=BUILD_UNIX_TIME;
		hal::cout<<"Mossy build: "<<BUILD_FULL_NAME<<hal::endl;
		hal::cout<<"Built on: "<<std::TC::GREEN<<asctime(gmtime(&bt))<<std::TC::RESET
		         <<" By: "     <<std::TC::GREEN<<BUILD_USERNAME      <<std::TC::RESET
		         <<" From: "   <<std::TC::GREEN<<BUILD_GIT_BRANCH    <<std::TC::RESET<<hal::endl;
		while(1) {
			//hal::cout<<hal::dec<<num++<<hal::endl;
		}
	}
}
