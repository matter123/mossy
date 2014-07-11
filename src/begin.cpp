/*
    Copyright 2013 Matthew Fosdick

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
#include <arch.h>
#include <hal/hal.h>
#include <hal/console.h>
#include <pc/cpu_stuff.h>
#include <string.h>
#include <build_info.h>
#include <time.h>

namespace kernel {
	extern "C"
	void init_exec(hal::multiboot_header *mboot) {
		time_t some_date=1404791398;
		tm *ret=gmtime(&some_date);
		//hal::cout<<ret->tm_hour;
		hal::cout<<asctime(ret)<<hal::endl;
		hal::cout<<"HAPPY ANNIVERSITY (1 year)"<<hal::endl;
		hal::init_mboot(mboot);
		hal::init_arch();
		hal::init_vendor();
		//hal::cls();
		hal::cout<<"the HAL says \"hello\""<<hal::endl;
		hal::cout<<hal::get_tag_count()<<" multiboot tags"<<hal::endl;
		hal::cout<<"Built on: "<<BUILD_UNIX_TIME<<" by: "<<BUILD_USERNAME
		         <<" from: "<<BUILD_GIT_BRANCH<<hal::endl;
		hal::halt(true);
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
