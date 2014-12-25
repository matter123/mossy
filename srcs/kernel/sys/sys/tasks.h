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
#include <arch.h>
namespace kernel {
	struct thread_info {
		uint32_t PID;
		uint8_t  priority;
		bool     active:1;
		bool     running:1;
		bool     sleeping:1;
		bool     waiting:1;
		bool     has_user_stack:1;
		int      resv_state:3;
		uint16_t sleep_ticks;
		void     *wait_on;
		void     *user_stack;
		::cpu_state *cpu_state;
	};
	void add_task(thread_info *s);
	thread_info *create_task(uintptr_t stack, void *func,bool kernel, uint32_t PID);
}
