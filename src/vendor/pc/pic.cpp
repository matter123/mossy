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
#include <hal/int.h>
#include <hal/console.h>
#include <arch.h>
#include <pc/cpu_stuff.h>
namespace pc {
#define M_CMD 0x20
#define M_DAT 0x21
#define S_CMD 0xA0
#define S_DAT 0xA1
	hal::idt_range pic_range;

	bool callback(int level, cpu_state *s);

	bool init_pic() {
		pic_range = hal::reserve_range(16,16,true);
		if(pic_range.len==0) {
			return false;
		}
		//remap pic around this range
		outb(M_CMD,0x11);//init
		outb(S_CMD,0x11);//init

		outb(M_DAT,pic_range.start);
		outb(S_DAT,pic_range.start+8);

		outb(M_DAT,0x4);//slave at IRQ2
		outb(S_DAT,0x2);//ITS A SLAVE

		outb(M_DAT,0x1);//x86
		outb(S_DAT,0x1);

		outb(M_DAT,0xFF);//all masked
		outb(S_DAT,0xFF);
		for(int i=0; i<pic_range.len; i++) {
			register_int(pic_range.start+i,callback,hal::REENTRANT,false);
		}
		return true;
	}

	bool callback(int level, cpu_state *s) {
		return true;
	}

	void use_irq(int num, void (*func)(cpu_state *),bool handle_eoi);
}
#endif