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

#include "pic.h"
#include "idt.h"
#include <io.h>
#include <stdint.h>
#include "../../monitor.h"

extern void * pic0;
extern void * pic1;
extern void * pic2;
extern void * pic3;
extern void * pic4;
extern void * pic5;
extern void * pic6;
extern void * pic7;
extern void * pic8;
extern void * pic9;
extern void * pic10;
extern void * pic11;
extern void * pic12;
extern void * pic13;
extern void * pic14;
extern void * pic15;

namespace x86 {
#define PIC_M_CMD    0x20
#define PIC_M_DATA   0x21
#define PIC_S_CMD    0xA0
#define PIC_S_DATA   0xA1
#define PIC_M_SLAVE  0x04
#define PIC_S_CSCDE  0x02
#define PIC_M_OFSET  0x20
#define PIC_S_OFSET  0x28

#define PIC_DEF_MSK  0x00

#define PIC_CMD_EOI  0x20
#define PIC_CMD_INIT 0x11
#define PIC_CMD_x86  0x01

	void install_pic_idt() {
		install_idt(&pic0,32,IDT_TYPE_INTRPT,0);
		install_idt(&pic1,33,IDT_TYPE_INTRPT,0);
		install_idt(&pic2,34,IDT_TYPE_INTRPT,0);
		install_idt(&pic3,35,IDT_TYPE_INTRPT,0);
		install_idt(&pic4,36,IDT_TYPE_INTRPT,0);
		install_idt(&pic5,37,IDT_TYPE_INTRPT,0);
		install_idt(&pic6,38,IDT_TYPE_INTRPT,0);
		install_idt(&pic7,39,IDT_TYPE_INTRPT,0);
		install_idt(&pic8,40,IDT_TYPE_INTRPT,0);
		install_idt(&pic9,41,IDT_TYPE_INTRPT,0);
		install_idt(&pic10,42,IDT_TYPE_INTRPT,0);
		install_idt(&pic11,43,IDT_TYPE_INTRPT,0);
		install_idt(&pic12,44,IDT_TYPE_INTRPT,0);
		install_idt(&pic13,45,IDT_TYPE_INTRPT,0);
		install_idt(&pic14,46,IDT_TYPE_INTRPT,0);
		install_idt(&pic15,47,IDT_TYPE_INTRPT,0);
	}

	void init_pic() {
		outb(PIC_M_CMD,PIC_CMD_INIT);
		outb(PIC_S_CMD,PIC_CMD_INIT);

		outb(PIC_M_DATA,PIC_M_OFSET);
		outb(PIC_S_DATA,PIC_S_OFSET);

		outb(PIC_M_DATA,PIC_M_SLAVE);
		outb(PIC_S_DATA,PIC_S_CSCDE);

		outb(PIC_M_DATA,PIC_CMD_x86);
		outb(PIC_S_DATA,PIC_CMD_x86);

		outb(PIC_M_DATA,PIC_DEF_MSK);
		outb(PIC_S_DATA,PIC_DEF_MSK);
	}

	void mask(int intrpt) {
		intrpt=intrpt&0xF;
		int data=PIC_M_DATA;
		if(intrpt>8) {
			data=PIC_S_DATA;
			intrpt-=8;
		}
		outb(data,inb(data)|(1<<intrpt));
	}

	void unmask(int intrpt) {
		intrpt=intrpt&0xF;
		int data=PIC_M_DATA;
		if(intrpt>8) {
			data=PIC_S_DATA;
			intrpt-=8;
		}
		outb(data,inb(data)&~(1<<intrpt));
	}
	pic_handle_t functions[16];
	extern "C" void pic_handle(stack_t s) {
		if(s.pic>7) {
			outb(PIC_S_CMD,PIC_CMD_EOI);
		}
		outb(PIC_M_CMD,PIC_CMD_EOI);
		if(functions[s.pic]!=0) {
			pic_handle_t handle=functions[s.pic];
			handle(s);
		}
	}

	void request_irq(int irq,pic_handle_t handler) {
		functions[irq]=handler;
		unmask(irq);
	}
}
