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
#include <_utility/pointer_control_block>
#include <_utility/unique_ptr>
#include <cstdlib>
#include <type_traits>
namespace std {
template <class T> class weak_ptr;
template <class T> class shared_ptr;

template <class T> class weak_ptr {
  public:
	typedef typename details::control_block<T>::element_type element_type;

  private:
	element_type *pointer;
	details::control_block<T> *control;

  public:
	constexpr weak_ptr() : pointer(nullptr), control(&details::nullptrctrl) {}
	weak_ptr(const weak_ptr &r) : pointer(r.pointer), control(r.control) { control->grab_weak(); }
	template <class Y> weak_ptr(const weak_ptr<Y> &r) : pointer(r.pointer), control(r.control) { control->grab_weak(); }
	template <class Y> weak_ptr(const std::shared_ptr<Y> &r); // NOTE: NEED TO IMP
	weak_ptr(weak_ptr &&r) : pointer(r.pointer), control(r.control) {
		r.pointer = nullptr;
		r.control = &details::nullptrctrl;
	}
	template <class Y> weak_ptr(weak_ptr<Y> &&r) : pointer(r.pointer), control(r.control) {
		control->grab_weak();
		r.reset();
	}
	~weak_ptr() { reset(); }
	weak_ptr &operator=(const weak_ptr &r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
	}
	template <class Y> weak_ptr &operator=(const weak_ptr<Y> &r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
	}
	template <class Y> weak_ptr &operator=(const shared_ptr<Y> &r); // NOTE: NEED TO IMP
	weak_ptr &operator=(weak_ptr &&r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
		r.reset();
	}
	template <class Y> weak_ptr &operator=(weak_ptr<Y> &&r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
		r.reset();
	}
	void reset() {
		control->release_weak();
		if(control->can_delete()) delete control;
		control = &details::nullptrctrl;
	}
	void swap(weak_ptr &r) {
		auto tp = pointer;
		auto tc = control;
		pointer = r.pointer;
		control = r.control;
		r.pointer = tp;
		r.control = tc;
	}
	long use_count() const { return control->shared_count; }
	bool expired() const { return !control->is_valid(); }
	std::shared_ptr<T> lock() const;                                      // NOTE: NEED TO IMP
	template <class Y> bool owner_before(const weak_ptr<Y> &other) const; // NOTE: no idea what these do
	template <class Y> bool owner_before(const std::shared_ptr<Y> &other) const;
};

template <class T> class shared_ptr {
  public:
	typedef typename details::control_block<T>::element_type element_type;

  private:
	element_type *pointer;
	details::control_block<T> *control;

  public:
	constexpr shared_ptr() : pointer(nullptr), control(&details::nullptrctrl) {}
	constexpr shared_ptr(nullptr_t) : pointer(nullptr), control(&details::nullptrctrl) {}
	template <class Y> explicit shared_ptr(Y *ptr) : pointer(ptr), control(new details::control_block(ptr)) {
		control->grab();
	}
	template <class Y, class Deleter>
	shared_ptr(Y *ptr, Deleter d) : pointer(ptr), control(new details::control_block(ptr, d)) {
		control->grab();
	}
	template <class Deleter>
	shared_ptr(std::nullptr_t ptr, Deleter d) : pointer(ptr), control(new details::control_block(ptr, d)) {
		control->grab();
	}
	~weak_ptr() { reset(); }
	weak_ptr &operator=(const weak_ptr &r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
	}
	template <class Y> weak_ptr &operator=(const weak_ptr<Y> &r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
	}
	template <class Y> weak_ptr &operator=(const shared_ptr<Y> &r); // NOTE: NEED TO IMP
	weak_ptr &operator=(weak_ptr &&r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
		r.reset();
	}
	template <class Y> weak_ptr &operator=(weak_ptr<Y> &&r) {
		pointer = r.pointer;
		control = r.control;
		control->grab_weak();
		r.reset();
	}
	void reset() {
		control->release_weak();
		if(control->can_delete()) delete control;
		control = &details::nullptrctrlelement_type * pointer;
	}
	void swap(weak_ptr &r) {
		auto tp = pointer;
		auto tc = control;
		pointer = r.pointer;
		control = r.control;
		r.pointer = tp;
		r.control = tc;
	}
	long use_count() const { return control->shared_count; }
	bool expired() const { return !control->is_valid(); }
	std::shared_ptr<T> lock() const;                                      // NOTE: NEED TO IMP
	template <class Y> bool owner_before(const weak_ptr<Y> &other) const; // NOTE: no idea what these do
	template <class Y> bool owner_before(const std::shared_ptr<Y> &other) const;
};
}
#endif
