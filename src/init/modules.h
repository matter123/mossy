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
#pragma once
#include <stdint.h>
#include <hal/multiboot.h>
#include <struct.h>

namespace kernel {
	struct mboot_module_t {
		uintptr_t mod_start;
		uintptr_t mod_size;
		char *string;
		uint32_t resv;
	} FULL;
	void load_modules();
	int get_module_count();
	mboot_module_t *get_module(int index);
	uint32_t get_magic(int index);
}