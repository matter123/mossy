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
#include <_stdver.h>
#include <cstdlib>
#include <type_traits>
namespace std {
template <class T> class weak_ptr;
template <class T> class shared_ptr {
  public:
	typedef typename details::control_block<T>::element_type element_type;
	typedef weak_ptr<T> weak_type;

  protected:
	element_type *pointer;
	details::control_block<T> *control;
	template <class Y> constexpr shared_ptr(Y *ptr, details::control_block<Y> *ctrl) : pointer(ptr), control(ctrl) {
		control->grab();
	}

  public:
	void reset() {
		control->release();
		if(control->can_delete()) { delete control; }
		pointer = nullptr;
		control = &details::nullptrctrl<T>;
	}
	// constructors
	constexpr shared_ptr() : pointer(nullptr), control(&details::nullptrctrl<T>) {}
	constexpr shared_ptr(std::nullptr_t) : pointer(nullptr), control(&details::nullptrctrl<T>) {}
	template <class Y> explicit shared_ptr(Y *ptr) : pointer(nullptr) {
		control = new details::control_block<Y>{ptr};
		control->grab();
	}
	template <class Y> shared_ptr(const shared_ptr<Y> &r, element_type *ptr) : pointer(ptr), control(r.control) {
		control->grab();
	}
	shared_ptr(const shared_ptr &r) : pointer(r.pointer), control(r.control) { control->grab(); }
	template <class Y> shared_ptr(const shared_ptr<Y> &r) : pointer(r.pointer), control(r.control) { control->grab(); }

	shared_ptr(shared_ptr &&r) {
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
	}
	template <class Y> shared_ptr(shared_ptr<Y> &&r) {
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
	}
	template <class Y> explicit shared_ptr(const std::weak_ptr<Y> &r) {
		if(r.use_count()) {
			pointer(r.pointer);
			control(r.control);
		} else {
			pointer(nullptr);
			control(&details::nullptrctrl<T>);
		}
		control->grab();
	}
	// shared_ptr doesn't support deleters so you can only construct a shared_ptr
	// from a unique_ptr that has a default_delete as iyts deleter
	template <class Y> shared_ptr(std::unique_ptr<Y, std::default_delete<Y>> &&r) {
		control = new details::control_block<Y>{r.release()};
		control->grab();
	}
	// deconstructor
	~shared_ptr() { reset(); }
	// operator='s
	shared_ptr &operator=(const shared_ptr &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab();
		return *this;
	}
	template <class Y> shared_ptr &operator=(const shared_ptr<Y> &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab();
	}
	shared_ptr &operator=(const shared_ptr &&r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
		return *this;
	}
	template <class Y> shared_ptr &operator=(const shared_ptr<Y> &&r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
		return *this;
	}
	template <class Y> shared_ptr &operator=(std::unique_ptr<Y, std::default_delete<Y>> &&r) {
		reset();
		control = new details::control_block<Y>{r.release()};
		control->grab();
		return *this;
	}
	void swap(shared_ptr &r) {
		element_type *ptr(pointer);
		details::control_block<T> *ctrl(control);
		pointer = r.pointer;
		control = r.control;
		r.pointer = ptr;
		r.control = ctrl;
	}
	element_type *get() const { return pointer ? pointer : control->pointer; }
	typename std::enable_if<!std::is_array<T>::value, T &>::type operator*() const {
		return pointer ? *pointer : *control->pointer;
	}
	typename std::enable_if<!std::is_array<T>::value, T *>::type operator->() const {
		return pointer ? pointer : control->pointer;
	}
#if(__CPP17)
	typename std::enable_if<std::is_array<T>::value, element_type &>::type operator[](std::ptrdiff_t idx) {
		return pointer ? pointer[idx] : control->pointer[idx];
	}
#endif
	long use_count() const { return control->shared_count; }
	bool unique() const { return use_count() == 1; }
	explicit operator bool() const { return !!pointer || !!control->shared_count; }
};

template <class T> class weak_ptr : protected shared_ptr<T> {
  public:
	typedef typename details::control_block<T>::element_type element_type;
	void reset() {
		this->control->release_weak();
		if(this->control->can_delete()) { delete this->control; }
		this->pointer = nullptr;
		this->control = &details::nullptrctrl<T>;
	}
	void swap(weak_ptr &r) {
		element_type *ptr(this->pointer);
		details::control_block<T> *ctrl(this->control);
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = ptr;
		r.control = ctrl;
	}
	long use_count() const { return this->control->shared_count; }
	bool expired() const { return use_count() == 0; }
	shared_ptr<T> lock() const {
		if(expired()) { return shared_ptr<T>(); }
		return shared_ptr<T>(*this);
	}
	constexpr weak_ptr() : shared_ptr<T>() {}
	weak_ptr(const weak_ptr &r) {
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
	}
	template <class Y> weak_ptr(const weak_ptr<Y> &r) {
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
	}
	template <class Y> weak_ptr(const std::shared_ptr<Y> &r) {
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
	}
	weak_ptr(weak_ptr &&r) {
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = &details::nullptrctrl<T>;
	}
	template <class Y> weak_ptr(weak_ptr<Y> &&r) {
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = &details::nullptrctrl<T>;
	}
	~weak_ptr() { reset(); }
	weak_ptr &operator=(const weak_ptr &r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
		return *this;
	}
	template <class Y> weak_ptr &operator=(const weak_ptr<Y> &r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
		return *this;
	}
	template <class Y> weak_ptr &operator=(const shared_ptr<Y> &r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
		return *this;
	}
	weak_ptr &operator=(weak_ptr &&r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = &details::nullptrctrl<T>;
		return *this;
	}
	template <class Y> weak_ptr &operator=(weak_ptr<Y> &&r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = &details::nullptrctrl<T>;
		return *this;
	}
};
}
#endif
