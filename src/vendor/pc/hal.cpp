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
#include <vendor.h>
#ifdef PC
#include <hal/multiboot.h>
#include <hal/mmap.h>
#include <hal/console.h>
#include <hal/hal.h>
#include "pic.h"
namespace hal {
	void init_vendor() {
		print_boot_msg("Init PIC",pc::init_pic(),true);
	}
	//a is boot info, b is videobuffer
	static mem_type a;
	static mem_type b;
	void add_special_mem_vendor() {
		add_region(3);
		a.bootinfo=true;
		a.resv_mem=true;
		b.videobuffer=true;
		b.resv_mem=true;
		//IVT and BDA
		add_region(0x0,0x4FF,a);
		//get EBDA
		uint32_t ebda_s=*(reinterpret_cast<uint16_t *>(0x40E));
		//if ebda is good add it
		if((ebda_s<<4)<0xA0000) {
			add_region(ebda_s<<4,0xA0000-(ebda_s<<4),a);
		} else {
			//else add a blank to make mmap.cpp happy
			//will get removed
			add_region(0,0,a);
		}
		//legacy video ram
		if(!add_region(0xA0000,0x5FFFF,b)) {
			hal::cout<<"OOPS";
			halt(true);
		}
	}
}
#endif