/*
 * Copyright 2013 Matthew Fosdick

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

#include <stdint.h>
#include "pit.h"
#include "pic.h"
#include <io.h>
#include "../../monitor.h"

namespace x86 {
#define PIT_C0_DATA 0x40
#define PIT_C1_DATA 0x41
#define PIT_C2_DATA 0x42
#define PIT_CMD_CHG 0x43

	void handleTick(stack_t s);

	void init_pit(int freq) {
		uint32_t div=1193180;
		switch(freq) {
			case PIT_FREQ_05MS:
				div/=(1000/5);
				break;
			case PIT_FREQ_10MS:
				div/=(1000/10);
				break;
			case PIT_FREQ_15MS:
				div/=(1000/15);
				break;
			case PIT_FREQ_20MS:
				div/=(1000/20);
				break;

		}
		outb(PIT_CMD_CHG,0x36);
		outb(PIT_C0_DATA,div&0xFF);
		outb(PIT_C0_DATA,(div<<8)&0xFF);
		x86::request_irq(0,&handleTick);
	}

	void handleTick(stack_t s) {
		std::cout<<"tick happened"<<std::endl;
	}
}
