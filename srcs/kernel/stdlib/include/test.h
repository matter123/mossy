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

#pragma once
#include <stdint.h>
#include <hal/console.h>
#include <text_color.h>
namespace kernel {
	void test();
#define unit_test(name, res, exp)\
	do{if((res)==(exp))hal::cout<<"  "<<(name)<<std::TC::GREEN<<" passed"<<std::TC::RESET<<hal::endl;\
		else {hal::cout<<"  "<<(name)<<std::TC::RED<<" failed"<<std::TC::RESET\
			               <<" expected ("<<(exp)<<") got ("<<(res)<<")"<<hal::endl;\
			kernel::panic((name));}}while(0)

	class test_module {
			const char *name;
			const char *depends;
			void (*test_func)();
			test_module *next;
			bool passed;
		public:
			test_module(const char *test_name, void (*test_function)());
			test_module(const char *test_name, void (*test_function)(), const char *dependencies);
			void run_test();
			test_module *get_next();
	};
}
