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
	constexpr weak_ptr();
	weak_ptr(const weak_ptr &r);
	template <class Y> weak_ptr(const weak_ptr<Y> &r);
	template <class Y> weak_ptr(const std::shared_ptr<Y> &r);
	weak_ptr(weak_ptr &&r);
	template <class Y> weak_ptr(weak_ptr<Y> &&r);
	~weak_ptr();
	weak_ptr &operator=(const weak_ptr &r);
	template <class Y> weak_ptr &operator=(const weak_ptr<Y> &r);
	template <class Y> weak_ptr &operator=(const shared_ptr<Y> &r);
	weak_ptr &operator=(weak_ptr &&r);
	template <class Y> weak_ptr &operator=(weak_ptr<Y> &&r);
};
}
#endif
