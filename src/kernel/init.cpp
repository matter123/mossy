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
#include <arch/int.h>
void test (cpu_state *cpu, void *sse_save,bool *in_use) {
	assert(cpu->int_num,40,"not handler for 40");
}
extern "C"
void init_exec() {
	install_interrupts();
	install_single_interrupt(40,test);
	install_single_interrupt(50,test);
	asm("xchg %bx, %bx");
	asm("int $40");
	asm("int $50");
	panic("reached end");
}
