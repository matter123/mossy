/*
    Copyright 2013 Matthew Fosdick

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
#include <hal/console.h>
#include <io.h>
#include <arch.h>
#include "pic.h"
namespace hal {
#define CHAN_BP 0x40
#define COMMAND 0x43
	uint64_t count;
	uint32_t freq;
	void tick(cpu_state *s);

	bool timer_init(uint32_t f) {
		uint32_t div=1193180/f;
		if(div<2) {
			div=2;
		} else if(div>65535) {
			div=65535;
		}
		outb(COMMAND, 0x36);
		outb(CHAN_BP+0,(uint8_t)(div&0xFF));
		outb(CHAN_BP+0,(uint8_t)((div>>8)&0xFF));
		pc::use_irq(1,&tick,false);
		return true;
	}
	/*
	    bool install_timer_callback(timer_tick callback, int tick_mod);
	    bool reset_counter();
	    uint64_t get_counter();
	    uint32_t get_timer_freq();*/

	void tick(cpu_state *s) {
		hal::cout<<"tick"<<hal::endl;
	}
}
#endif