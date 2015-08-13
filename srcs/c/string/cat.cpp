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
void test_strcat()__attribute__((used));
static kernel::test_module test_module=kernel::test_module("STRCAT  ",&test_strcat,
                                                           "HEAP    UNICODE STRCMP  ");
void test_strcat() {
	const char *a="apple";
	char *b=(char *)malloc(6);
	unit_test("strcpy         #1",strcmp(strcpy(b,a),a),0);
	const char *c="application";
	unit_test("strcpy         #2",strcmp(strlcpy(b,c,5),"appl"),0);
}
#endif
extern "C" {
	char *strcat(char *dest, const char *src) {
		char *d=dest;
		while(*dest++);
		dest--;
		while(*src) {
			*dest++=*src++;
		}
		*dest='\0';
		return d;
	}
	char *strncat(char *dest,const char *src, size_t num) {
		char *d=dest;
		while(*dest++);
		dest--;
		while(*src&&num--) {
			*dest++=*src++;
		}
		*dest='\0';
		return d;
	}
	char *strlcat(char *dest, const char *src, size_t num) {
		char *d=dest;
		while(*dest++);
		dest--;
		while(num) {
			int len=unicode::utf8::get_char_len(src);
			if(num<len) {
				break;
			}
			unicode::utf8::char_copy(dest,src);
			dest+=len;
			src+=len;
			num-=len;
		}
		*dest='\0';
		return d;
	}
}
