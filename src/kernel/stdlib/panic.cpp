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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
NORETURN
extern "C"
void panic(const char *message) {
	volatile uint16_t *vga_mem=reinterpret_cast<uint16_t *>(0xB8000);
	for(int i=0;i<80*25;i++)vga_mem[i]=0x4F00;
	const char *panic="PANIC PANIC PANIC PANIC PANIC PANIC PANIC PANIC PANIC PANIC PANIC PANIC PANIC PA";
	int i=0;
	while(*panic) vga_mem[i++]=0xF400|*panic++;
	i=80*2+2;
	while(*message) vga_mem[i++]|=*message++;
	while(1);
}
NORETURN
extern "C"
void panic_fn(const char *message,const char *func,const char *file, int line) {
	const char *fmt = "panic in function '%s' on line %d in file '%s': %s";
	int size = strlen(fmt)+strlen(message)+strlen(func)+strlen(file)+5;
	char *alloc = (char *)__alloca(size);
	sprintf(alloc,fmt,func,line,file,message);
	panic(alloc);
}
NORETURN
extern "C"
void assertf(const char *message) {
	const char *fmt = "assertion failed: %s";
	int size = strlen(fmt)+strlen(message)+5;
	char *alloc = (char *)__alloca(size);
	sprintf(alloc,fmt,message);
	panic(alloc);
}
