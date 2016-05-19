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
#ifndef __PTR_CB_HPP
#define __PTR_CB_HPP
#include <_stdver.h>
#include <cstdlib>
#include <type_traits>
namespace std {
template <class T> struct control_block {
#if __CPP17
	typedef std::remove_extent_t<T> element_type;
#else
	typedef T element_type;
#endif
	element_type *pointer;
	std::size_t shared_count;
	std::size_t weak_count;
	control_block(element_type *ptr, std::size_t s_count, std::size_t w_count) {
		pointer = ptr;
		shared_count = s_count;
		weak_count = w_count;
	}
	void grab_weak() { weak_count++; }
	void release_weak() { weak_count--; }
	void grab() { shared_count++; }
	void release() {
		shared_count--;
		if(!shared_count)
			delete pointer;
	}
	bool is_valid() { return shared_count; }
	bool can_delete() { return !(shared_count || weak_count); }
};
}
#endif
