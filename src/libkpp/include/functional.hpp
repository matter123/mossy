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
#ifndef __FUNCTIONAL_HPP
#define __FUNCTIONAL_HPP
#include <cstddef>
#include <memory>
namespace std {
template <class Key> struct hash {
	typedef Key argument_type;
	typedef std::size_t result_type;
	result_type operator()(argument_type k) {
		if(sizeof(argument_type) <= sizeof(result_type)) {
			return reinterpret_cast<result_type>(k);
		}
		result_type sum = 0, count;
		const_pointer_t pos = reinterpret_cast<const_pointer_t>(addressof(k));
		for(count = 0; count < sizeof(argument_type); count++) {
			result_type imm = (count + 1);
			imm *= *pos;
			pos++;
			sum += imm;
		}
		return sum;
	}
};
}
#endif
