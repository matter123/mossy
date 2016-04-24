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
#include <string.h>
#include <vga_text.h>
#include <sys/thread_info.h>


const int EXCEPTION_COUNT = 32;
void (*exceptions[EXCEPTION_COUNT])(cpu_state *);
interrupt_handler handlers[256-EXCEPTION_COUNT][8];

extern "C" const char exc_arr[256][64];
extern "C" idt_entry IDT[256];
extern "C" uintptr_t jump_table[256];
extern "C" uintptr_t jump_table2[256];


extern "C" int lidtr();
extern "C" void exception_handler();
extern "C" void isr_handler();


void set_entry(int int_num,idt_entry *entry) {
	memcpy(&IDT[int_num],entry,sizeof(idt_entry));
}
void set_entry(int int_num, void *target,int type,bool userspace) {
	idt_entry entry;
	memset(&entry,0,sizeof(idt_entry));
	entry.type=type;
	entry.present=true;
	entry.dpl=userspace?0x3:0x0;
	entry.target_cs=0x8;
	entry.target_low=(uintptr_t)target&0xFFFF;
	entry.target_mid=((uintptr_t)target>>16)&0xFFFF;
	entry.target_high=((uintptr_t)target>>32)&0xFFFFFFFF;
	set_entry(int_num, &entry);
}
void install_JT1(int int_num,void *target) {
	jump_table[int_num]=(uintptr_t)target;
}
void install_JT2(int int_num,void *target) {
	jump_table2[int_num]=(uintptr_t)target;
}

void error_log(cpu_state *s);
void def_handler(cpu_state *s);
void install_interrupts() {
	for(int i=0;i<256;i++) {
		set_entry(i,(void *)exc_arr[i],INT_INT_GATE,false);
		if(i<EXCEPTION_COUNT) {
			install_JT1(i, reinterpret_cast<void *>(exception_handler));
			install_JT2(i, reinterpret_cast<void *>(def_handler));
			exceptions[i]=error_log;
		} else {
			install_JT1(i, reinterpret_cast<void *>(isr_handler));
			install_JT2(i, reinterpret_cast<void *>(def_handler));
			memset(handlers[i-EXCEPTION_COUNT], 0, sizeof(interrupt_handler)*8);
		}
	}
	lidtr();
}
void def_handler(cpu_state *s) {
	if(s->int_num<EXCEPTION_COUNT) {
		//exceptions are simple
		exceptions[s->int_num](s);
	} else {
		//store the cpu_dtate structure pointer
		thread_info *info=get_current_thread_info();
		if(s->cs&0x3) {
			//in userspace
			info->userspace=s;
		} else {
			//in kernel space
			if(info->kinterrupt) {
				Log(LOG_ERROR, "[INTRPT]", "intrrupt trying to be handled whilst in interrupt");
			}
			info->kinterrupt=s;
		}
		for(int i=0;i<8;i++) {
			interrupt_handler handle=handlers[s->int_num-32][i];
			if(handle.handler) {
				handle.handler(handle.context);
			}
		}
		if(s->cs&0x3) {
			info->userspace=nullptr;
		} else {
			//in kernel space
			if(info->kinterrupt!=s) {
				Log(LOG_ERROR, "[INTRPT]", "interrupt returning from unexpected spot");
			}
			info->kinterrupt=nullptr;
		}
	}
}
void error_log(cpu_state *s) {
	printf("%d(%s) err: %d rflags: %x cs: %x\n",s->int_num,s->dbg,s->err_code,s->rflags,s->cs);
	printf("rip:%.16p rsp:%.16p\n",s->rip,s->rsp);
	printf("rax:%.16p rbx:%.16p\n",s->rax,s->rbx);
	printf("rcx:%.16p rdx:%.16p\n",s->rcx,s->rdx);
	printf("rsi:%.16p rdi:%.16p\n",s->rsi,s->rdi);
	printf("r8 :%.16p r9 :%.16p\n",s->r8, s->r9);
	printf("r10:%.16p r11:%.16p\n",s->r10,s->r11);
	printf("r12:%.16p r13:%.16p\n",s->r12,s->r13);
	printf("r14:%.16p r15:%.16p\n",s->r14,s->r15);
	while("true");
}

void install_exception(int int_num,void (*handler)(cpu_state *)) {
	exceptions[int_num]=handler;
}
void install_handler(int int_num,void (*handler)(void *context),void *context) {
	for(int i=0;i<8;i++) {
		interrupt_handler handle=handlers[int_num-32][i];
		if(handle.handler) {
			continue;
		}
		handlers[int_num-32][i].handler=handler;
		handlers[int_num-32][i].context=context;
		break;
	}
}
