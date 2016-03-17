#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
volatile uint16_t *vga_mem=reinterpret_cast<uint16_t *>(0xB8000);
uint16_t index=0;

void putc(char c) {
	vga_mem[index++]=15<<8|c;
}
void puts(const char *s) {
	while(*s) {
		if(*s=='\n') {
			index += 80-index%80;
			continue;
		}
		if(index>=80*25)index=0;
		vga_mem[index++]=15<<8|*s++;
	}
}
void printf(const char *fmt,...) {
	va_list list,l2;
	va_start(list,fmt);
	va_copy(l2,list);
	int len=vsnprintf(nullptr,0,fmt,l2);
	va_end(l2);
	char* buf=(char *)__alloca(len+1);
	vsprintf(buf,fmt,list);
	puts(buf);
	va_end(list);
}
