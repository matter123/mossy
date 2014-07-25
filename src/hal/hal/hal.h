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
#pragma once
#include <stdint.h>
#include <hal/multiboot.h>
namespace hal {

	//called before init_vendor()
	void init_arch();

	void init_vendor();

	void add_phys_mem_arch();

	void add_phys_mem_vendor();

	void add_virt_mem_arch();

	void add_virt_mem_vendor();

	uintptr_t get_page_offset_addr();

	void enable_interrupts();

	void disable_interrupts();

	void halt(bool no_int);

	void magic_break();
}