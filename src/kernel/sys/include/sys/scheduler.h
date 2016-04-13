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
#pragma once
#include <arch/int.h>
struct thread_info {
	uint64_t stack_guard_1;
	uint32_t thread_id;
	uint32_t process_id;
	uint64_t block_on;
	enum thread_state {
		RUNNING,
		WAITING,
		BLOCKING,
		SLEEPING,
	} state;
	int priority;
	int ticks_left;
	int sleep_ticks;
	uint8_t sse_save[512] __attribute__((aligned(16)));
	uint64_t stack_guard_2;
};
static_assert(sizeof(thread_info)<=1024,"thread info is too big");

extern "C" const ptrdiff_t sse_save_offset;

void init_scheduler();

void yield();

void add_task(cpu_state *);

void setup_kernel_thread_info();
