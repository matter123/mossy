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
template <class T, class D = default_delete<nullptr_t>> class weak_ptr;
template <class T, class D = default_delete<nullptr_t>> class shared_ptr {
  public:
	typedef typename details::control_block<T, D>::element_type element_type;
	typedef weak_ptr<T, D> weak_type;

  protected:
	element_type *pointer;
	details::control_block<T, D> *control;
	template <class Y, class Del>
	constexpr shared_ptr(Y *ptr, details::control_block<Y, Del> *ctrl) : pointer(ptr), control(ctrl) {
		control->grab();
	}

  public:
	void reset() {
		control->release();
		if(control->can_delete()) { delete control; }
		pointer = nullptr;
		control = (details::control_block<T, D> *)&details::nullptrctrl<D>;
	}
	// constructors
	constexpr shared_ptr() : pointer(nullptr), control(details::nullptrctrl<D>) {}
	constexpr shared_ptr(std::nullptr_t) : pointer(nullptr), control(details::nullptrctrl<D>) {}
	template <class Y> explicit shared_ptr(Y *ptr) : pointer(nullptr) {
		control = new details::control_block<Y, D>{ptr};
		control->grab();
	}
	template <class Y, class Del> explicit shared_ptr(Y *ptr) : pointer(nullptr) {
		control = new details::control_block<Y, Del>{ptr};
		control->grab();
	}
	template <class Del> explicit shared_ptr(std::nullptr_t) : pointer(nullptr) {
		control = details::nullptrctrl<D>;
		control->grab();
	}
	template <class Y> shared_ptr(const shared_ptr<Y, D> &r, element_type *ptr) : pointer(ptr), control(r.control) {
		control->grab();
	}
	shared_ptr(const shared_ptr &r) : pointer(r.pointer), control(r.control) { control->grab(); }
	template <class Y> shared_ptr(const shared_ptr<Y, D> &r) : pointer(r.pointer), control(r.control) {
		control->grab();
	}
	// maybe need a <class Y, class Del> overload here
	shared_ptr(shared_ptr &&r) {
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
	}
	template <class Y> shared_ptr(shared_ptr<Y, D> &&r) {
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
	}
	template <class Y> explicit shared_ptr(const std::weak_ptr<Y, D> &r) {
		if(r.use_count()) {
			pointer(r.pointer);
			control(r.control);
		} else {
			pointer(nullptr);
			control(details::nullptrctrl<D>);
		}
		control->grab();
	}
	template <class Y, class Del> shared_ptr(std::unique_ptr<Y, Del> &&r) {
		control = new details::control_block<Y, Del>{r.release(), r.get_deleter()};
		control->grab();
	}
	// deconstructors
	~shared_ptr() { reset(); }
	// operator='s
	shared_ptr &operator=(const shared_ptr &r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab();
		return *this;
	}
	template <class Y> shared_ptr &operator=(const shared_ptr<Y, D> &r) {
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
	template <class Y> shared_ptr &operator=(const shared_ptr<Y, D> &&r) {
		reset();
		pointer(r.pointer);
		control(r.control);
		control->grab();
		r.reset();
		return *this;
	}
	template <class Y, class Del> shared_ptr &operator=(std::unique_ptr<Y, Del> &&r) {
		reset();
		control = new details::control_block<Y, Del>{r.release(), r.get_deleter()};
		control->grab();
		return *this;
	}
	void swap(shared_ptr &r) {
		element_type *ptr(pointer);
		details::control_block<T, D> *ctrl(control);
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

template <class T, class D> class weak_ptr : protected shared_ptr<T, D> {
  public:
	typedef typename details::control_block<T, D>::element_type element_type;
	void reset() {
		this->control->release_weak();
		if(this->control->can_delete()) { delete this->control; }
		this->pointer = nullptr;
		this->control = details::nullptrctrl<D>;
	}
	void swap(weak_ptr &r) {
		element_type *ptr(this->pointer);
		details::control_block<T, D> *ctrl(this->control);
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = ptr;
		r.control = ctrl;
	}
	long use_count() const { return this->control->shared_count; }
	bool expired() const { return use_count() == 0; }
	shared_ptr<T, D> lock() const {
		if(expired()) { return shared_ptr<T, D>(); }
		return shared_ptr<T, D>(*this);
	}
	constexpr weak_ptr() : shared_ptr<T, D>() {}
	weak_ptr(const weak_ptr &r) {
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
	}
	template <class Y> weak_ptr(const weak_ptr<Y, D> &r) {
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
	}
	template <class Y> weak_ptr(const std::shared_ptr<Y, D> &r) {
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
	}
	weak_ptr(weak_ptr &&r) {
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = details::nullptrctrl<D>;
	}
	template <class Y> weak_ptr(weak_ptr<Y, D> &&r) {
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = details::nullptrctrl<D>;
	}
	~weak_ptr() { reset(); }
	weak_ptr &operator=(const weak_ptr &r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
		return *this;
	}
	template <class Y> weak_ptr &operator=(const weak_ptr<Y, D> &r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		this->control->grab_weak();
		return *this;
	}
	template <class Y> weak_ptr &operator=(const shared_ptr<Y, D> &r) {
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
		r.control = details::nullptrctrl<D>;
		return *this;
	}
	template <class Y> weak_ptr &operator=(weak_ptr<Y, D> &&r) {
		reset();
		this->pointer = r.pointer;
		this->control = r.control;
		r.pointer = nullptr;
		r.control = details::nullptrctrl<D>;
		return *this;
	}
};
}
#endif
