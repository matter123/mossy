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
#include <stdlib.h>
#include <hal/int.h>
#include <hal/console.h>
#include <arch.h>
#include <io.h>
#include <pc/cpu_stuff.h>
namespace pc {
#define M_CMD 0x20
#define M_DAT 0x21
#define S_CMD 0xA0
#define S_DAT 0xA1
	hal::idt_range pic_range;

	bool callback(int level, cpu_state *s);
	void (*funcs[16])(cpu_state *)= {NULL};
	bool handle_eoi[16] = {false};

	bool init_pic() {
		pic_range = hal::reserve_range(16,16,true);
		if(pic_range.len==0) {
			return false;
		}
		hal::cout<<pic_range.start<<hal::endl;
		//remap pic around this range
		outb(M_CMD,0x11);//init
		io_wait();
		outb(S_CMD,0x11);//init
		io_wait();

		outb(M_DAT,pic_range.start);
		io_wait();
		outb(S_DAT,pic_range.start+8);
		io_wait();

		outb(M_DAT,0x4);//slave at IRQ2
		io_wait();
		outb(S_DAT,0x2);//ITS A SLAVE
		io_wait();

		outb(M_DAT,0x1);//x86
		io_wait();
		outb(S_DAT,0x1);
		io_wait();

		outb(M_DAT,0xFF);//all masked
		io_wait();
		outb(S_DAT,0xFF);
		for(int i=0; i<pic_range.len; i++) {
			register_int(pic_range.start+i,callback,hal::REENTRANT,false);
		}
		return true;
	}

	bool callback(int level, cpu_state *s) {
		int irq=get_int_num(s)-pic_range.start;
		void (*func)(cpu_state *)=funcs[irq];
		if(handle_eoi[irq]) {
			if(irq>8) {
				outb(S_CMD,0x20);
			}
			outb(M_CMD,0x20);
		}
		if(func!=NULL) {
			func(s);
		}
		if(!handle_eoi[irq]) {
			if(irq>8) {
				outb(S_CMD,0x20);
			}
			outb(M_CMD,0x20);
		}
		return true;
	}

	void unmask(int irq) {
		uint16_t port;
		uint8_t value;

		if(irq < 8) {
			port = M_DAT;
		} else {
			port = S_DAT;
			irq -= 8;
		}
		value = inb(port) & ~(1 << irq);
		io_wait();
		outb(port, value);
	}

	void use_irq(int num, void (*func)(cpu_state *),bool handle_eoi_before) {
		if(num<16) {
			funcs[num]=func;
			handle_eoi[num]=handle_eoi_before;
			unmask(num);
		}
	}
}
#endif