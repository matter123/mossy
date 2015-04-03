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

#pragma once
/**
 * @file test.h
 * @brief testing framework
 * @details provides basic utilities to perform dependant unit tests at run-time
 *
 * @note it is recommended to wrap your testing implementation in #ifdef TEST ... #endif
 */
#include <stdint.h>
#include <hal/console.h>
#include <sys/text_color.h>
namespace kernel {
	/**
	 * @brief runs all tests
	 * @details runs all the test in the correct order to satisfy dependencies
	 * @date created on 2015-02-07
	 */
	void test();
	/**
	 * @def unit_test
	 * @brief run unit test
	 * @details runs the unit test in \a res and compares the result to \a exp
	 * prints result on screen and halts if the test fails
	 *
	 * @param name the name to print for this unit test
	 * @param res the expression to test
	 * @param exp the expected result of evaluating \a res
	 *
	 * @date created on 2015-02-07
	 */
#define unit_test(name, res, exp)\
	 do {\
	 	auto __name=name;\
	 	auto __res=res;\
	 	auto __exp=exp;\
	 	__unit_test(__name, __res, __exp);\
	 } while(0)
#define __unit_test(name, res, exp)\
	do{if((res)==(exp))hal::cout<<"  "<<(name)<<std::TC::GREEN<<" passed"<<std::TC::RESET<<hal::endl;\
		else {hal::cout<<"  "<<(name)<<std::TC::RED<<" failed"<<std::TC::RESET\
			               <<" expected ("<<(exp)<<") got ("<<(res)<<")"<<hal::endl;\
			kernel::panic((name));}}while(0)

	/**
	 * @class test_module
	 * @brief a class to store information about a test module
	 * @details this class provides the bulk of the magic required to make unit tests work
	 *
	 * @note the standard implementation of a test module is to have a static pointer
	 * to this class and initalize it pointing to a static function which runs the test, and the
	 * constructors are built around that.
	 */
	class test_module {
			/** the name of the module */
			const char *name;
			/** a string of module names that this module depends on */
			const char *depends;
			/** function to run during test */
			void (*test_func)();
			/** the next module in the linked list */
			test_module *next;
			/** an indicator if this module has already passed it checks */
			bool passed;
		public:
			/**
			 * @brief constructs a test_module with no dependencies
			 *
			 * @param test_name a 8 byte string of the name of the module
			 * @param test_function the function to call to run the modules tests
			 *
			 * @date craeted on 2015-02-07
			 */
			test_module(const char *test_name, void (*test_function)());
			/**
			 * @brief constructs a test module with dependenencies
			 *
			 * @param test_name a 8 byte string of the name of the module
			 * @param test_function the function to call to run the modules tests
			 * @param dependencies a string of module names this module depends
			 * each name should be padded so they are 8 bytes long
			 *
			 * @date craeted on 2015-02-07
			 */
			test_module(const char *test_name, void (*test_function)(), const char *dependencies);
			/**
			 * @brief runs the tests
			 * @details searches the linked list for the depends and runs all depends before calling
			 * \a test_func and finally sets \a passed to true
			 */
			void run_test();
			/**
			 * @brief gets the next test_module in the linked list
			 * @return the next test_module
			 */
			test_module *get_next();
	};
}
