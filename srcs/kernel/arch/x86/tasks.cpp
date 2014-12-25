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
#include <arch.h>
#ifdef X86
#include <stdlib.h>
#include <string.h>
#include <hal/hal.h>
#include <hal/console.h>
namespace x86 {
	static cpu_state *tasks[128];
	static uint next=1;
	static int last=-1;
	cpu_state *schedule(cpu_state *s) {
		if(last==-1) {
			last=0;
		}
		if(next>128) {
			next%=128;
		}
		tasks[last]=s;
		for(int i=128; i-->0; i++) {
			if(tasks[next%128]) {
				last=next%128;
				next++;
				break;
			}
			next++;
		}
		return tasks[last];
	}
	void add_task(cpu_state *s) {
		uint i=1;
		while(tasks[i]) {
			i++;
		}
		tasks[i]=s;
	}
	cpu_state *create_task(uintptr_t stack, void *func,bool kernel) {
		stack-=sizeof(cpu_state);
		cpu_state *s=reinterpret_cast<cpu_state *>(stack);
		memset(s,0,sizeof(cpu_state));
		s->eip=reinterpret_cast<uintreg_t>(func);
		s->eflags|=(1<<9);
		s->cs=(kernel?0x8:0x18);
		s->ds=(kernel?0x10:0x20);
		s->es=(kernel?0x10:0x20);
		s->fs=(kernel?0x10:0x20);
		s->gs=(kernel?0x10:0x20);
		if(!kernel) {
			s->esp=stack;
			s->ss=0x20;
		}
		return s;
	}
}
#endif