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
	class test {
			int getIndex() {
				static int i=0;
				return i++;
			}
			int cindex;
		public:
			test() {
				cindex=getIndex();
				hal::cout<<"Created a new test at: "<<hal::address<<this<<hal::endl;
				hal::cout<<"\tindex: "<<hal::dec<<cindex<<hal::endl;
			}
			~test() {
				hal::cout<<"destroying: "<<hal::dec<<cindex<<hal::endl;
			}
	};
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
		test *t=new test();
		test *t2=new test();
		delete t2;
		t2=new test();
		delete t;
		t=new test();
		delete t2;
		delete t;
		hal::halt(true);
	}
}