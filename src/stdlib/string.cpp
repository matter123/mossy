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

#include <string.h>
#include <stdint.h>
namespace std {
	cstring strcat(cstring s1,c_cstring s2) {
		cstring s=s1;
		if(*(s1)) {
			while(*(++s1));
		}
		while ( (*s1++ = *s2++) );
		return s;
	}
	cstring strcpy(cstring s1,c_cstring s2) {
		cstring s=s1;
		while(*(s2))*(s1++)=*(s2++);
		*(s1)='\0';
		return s;
	}
	int    strlen(c_cstring s1) {
		int i=0;
		while(*(s1++))i++;
		return i;
	}
	bool   strcmp(c_cstring s1,c_cstring s2) {
		while (*s1 && *s1 == *s2 ) {
			++s1;++s2;
		}
		return *(s1) == *(s2);
	}
	void *memmove(void *dest,const void *src,size_t n) {
		int delta=0;
		uint32_t from=0;
		uint32_t to=0;
		if(dest==src)return dest;
		if(dest<src) {
			delta=1;
			from=0;
			to=n;
		}else {
			delta=-1;
			from=n-1;
			to =-1;
		}
		for(uint32_t offset=from;offset!=to;offset+=delta)
			*(((uint8_t*)dest) + offset) = *(((uint8_t*)src) + offset);
		return dest;
	}

	void *memcpy(void *dest,const void *src,size_t n) {
		uint8_t *s1=reinterpret_cast<uint8_t *>(dest);
		const uint8_t *s2=(uint8_t *)src;
    	while ( n-- ) {
        	*s1++ = *s2++;
    	}
    	return dest;
	}

	void *memset(void *addr,uint8_t c,size_t size) {
		uint8_t *addr8=(uint8_t *)addr;
		for(unsigned int i=0;i<size;i++) {
			*(addr8++)=c;
		}
		return addr;
	}
	void *memset16(uint16_t *addr,uint16_t c,size_t size) {
		uint16_t *addri=(uint16_t *)addr;
		for(unsigned int i=0;i<size;i++) {
				*(addri++)=c;
		}
		return addr;
	}
	void *memset32(uint32_t *addr,uint32_t c,size_t size) {
		uint32_t *addri=(uint32_t *)addr;
		for(unsigned int i=0;i<size;i++) {
			*(addri++)=c;
		}
		return addr;
	}
}
