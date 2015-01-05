/*
    Copyright 2014 Matthew Fosdick

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
#include <unicode.h>
#include <iterators.h>
#include <utf8.h>
#if TEST
#include <test.h>
#include <stdlib.h>
void test_strcpy()__attribute__((used));
static kernel::test_module test_module=kernel::test_module("STRCPY  ",&test_strcpy,
                                                           "HEAP    UNICODE STRCMP  ");
void test_strcpy() {
	const char *a="apple";
	char *b=(char *)malloc(6);
	unit_test("strcpy         #1",strcmp(strcpy(b,a),a),0);
	const char *c="application";
	unit_test("strcpy         #2",strcmp(strlcpy(b,c,5),"appl"),0);
}

#endif
extern "C" {
	void *memcpy(void *dest, const void *src, size_t num) {
		uint8_t *d=(uint8_t *)dest;
		const uint8_t *s=(const uint8_t *)src;
		while(num--) {
			*d++=*s++;
		}
		return dest;
	}
	void *memmove(void *dest, const void *src, size_t num) {
		if(dest==src) {
			return dest;
		}
		if(dest>src) {
			uint8_t *d=((uint8_t *)dest)+(num-1);
			const uint8_t *s=((const uint8_t *)src)+(num-1);
			while(num--) {
				*d--=*s--;
			}
		} else {
			uint8_t *d=(uint8_t *)dest;
			const uint8_t *s=(const uint8_t *)src;
			while(num--) {
				*d++=*s++;
			}
		}
		return dest;
	}
	char *strcpy(char *dest,const char *src) {
		char *d=dest;
		while(*src) {
			*dest++=*src++;
		}
		*dest='\0';
		return d;
	}
	char *strncpy(char *dest, const char *src, size_t num) {
		bool zero=false;
		char *d=dest;
		while(num--) {
			if(!zero) {
				*dest=*src++;
				if(!*dest++) {
					zero=true;
				}
				continue;
			}
			*dest='\0';
		}
		return d;
	}
	char *strlcpy(char *dest, const char *src, size_t num) {
		unicode::codeunits src_cu(const_cast<char *>(src));
		unicode::codeunits dest_cu(dest);
		for(char *cu=src_cu.begin(); src_cu.has_next()&&num>1; cu=src_cu.next()) {
			num-=unicode::utf8::get_char_len(cu);
			if(num>1) {
				unicode::copy_codeunit(dest_cu.value(),cu);
				dest_cu.next();
				continue;
			}
			break;
		}
		*(dest_cu.next())='\0';
		return dest;
	}
	char *mbsncpy(char *dest, const char *src, size_t num) {
		unicode::codeunits src_cu(const_cast<char *>(src));
		unicode::codeunits dest_cu(dest);
		for(char *cu=src_cu.begin(); src_cu.has_next()&&num>1; cu=src_cu.next()) {
			num-=unicode::utf8::get_char_len(cu);
			if(num>1) {
				unicode::copy_codeunit(dest_cu.value(),cu);
				dest_cu.next();
				continue;
			}
			break;
		}
		while(num>1) {
			*(dest_cu.value())='\0';
			dest_cu.next();
		}
		*(dest_cu.value())='\0';
		return dest;
	}
}
