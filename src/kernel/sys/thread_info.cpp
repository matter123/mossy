/*
	Copyright 2016 Matthew Fosdick

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

thread_info *get_current_thread_info() {
	uintptr_t rsp=0;
	rsp=(uintptr_t)&rsp;
	uintptr_t mask=~(stack_size-1);
	return (thread_info *)(rsp&mask);
}
