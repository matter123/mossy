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
	static test_module *first=NULL;
	test_module::test_module(const char *test_name, void (*test_function)()) {
		this->name=test_name;
		this->depends="";
		this->test_func=test_function;
		this->passed=false;
		this->next=first;
		first=this;
	}
	test_module::test_module(const char *test_name, void (*test_function)(), const char *dep) {
		this->name=test_name;
		this->depends=dep;
		this->test_func=test_function;
		this->passed=false;
		this->next=first;
		first=this;
	}
	void test_module::run_test() {
		if(this->passed) {
			return;
		}
		const char *dep=this->depends;
		test_module *mod=first;
		while(*dep!='\0') {
			if(!mod) {
				hal::cout<<std::TC::RED<<name<<std::TC::RESET<<hal::endl;
				kernel::panic(dep);
			}
			for(int i=0; i<8; i++) {
				if(mod->name[i]!=*(dep+i)) {
					mod=mod->next;
				}
			}
			mod->run_test();
			dep+=8;
			mod=first;
		}
		hal::cout<<name<<hal::endl;
		this->test_func();
		this->passed=true;
		hal::cout<<std::TC::GREEN<<"PASSED"<<std::TC::RESET<<hal::endl;
	}
	test_module *test_module::get_next() {
		return this->next;
	}
	void test() {
		test_module *cur=first;
		while(cur) {
			cur->run_test();
			cur=cur->get_next();
		}
		/*
		unit_test("unicode len    #1",strlen("Áﬃcion𝐚ḍ̇o"),20);
		unit_test("unicode len    #2",mbslen("Áﬃcion𝐚ḍ̇o"),11);
		unit_test("unicode len    #3",get_char_len("ﬃ"),3);*/
	}
}