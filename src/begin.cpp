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

namespace kernel {
	extern "C"
	void init_exec(hal::multiboot_header *mboot) {
		char a[100];
		hal::cout<<strcpy(a, "fruit")<<hal::endl;
		hal::cout<<strncpy(a, "fruits and apples are fun",10)<<hal::endl;
		hal::cout<<strcat("pies are fast\0apple be cool"," and cool")<<hal::endl;
		hal::cout<<(char *)memcpy(a,"five apples\0aaaaaaaaa",22)<<hal::endl; \
		memmove(a+8,a+4,8);
		hal::cout<<a<<hal::endl;
		hal::cout<<strncat(a,"are very fun",8)<<hal::endl;
		hal::cout<<memcmp("apples", "apples",7)<<" "<<
		         memcmp("apples\0abc", "apples\0def",11)<<" "<<
		         memcmp("apples\0POD", "apples\0Aef",11)<<hal::endl;
		hal::cout<<strcmp("apples", "apples")<<" "<<
		         strcmp("apples\0abc", "apples\0def")<<" "<<
		         strcmp("apples\0POD", "apples\0Aef")<<hal::endl;
		hal::cout<<(const char *)memchr((void *)"abcdefg8hijklmnopqrstuvxyz",'8',28)<<hal::endl;
		hal::cout<<strchr("abcdefg8hijklmnopqrstuvxyz",'8')<<hal::endl;
		hal::cout<<strcspn("apples are very red, except when they're green"," ")<<hal::endl;
		hal::cout<<strpbrk("apples are very red, except when they're green"," ")<<hal::endl;
		hal::cout<<strrchr("apples are very red, except when they're green",'g')<<hal::endl;
		hal::cout<<strspn("apples are very red, except when they're green","aples rvyd")<<hal::endl;
		hal::cout<<strstr("apples are very red, except when they're green","except")<<hal::endl;
		hal::cout<<strtok("apples are very red, except when they're green"," ,")<<hal::endl;
		char *str=strtok(NULL," ,");
		while(str) {
			hal::cout<<str<<hal::endl;
			str=strtok(NULL," ,");
		}
		while(1);
		/*
		    hal::cout<<hal::address<<reinterpret_cast<uintptr_t>(mboot)<<hal::endl;
		    //hal::init_mboot(mboot);
		    hal::cout<<hal::address<<reinterpret_cast<uintptr_t>(hal::get_header());
		    hal::init_arch();
		    hal::init_vendor();
		    //hal::cls();
		    hal::cout<<"the HAL says \"hello\""<<hal::endl;
		    hal::cout<<(((1<<29)&cpuid(0x80000001,3))==(1<<29))<<hal::endl;
		    hal::halt(true);
		    asm volatile(
		    " movl $0, %eax \
		    \nmovl $1, %ebx \
		    \nmovl $2, %ecx \
		    \nmovl $3, %edx \
		    \nmovl $4, %esi \
		    \nmovl $5, %edi \
		    \nint $3");*/
	}
}
