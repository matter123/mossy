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
#include <utf8.h>
#if TEST
#include <test.h>
#include <stdlib.h>
void test_strcmp()__attribute__((used));
static kernel::test_module test_module=kernel::test_module("STRCMP  ",&test_strcmp);
void test_strcmp() {
	unit_test("string compare #1",strcmp("apple","apple"),0);
	unit_test("string compare #2",strcmp("apple","apples"),-115);
	unit_test("string compare #3",strcmp("appleA","apples"),-50);
	unit_test("string compare #4",strcmp("appleA","apple"),65);
	unit_test("string compare #5",strcmp("appleA","apple "),33);
	unit_test("string compare #6",strcmp("\0"," "),-20);
	unit_test("string compare #7",strcmp(" ","\0"),20);
	unit_test("strn compare   #1",strncmp("apple","apple",5),0);
	unit_test("strn compare   #2",strncmp("apple","apple",4),0);
	unit_test("strn compare   #1",strncmp("appld","apple",4),0);
	char test1[]={'a', 'p', 'p', 'l', 'e', '1', '2', '\0'};
	char test2[]={'a', 'p', 'p', 'l', 'e', '1', '2', '\0'};
	char test3[]={'a', 'p', 'p', 'l', 'e', '1', '3', '\0'};
	unit_test("memcmp test    #1",memcmp(test1,test1,7),0);
	unit_test("memcmp test    #2",memcmp(test1,test2,7),0);
	unit_test("memcmp test    #2",memcmp(test1,test3,7),-1);
	unit_test("memcmp test    #2",memcmp(test3,test1,7),1);
}
void test_mbscmp()__attribute__((used));
static kernel::test_module test_mod=kernel::test_module("MBSCMP  ",&test_strcmp,"STRCAT  UNICODE ");
void test_mbscmp() {
	auto test1="\xC0\xA0";
	auto test2=" ";
	auto test2a=" ";
	auto test3="\xE0\x80\xA0";
	unit_test("mbstr compare  #1",mbscmp(test2,test2),0);
	unit_test("mbstr compare  #2",mbscmp(test2,test2a),0);
	unit_test("mbstr compare  #3",mbscmp(test2,test1),0);
	unit_test("mbstr compare  #4",mbscmp(test1,test2),0);
	unit_test("mbstr compare  #5",mbscmp(test2,test3),0);
	unit_test("mbstr compare  #6",mbscmp(test3,test2),0);
	unit_test("mbstr compare  #7",mbscmp(test1,test3),0);
	unit_test("mbstr compare  #8",mbscmp(test3,test1),0);
}
#endif
extern "C" {
	int memcmp(const void *ptr1, const void *ptr2, size_t num) {
		const uint8_t *p1=(const uint8_t *)ptr1;
		const uint8_t *p2=(const uint8_t *)ptr2;
		while(num--) {
			if(*p1!=*p2) {
				return *p1-*p2;
			}
			p1++;
			p2++;
		}
		return 0;
	}
	int strcmp(const char *str1, const char *str2) {
		while(*str1&&*str1==*str2) {
			str1++;
			str2++;
		}
		if(*str1==*str2) {
			return 0;
		}
		return *str1-*str2;
	}
	int mbscmp(const char *str1, const char *str2) {
		while(*str1 && *str2) {
			if(unicode::utf8::is_char_equal(str1,str2)) {
				str1=unicode::utf8::next_char(str1);
				str2=unicode::utf8::next_char(str2);
			} else {
				break;
			}
		}
		return unicode::utf8::decode_char(str1)-unicode::utf8::decode_char(str2);
	}
	int strncmp(const char *str1, const char *str2, size_t num) {
		while(num--) {
			if(*str1&&*str1==*str2) {
				if(!*str1) {
					return 0;
				}
				str1++;
				str2++;
			} else {
				return *str1-*str2;
			}
		}
		return 0;
	}
	int mbsncmp(const char *str1, const char *str2, size_t num) {
		while(*str1 && *str2 && num) {
			int len=unicode::utf8::get_char_len(str1);
			if(len>num) {
				return 0;
			}
			if(unicode::utf8::is_char_equal(str1,str2)) {
				str1=unicode::utf8::next_char(str1);
				str2=unicode::utf8::next_char(str2);
				num-=len;
			} else {
				break;
			}
		}
		return unicode::utf8::decode_char(str1)-unicode::utf8::decode_char(str2);
	}
}
