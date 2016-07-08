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
#ifndef __SHARED_PTR_HPP
#define __SHARED_PTR_HPP
#include <_memory/pointer_control_block>
#include <_memory/unique_ptr>
#include <cstdlib>
#include <type_traits>
namespace std {
template <class T, class D = default_delete<nullptr_t>> class weak_ptr;
template <class T, class D = default_delete<nullptr_t>> class shared_ptr;
template <class T, class D> class weak_ptr {
  public:
	typedef typename details::control_block<T, D>::element_type element_type;

  private:
	element_type *pointer;
	details::control_block<T, D> *control;

  public:
	constexpr weak_ptr() : pointer(nullptr), control(details::nullptrctrl<D>) {}
	weak_ptr(const weak_ptr &r) : pointer(r.pointer), control(r.control) { control->grab_weak(); }
	template <class Y> weak_ptr(const weak_ptr<Y, D> &r) : pointer(r.pointer), control(r.control) {
		control->grab_weak();
	}
	template <class Y, class E> weak_ptr(const weak_ptr<Y, E> &r) : pointer(r.pointer), control(r.control) {
		control->grab_weak();
	}
	template <class Y> weak_ptr(const shared_ptr<Y, D> &r) : pointer(r.pointer), control(r.control) {
		control->grab_weak();
	}
	template <class Y, class E> weak_ptr(const shared_ptr<Y, E> &r) : pointer(r.pointer), control(r.control) {
		control->grab_weak();
	}
	template <class Y> weak_ptr(weak_ptr<Y> &&r) : pointer(r.pointer), control(r.control) { r.reset(); }
	template <class Y, class E> weak_ptr(weak_ptr<Y, E> &&r) : pointer(r.pointer), control(r.control) { r.reset(); }
	void reset() {
		pointer = nullptr;
		control->release_weak();
		if(control->can_delete) {
			delete control;
			control = details::nullptrctrl<D>;
		}
	}
	~weak_ptr() { reset(); }
	weak_ptr &operator=(const weak_ptr &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab_weak();
		return *this;
	}
	template <class Y> weak_ptr &operator=(const weak_ptr<Y, D> &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab_weak();
		return *this;
	}
	template <class Y, class E> weak_ptr &operator=(const weak_ptr<Y, E> &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab_weak();
		return *this;
	}
	template <class Y> weak_ptr &operator=(const shared_ptr<Y, D> &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab_weak();
		return *this;
	}
	template <class Y, class E> weak_ptr &operator=(const shared_ptr<Y, E> &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab_weak();
		return *this;
	}
	weak_ptr &operator=(weak_ptr &&r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		r.reset();
		return *this;
	}
	template <class Y> weak_ptr &operator=(weak_ptr<Y, D> &&r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		r.reset();
		return *this;
	}
	template <class Y, class E> weak_ptr &operator=(weak_ptr<Y, E> &&r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		r.reset();
		return *this;
	}
	void swap(weak_ptr &r) {
		element_type *ptr = pointer;
		details::control_block<T, D> *ctrl = control;
		pointer(r.pointer);
		control(r.control);
		r.pointer(ptr);
		r.control(ctrl);
	}
	long use_count() const { return control->shared_count; }
	bool expired() const { return use_count() == 0; }
};
template <class T, class D> class shared_ptr {};
}
#endif
