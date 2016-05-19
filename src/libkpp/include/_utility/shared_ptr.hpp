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
#include <cstdlib>
#include <type_traits>
namespace std {
template <class T> class shared_ptr {
  public:
	typedef typename std::control_block<T>::element_type element_type;

  private:
	element_type *pointer;
	control_block<T> *control;

  public:
	constexpr shared_ptr() {
		pointer = nullptr;
		control = nullptr;
	}
	constexpr shared_ptr(std::nullptr_t) {
		pointer = nullptr;
		control = nullptr;
	}
	template <class Y> explicit shared_ptr(Y *ptr) {
		pointer = ptr;
		control = new control_block<T>(ptr, 1, 0);
		control->grab();
	}
	template <class Y> shared_ptr(const shared_ptr<Y> &r, element_type *ptr) {
		pointer = ptr;
		control = r.control;
		control->grab();
	}
};
}
#endif
