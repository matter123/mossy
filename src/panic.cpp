/*
 * Copyright 2013 Matthew Fosdick

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

#include "panic.h"
#include <cstring>
#include <conv.hpp>
#include "monitor.h"

namespace kernel {
	void panic() {
		panic("Unknown Error\nAbandon All Hope");
	}
	void panic(uint32_t code) {
		panic("");
	}
	void panic(std::c_cstring msg) {
		std::cout<<std::endl<<"PANIC: An Unrecoverable Error Has Occurred";
		std::cout<<std::endl<<msg;
		asm("cli\nhlt");
		while(1);//will never execute
	}
	void panic(std::c_cstring file,uint32_t line) {
		panic(" ");
	}
}
