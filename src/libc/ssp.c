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
const uint64_t __stack_chk_guard = 0xD5000AFF0DCD12D5;
NORETURN void __stack_chk_fail() {
#if IN_LIBC
	abort();
#elif IN_LIBK
	panic("Stack smashing detected");
#endif
}
