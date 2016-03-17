#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
volatile uint16_t *vga_mem=reinterpret_cast<uint16_t *>(0xB8000);
uint16_t x=0,y=0;

void putc(char c) {
	vga_mem[y*80+x]=15<<8|c;
}
void puts(const char *s) {
	while(*s) {
		if(x>=80||*s=='\n') {
			x=0;
			y++;
			if(*s=='\n') {
				s++;
				continue;
			}
		}
		if(y>25) {
			y=0;
			x=0;
		}
		vga_mem[y*80+x++]=15<<8|*s++;
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
