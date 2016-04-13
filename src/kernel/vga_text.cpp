#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <cpuid.h>
volatile uint16_t *vga_mem=reinterpret_cast<uint16_t *>(0xB8000);
#define PORT 0x3f8
uint16_t x=0,y=0;
int is_transmit_empty() {
   return inb(PORT + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);

   outb(PORT,a);
}
void putc(char c) {
	write_serial(c);
	if(x>=80||c=='\n') {
		x=0;
		y++;
		if(c=='\n') {
			return;
		}
	}
	vga_mem[y*80+x++]=7<<8|c;
}
void puts(const char *s) {
	while(*s) {
		if(x>=80||*s=='\n') {
			x=0;
			y++;
			if(*s=='\n') {
				write_serial(*s);
				s++;
				continue;
			}
		}
		if(y>25) {
			y=0;
			x=0;
			for(int i=0;i<80;i++) {
				vga_mem[0*80+i]=7<<8|' ';
			}
		}
		write_serial(*s);
		vga_mem[y*80+x++]=7<<8|*s++;
	}
}
void printf(const char *fmt,...) {
	va_list list,l2;
	va_start(list,fmt);
	va_copy(l2,list);
	int len=vsnprintf(nullptr,0,fmt,l2);
	va_end(l2);
	char* buf=(char *)__alloca(len+1);
	vsnprintf(buf,len+1,fmt,list);
	puts(buf);
	va_end(list);
}
