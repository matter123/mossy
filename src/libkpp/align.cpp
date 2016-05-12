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
#include <memory>
namespace std {
void *align(std::size_t alignment, std::size_t size, void *&ptr, std::size_t &space) {
	uintptr_t p = reinterpret_cast<uintptr_t>(ptr);
	if(p & alignment - 1) {
		// not aligned
		std::size_t gap = alignment - (p % alignment);
		if(gap + size > space) {
			return nullptr;
		}
		space -= gap;
		p += gap;
		ptr = reinterpret_cast<void *>(p);
	}
	return ptr;
}
}
