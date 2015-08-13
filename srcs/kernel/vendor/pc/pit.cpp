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
#include <vendor.h>
#ifdef PC
#include <hal/timer.h>
#include <io.h>
#include <arch.h>
#include "pic.h"
namespace hal {
#define CHAN_BP 0x40
#define COMMAND 0x43
	static uint64_t count=0;
	static uint32_t freq;
	static timer_tick timers[16];
	static int mods[16];
	void tick(cpu_state *s);

	bool timer_init(uint32_t f) {
		freq=f;
		uint32_t div=1193180/f;
		if(div<2) {
			div=2;
		} else if(div>65535) {
			div=65535;
		}
		outb(COMMAND, 0x36);
		outb(CHAN_BP+0,(uint8_t)(div&0xFF));
		outb(CHAN_BP+0,(uint8_t)((div>>8)&0xFF));
		pc::use_irq(0,&tick,true);
		return true;
	}

	bool install_timer_callback(timer_tick callback, int tick_mod) {
		for(int i=0; i<16; i++) {
			if(timers[i]==NULL) {
				timers[i]=callback;
				mods[i]=tick_mod;
				return true;
			}
		}
		return false;
	}

	bool reset_counter() {
		count=0;
		return true;
	}
	uint64_t get_counter() {
		return count;
	}
	uint32_t get_timer_freq() {
		return freq;
	}

	void tick(cpu_state *s) {
		for(int i=0; i<16; i++) {
			if(timers[i]!=NULL&&count%mods[i]==0) {
				timers[i](count);
			}
		}
		count++;
	}
}
#endif
