/*
    Copyright 2015 Matthew Fosdick

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
#ifndef __UTIL
#pragma error this file should only be included by utility.h
#endif
namespace std {
	template<class T1, class T2>
	struct pair {
		T1 first;
		T2 second;
		constexpr pair() {
			first {} {};
			second {} {};
		}
		constexpr pair(const T1 &first, const T2 &second) {
			this->first=first;
			this->second=second;
		}
	};
}
