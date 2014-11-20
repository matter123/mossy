/*
    Copyright 2013-2014 Matthew Fosdick

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
#include <hal/console.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unicode.h>
#include <utf8.h>
#include <text_color.h>
#include <test.h>
#include <hal/hal.h>


namespace kernel {
	test_module::test_module(char *f_name,void (*func)()) {
		hal::magic_break();
	}
	void test() {
		hal::cls();
		bool abrt=false;
		unit_test("unit test, pass  ", 7 ,7);
		unit_test("unit test, fail  ", 7 ,77);
		abrt=true;
		unit_test("string compare #1",strcmp("apple","apple"),0);
		unit_test("string compare #2",strcmp("apple","apples"),-115);
		unit_test("string compare #3",strcmp("appleA","apples"),-50);
		unit_test("string compare #4",strcmp("appleA","apple"),65);
		unit_test("string compare #5",strcmp("appleA","apple "),33);
		char foo[]="foo\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
		unit_test("string copy    #1",strcmp(strcpy(foo,"bar"),"bar"),0);
		unit_test("string cat     #1",strcmp(strcat(foo,"bar"),"barbar"),0);
		unit_test("string cat     #2",strcmp(strncat(foo,"bar",1),"barbarb"),0);
		unit_test("unicode len    #1",strlen("Áﬃcion𝐚ḍ̇o"),20);
		unit_test("unicode len    #2",mbslen("Áﬃcion𝐚ḍ̇o"),11);
		unit_test("unicode len    #3",get_char_len("ﬃ"),3);
		void *h1=malloc(4);
		free(h1);
		void *h2=malloc(4);
		unit_test("heap merge     #1",h2,h1);
		free(h2);
		void *bar[20];
		for(int i=0; i<20; i++) {
			bar[i]=malloc(4*(i+1));
		}
		for(int i=1; i<20; i+=2) {
			free(bar[i]);
		}
		for(int i=0; i<20; i+=2) {
			free(bar[i]);
		}
		h1=malloc(840);
		unit_test("heap merge     #2",h1,h2);
		free(h1);
	}
}