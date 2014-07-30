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
#include <sys/kstacks.h>
#include <sys/tasks.h>
#include <sys/heap.h>

namespace kernel {
	void B() {
		while(1) {
			static int let=5;
			hal::cout<<std::TC::RED<<(char)(33+let++);
			let%=93;
		}
	}
	extern "C"
	void init_exec(hal::multiboot_header *mboot) {
		//hal::magic_break();
		hal::init_mboot(mboot);
		hal::init_arch();
		hal::init_vendor();
		hal::print_boot_msg("Init kstacks",init_kstacks(),true);
		hal::print_boot_msg("Init heap",heap_init(),true);
		time_t bt=BUILD_UNIX_TIME;
		hal::cout<<"Built on: "<<std::TC::GREEN<<asctime(gmtime(&bt))<<std::TC::WHITE
		         <<" By: "     <<std::TC::GREEN<<BUILD_USERNAME      <<std::TC::WHITE
		         <<" From: "   <<std::TC::GREEN<<BUILD_GIT_BRANCH    <<std::TC::WHITE<<hal::endl;
		hal::enable_interrupts();
		void *a=malloc(8);
		void *b=malloc(8);
		free(a);
		free(b);
		void *c=malloc(16);
		hal::cout<<hal::address<<a<<" "<<b<<" "<<c<<hal::endl;
		hal::halt(true);
		add_task(create_task(get_new_stack(),(void *)&B,true));
		while(1) {
			static int let=0;
			hal::cout<<std::TC::GREEN<<(char)(33+let++);
			let%=93;
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
