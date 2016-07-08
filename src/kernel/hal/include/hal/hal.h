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
// useful functions that very greatly based on platform
namespace hal {
void magic_break();
void idle();
void halt();
void enable_interrupts();
void disable_interrupts();

#define enter_critical()           \
	do {                           \
		hal::disable_interrupts(); \
		__sync_synchronize();      \
	} while(0)

#define leave_critical()          \
	do {                          \
		__sync_synchronize();     \
		hal::enable_interrupts(); \
	} while(0)
}
