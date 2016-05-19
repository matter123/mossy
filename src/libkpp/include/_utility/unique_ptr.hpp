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
#ifndef __UNIQUE_PTR_HPP
#define __UNIQUE_PTR_HPP
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
namespace std {
template <class T, class Deleter = std::default_delete<T>> class unique_ptr {
  public:
	typedef T *pointer;
	typedef T element_type;
	typedef Deleter deleter_type;

  private:
	pointer elem;
	deleter_type del;

  public:
	pointer get() { return elem; }
	Deleter &get_deleter() { return del; }
	const Deleter &get_deleter() const { return del; }
	pointer release() {
		pointer temp(elem);
		elem = nullptr;
		return temp;
	}
	void reset(pointer ptr = pointer()) {
		pointer old(elem);
		elem = ptr;
		if(old != nullptr) { delete old; }
	}
	void swap(unique_ptr &other) {
		pointer temp(elem);
		Deleter deltemp(del);
		elem = other.elem;
		del = other.del;
		other.elem = temp;
		other.del = deltemp;
	}
	constexpr unique_ptr() : elem(nullptr) {}
	constexpr unique_ptr(nullptr_t) : elem(nullptr){};
	unique_ptr(pointer p) : elem(p){};
	unique_ptr(pointer p,
	           typename std::conditional<std::is_lvalue_reference<std::remove_const_t<Deleter>>::value, Deleter,
	                                     typename std::add_lvalue_reference<const Deleter>::type>::type d) {
		elem(p);
		del = d;
	}
	unique_ptr(pointer p, typename std::conditional<std::is_lvalue_reference<std::remove_const_t<Deleter>>::value,
	                                                typename std::add_rvalue_reference<Deleter>::type,
	                                                typename std::add_rvalue_reference<Deleter>::type>::type d) {
		elem(p);
		del = std::move(d);
	}
	unique_ptr(unique_ptr &&u) : elem(u.release()) {
		if(std::is_reference<Deleter>::value) {
			del = u.get_deleter();
		} else {
			del = std::move(u.del);
		}
	};
	~unique_ptr() { reset(); }
	unique_ptr &operator=(unique_ptr &&rhs) {
		reset(rhs.release());
		return *this;
	}
	template <class U> unique_ptr &operator=(unique_ptr<U> &&rhs) {
		reset(rhs.release());
		return *this;
	}
	unique_ptr &operator=(nullptr_t) { reset(); }
	explicit operator bool() const { elem != nullptr; }
	typename std::add_lvalue_reference_t<T> operator*() const { return *elem; }
	pointer operator->() const { return elem; }
};
template <class T> class unique_ptr<T[]> {
	typedef T *pointer;
	typedef T element_type;
	pointer elem;

  public:
	pointer get() { return elem; }
	pointer release() {
		pointer temp(elem);
		elem = nullptr;
		return temp;
	}
	void reset(pointer ptr = pointer()) {
		pointer old(elem);
		elem = ptr;
		if(old != nullptr) { delete[] old; }
	}
	template <class U> void reset(U) = delete;
	void reset(std::nullptr_t) {
		pointer old(elem);
		elem = nullptr;
		if(old != nullptr) { delete[] old; }
	}
	void swap(unique_ptr &other) {
		pointer temp(elem);
		elem = other.elem;
		other.elem = temp;
	}
	constexpr unique_ptr() : elem(nullptr) {}
	constexpr unique_ptr(nullptr_t) : elem(nullptr){};
	unique_ptr(pointer p) : elem(p){};
	unique_ptr(unique_ptr &&u) : elem(u.release()){};
	~unique_ptr() { reset(); }
	unique_ptr &operator=(unique_ptr &&rhs) {
		reset(rhs.release());
		return *this;
	}
	template <class U> unique_ptr &operator=(unique_ptr<U> &&rhs) {
		reset(rhs.release());
		return *this;
	}
	unique_ptr &operator=(nullptr_t) { reset(); }
	T &operator[](size_t i) const { return elem[i]; }
};
template <class T, class... Args> unique_ptr<T> make_unique(Args &&... args) {
	return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
template <class T> unique_ptr<T> make_unique(std::size_t size) {
	return unique_ptr<T>(new typename std::remove_extent<T>::type[size]);
}

template <class T1, class T2> bool operator==(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
	return x.get() == y.get();
}
template <class T1, class T2> bool operator!=(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
	return x.get() != y.get();
}
template <class T1, class T2> bool operator<=(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
	return x.get() <= y.get();
}
template <class T1, class T2> bool operator<(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
	return x.get() < y.get();
}
template <class T1, class T2> bool operator>=(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
	return x.get() >= y.get();
}
template <class T1, class T2> bool operator>(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
	return x.get() > y.get();
}
template <class T> bool operator==(const unique_ptr<T> &x, nullptr_t) { return !x; }
template <class T> bool operator==(nullptr_t, const unique_ptr<T> &x) { return !x; }
template <class T> bool operator!=(const unique_ptr<T> &x, nullptr_t) { return !!x; }
template <class T> bool operator!=(nullptr_t, const unique_ptr<T> &x) { return !!x; }
template <class T> bool operator<=(const unique_ptr<T> &x, nullptr_t) { return x <= nullptr; }
template <class T> bool operator<=(nullptr_t, const unique_ptr<T> &x) { return nullptr <= x; }
template <class T> bool operator>=(const unique_ptr<T> &x, nullptr_t) { return x >= nullptr; }
template <class T> bool operator>=(nullptr_t, const unique_ptr<T> &x) { return nullptr >= x; }
template <class T> bool operator<(const unique_ptr<T> &x, nullptr_t) { return x < nullptr; }
template <class T> bool operator<(nullptr_t, const unique_ptr<T> &x) { return nullptr < x; }
template <class T> bool operator>(const unique_ptr<T> &x, nullptr_t) { return x > nullptr; }
template <class T> bool operator>(nullptr_t, const unique_ptr<T> &x) { return nullptr > x; }
template <class T> struct hash<unique_ptr<T>> {
	typedef typename hash<T>::result_type result_type;
	typedef unique_ptr<T> argument_type;
	result_type operator()(argument_type u) { return hash<T>()(u.get()); }
};
}
#endif
