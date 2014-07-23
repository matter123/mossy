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
#include <hal/paging.h>
#include <string.h>
#include <build_info.h>
#include <time.h>
#include <text_color.h>

namespace kernel {
	extern "C"
	void init_exec(hal::multiboot_header *mboot) {
		//hal::magic_break();
		hal::init_mboot(mboot);
		hal::init_arch();
		hal::init_vendor();
		hal::enable_interrupts();
		time_t bt=BUILD_UNIX_TIME;
		hal::cout<<"Built on: "<<std::TC::GREEN<<asctime(gmtime(&bt))<<std::TC::WHITE
		         <<" By: "     <<std::TC::GREEN<<BUILD_USERNAME      <<std::TC::WHITE
		         <<" From: "   <<std::TC::GREEN<<BUILD_GIT_BRANCH    <<std::TC::WHITE<<hal::endl;
		//hal::halt(false);
		hal::map_free_to_virt_cur(0x700000, {true,false,true});
		hal::map_phys_to_virt_cur(0x800000,hal::phys_from_virt(0x700000), {true,false,true});


		strcpy((char *)0x700000,"APPLE");
		hal::cout<<(char *)0x800000;
		while(1) {
			//hal::cout<<"A";
		}
		asm volatile(
		    " movl $0, %eax \
		    \nmovl $1, %ebx \
		    \nmovl $2, %ecx \
		    \nmovl $3, %edx \
		    \nmovl $4, %esi \
		    \nmovl $5, %edi \
		    \nint $3");
	}
}
