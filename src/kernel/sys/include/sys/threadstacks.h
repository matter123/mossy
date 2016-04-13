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

#pragma once
#include <sys/scheduler.h>
/**
 * calculates the size of kernel stacks from command line paramater
 */
void thread_stack_size_init();

/**
 * initalizes the free stack list
 */
void thread_stacks_init();

/**
 * gets the next availiable free stack
 * @return thread_info at the bottom of the stack
 */
thread_info *get_next_stack();

/**
 * returns a stack to the free stack list
 * @param stack the thread_info of the returned stack
 */
void free_stack(thread_info *stack);

/**
 * stack_size is the size of kernel stacks
 */
extern "C" size_t stack_size;
