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
#include <sys/scheduler.h>
#include <sys/threadstacks.h>
#include <hal/hal.h>
#include <arch/int.h>
#include <string.h>
thread_info *ringbuffer[64];
int front_idx = 0, rear_idx = 0;

extern void *C0_handler;
extern "C" uint64_t __stack_chk_guard;

uint32_t next_task_id = 0;

void idle_thread() { asm("hlt"); }

uint32_t get_next_task_id() { return __sync_add_and_fetch(&next_task_id, 1); }
// uint8_t fpu_control[16];
bool initalized = false;
void yield(uint32_t thread_id) {
	if(initalized) asm("int $0xC0" ::"a"(thread_id));
}
void yield() { yield(0); }
void wait(uintptr_t object) {
	get_current_thread_info()->block_on = object;
	if(initalized) yield();
}
void release(uintptr_t object) {
	if(initalized) {
		enter_critical();
		asm("int $0xC3" ::"a"(object));
	}
}
void add_task(void *func, void *context) {
	thread_info *info = get_next_stack();
	info->stack_guard_1 = __stack_chk_guard;
	info->stack_guard_2 = __stack_chk_guard;
	info->state = thread_info::WAITING;
	info->thread_id = get_next_task_id();
	uintptr_t top = reinterpret_cast<uintptr_t>(info) + stack_size;
	cpu_state *task = reinterpret_cast<cpu_state *>(top - sizeof(cpu_state));
	task->rip = reinterpret_cast<uint64_t>(func);
	task->rsp = reinterpret_cast<uint64_t>(top);
	task->rdi = reinterpret_cast<uint64_t>(context);
	task->rflags = 0x200286;
	task->cs = 0x8;
	task->ss = 0x10;
	task->fs = 0x10;
	task->gs = 0x10;
	info->kinterrupt = task;
	asm volatile(" fxsave %0; " ::"m"(info->sse_save));
	ringbuffer[rear_idx++ % 64] = info;
	rear_idx %= 64;
}
void init_scheduler() {
	install_JT1(0xC0, &C0_handler);
	initalized = true;
}
extern "C" cpu_state *get_next(cpu_state *current) {
	thread_info *info = get_current_thread_info();
	info->kinterrupt = current;
	if(info->state == thread_info::RUNNING) {
		info->state = thread_info::WAITING;
		// add back to ready queue
		ringbuffer[rear_idx++ % 64] = info;
		rear_idx %= 64;
	} else {
		// assume already added to list
	}
	// select item from ready queue
	if(rear_idx == front_idx) {
		// ready queue is empty
		// do idle
		PANIC("ready queue is empty");
	} else {
		do {
			info = ringbuffer[front_idx++ % 64];
			front_idx %= 64;
		} while(info->state != thread_info::WAITING);
		cpu_state *temp = info->kinterrupt;
		info->kinterrupt = 0;
		info->state = thread_info::RUNNING;
		return temp;
	}
}

void setup_kernel_thread_info() {
	thread_info *info = get_current_thread_info();
	info->stack_guard_1 = __stack_chk_guard;
	info->stack_guard_2 = __stack_chk_guard;
	info->state = thread_info::RUNNING;
	info->thread_id = get_next_task_id();
}
