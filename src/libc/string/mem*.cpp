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

extern "C" void *memcpy(void *dest, const void *src, size_t size) {
	pointer_t pdest = (pointer_t)dest;
	const_pointer_t psrc = (const_pointer_t)src;
	while(size--) *pdest++ = *psrc++;
	return dest;
}
int memcmp(const void *mem1, const void *mem2, size_t num) {
	const_pointer_t p1 = (const_pointer_t)mem1;
	const_pointer_t p2 = (const_pointer_t)mem2;
	while(num--) {
		if(*p1 != *p2) { return *p1 - *p2; }
		p1++;
		p2++;
	}
	return 0;
}
void *memset(void *dest, int value, size_t size) {
	pointer_t pdest = (pointer_t)dest;
	while(size--) *pdest++ = value;
	return dest;
}
void *memmove(void *dest, const void *src, size_t size) {
	pointer_t pdest = (pointer_t)dest;
	const_pointer_t psrc = (const_pointer_t)src;
	if(dest == src) return dest;
	if(pdest < psrc) {
		while(size--) *pdest++ = *psrc++;
	} else {
		while(size) *(pdest + (size - 1)) = *(psrc + (size - 1));
		size--;
	}
	return dest;
}
void *memset16(uint16_t *ptr, uint16_t value, size_t num) {
	uint16_t *ret = ptr;
	while(num--) { *ptr++ = value; }
	return ret;
}
