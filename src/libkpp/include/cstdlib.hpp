/*
	Copyright 2016 Matthew Fosdick

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
#ifndef __CSTDLIB_HPP
#define __CSTDLIB_HPP
#include <stdlib.h>
namespace std {
using ::div_t;
using ::ldiv_t;
using ::lldiv_t;
using ::size_t;
using ::abort;
using ::malloc;
using ::calloc;
using ::realloc;
using ::free;
using ::atof;
using ::atoi;
using ::atol;
using ::atoll;
using ::strtol;
// using ::strtoll;
using ::strtoul;
// using ::strtoull;
// using ::strtof;
// using ::strtod;
// using ::strtold;
// using ::rand;
}
#endif
