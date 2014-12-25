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
#include <arch.h>
#ifdef X86
#include <stdint.h>
#include <stdlib.h>

extern "C" uint32_t get_ebp();
namespace hal {
	struct stack_frame {
		uint32_t ebp;
		uint32_t eip;
	};

	uint32_t get_your_ebp(uint32_t junk) {
		uint32_t *ebp=&junk-2;
		return ebp[0];
	}

	uintptr_t get_function(stack_frame *s) {
		return s->eip;
	}
	stack_frame *get_next_frame(stack_frame *s) {
		if(s->eip) {
			return reinterpret_cast<stack_frame *>(s->ebp);
		}
		return NULL;
	}
	stack_frame *get_current_frame() {
		return reinterpret_cast<stack_frame *>(get_ebp());
	}
}
#endif
