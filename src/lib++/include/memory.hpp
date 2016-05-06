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
#pragma once
#include <stdlib.h>
namespace std {
template <class T> T *addressof(T &arg) {
	return reinterpret_cast<T *>(&const_cast<char &>(reinterpret_cast<const volatile char &>(arg)));
}
template <typename T> struct allocator {
  public:
	typedef T value_type;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef T &reference;
	typedef const T &const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	template <typename U> struct rebind { typedef std::allocator<U> other; };
	allocator() = 0;
	allocator(const allocator &other) = 0;
	template <class U> allocator(const allocator<U> &other){};
	~allocator() = 0;
	allocator<T>::pointer address(reference x) const { addressof(x); }
	allocator<T>::const_pointer address(const_reference x) const { addressof(x); };
	allocator<T>::pointer allocate(size_type n, const void *hint = 0) {
		return calloc(n, sizeof(T));
	}
	void deallocate(allocator<T>::pointer p, size_type n) { return free(p); }
	size_type max_size() const { return UINT32_MAX; }
	template <class U> void destroy(U *p) { p->~U(); }
};
template <> struct allocator<void> {
	typedef void value_type;
	typedef void *pointer;
	typedef const void *const_pointer;
};
}
