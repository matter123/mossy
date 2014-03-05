/*
 * Copyright 2014 Matthew Fosdick

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
namespace std {
	enum debug_level {
		dl_critical,
		dl_error,
		dl_warning,
		dl_info,
		dl_off,
	};

	void debug(debug_level level,c_cstring s);

	void debug_report_level(debug_level level);

	void error(c_cstring s);

	void cls();

	void strace(uint32_t depth) __attribute__((noreturn));

	void assert(bool condition) __attribute__((noreturn));
}