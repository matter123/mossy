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
#include <sys/tasks.h>
#include <string.h>
namespace kernel {
	thread_info *create_task(uintptr_t stack, void *func,bool kernel, uint32_t PID) {
		thread_info *t=reinterpret_cast<thread_info *>(stack);
		memset((void *)t,0,sizeof(thread_info));
		stack-=sizeof(thread_info);
#ifdef X64
		t->cpu_state = x64::create_task(stack, func, kernel);
#elif defined X86
		t->cpu_state = x86::create_task(stack, func, kernel);
#endif
		t->PID=PID;
		t->active=true;
		return t;
	}
}
