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
#ifndef __CSTDDEF_HPP
#define __CSTDDEF_HPP
#include <stddef.h>
namespace std {
using ::size_t;
using ::ptrdiff_t;
typedef decltype(nullptr) nullptr_t;
typedef long double max_align_t;
}
#endif
