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
	va_list list;
	va_start(list,fmt);
	int len=vsprintf(nullptr,fmt,list);
	char* buf=(char *)__alloca(len);
	vsprintf(buf,fmt,list);
	puts(buf);
	va_end(list);
}
