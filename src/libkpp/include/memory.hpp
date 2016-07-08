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
#ifndef __MEMORY_HPP
#define __MEMORY_HPP
#include <cstddef>

#include <_memory/shared_ptr>
#include <_memory/unique_ptr>
namespace std {
template <class T> T *addressof(T &arg) {
	return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}
void *align(std::size_t alignment, std::size_t size, void *&ptr, std::size_t &space);
}
#endif
