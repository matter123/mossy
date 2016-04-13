/*
	Copyright 2015 Matthew Fosdick

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
#include<sys/scheduler.h>
#include<arch/int.h>
#include <sys/threadstacks.h>
constexpr ptrdiff_t sse_save_offset=offsetof(thread_info, sse_save);

cpu_state *other;
extern void *C0_handler;
void yield() {
	asm("int $0xC0");
}
void add_task(cpu_state *task) {
	other=task;
}
void init_scheduler() {
	install_JT1(0xC0,&C0_handler);
}
extern "C"
cpu_state *get_next(cpu_state *current) {
	if(!other)other=current;
	cpu_state *temp=other;
	other=current;
	return temp;
}

extern "C" uint64_t __stack_chk_guard;
void setup_kernel_thread_info() {
	uintptr_t rsp=0;
	thread_info *info=(thread_info *)((uintptr_t)&rsp&~stack_size);
	info->stack_guard_1=__stack_chk_guard;
	info->stack_guard_2=__stack_chk_guard;
	info->state=thread_info::RUNNING;
}
