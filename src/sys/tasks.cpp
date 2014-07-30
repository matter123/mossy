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
#include <x86/tasks.h>
#include <x64/tasks.h>
namespace kernel {
	void add_task(cpu_state *s) {
#ifdef X64
		return x64::add_task(s);
#elif defined X86
		return x86::add_task(s);
#endif
	}
	cpu_state *create_task(uintptr_t stack, void *func,bool kernel) {
#ifdef X64
		return x64::create_task(stack, func, kernel);
#elif defined X86
		return x86::create_task(stack, func, kernel);
#endif
	}
}