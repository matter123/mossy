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

#include "init/multiboot.h"
#include "arch/arch.h"
#include <hal/hal.h>
#include <hal/console.h>

namespace kernel {
	extern "C"
	void init_exec(multiboot_t *mboot) {
		hal::init_arch(mboot);
		hal::init_vendor(mboot);
		hal::cls();
		hal::cout<<"the HAL says \"hello\""<<hal::endl;
		asm("int $3");
		while(1);
	}
}
