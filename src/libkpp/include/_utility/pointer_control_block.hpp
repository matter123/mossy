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
#include <utility>
namespace std {
namespace details {

template <class T, class Deleter = default_delete<T>> struct control_block {
#if __CPP17
	typedef std::remove_extent_t<T> element_type;
#else
	typedef T element_type;
#endif
	element_type *pointer;
	Deleter del;
	std::size_t shared_count;
	std::size_t weak_count;
	control_block(element_type *ptr, Deleter d = Deleter()) {
		del = d;
		pointer = ptr;
		shared_count = 0;
		weak_count = 0;
	}
	void grab_weak() { weak_count++; }
	void release_weak() { weak_count--; }
	void grab() { shared_count++; }
	void release() {
		shared_count--;
		if(!shared_count && pointer) del(pointer);
	}
	bool is_valid() { return shared_count; }
	bool can_delete() { return !(shared_count || weak_count); }
};
template <class Deleter = default_delete<nullptr_t>> struct null_control_block : control_block<nullptr_t, Deleter> {
	null_control_block(nullptr_t *ptr, Deleter d = Deleter()) : control_block<nullptr_t, Deleter>(ptr, d) {}
	void grab_weak() {}
	void release_weak() {}
	void grab() {}
	void release() {}
	bool is_valid() { return false; }
	bool can_delete() { return false; }
};
template <class D> null_control_block<D> nullptrctrl(nullptr);
}
}
#endif
