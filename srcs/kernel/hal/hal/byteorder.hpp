/*
    Copyright 2014 Matthew Fosdick

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
#include <arch.h>

namespace hal {
	static int byte_order=0;
	template<class T>
	T toLSB(T num) {
		if(!byte_order) byte_order=::get_byte_order();
		if(byte_order==1) {
			return num;
		}
		return swap_byte_order(num);
	}

	template<class T>
	T fromLSB(T num) {
		if(!byte_order) byte_order=::get_byte_order();
		if(byte_order==1) {
			return num;
		}
		return swap_byte_order(num);
	}

	template<class T>
	T toMSB(T num) {
		if(!byte_order) byte_order=::get_byte_order();
		if(byte_order==2) {
			return num;
		}
		return swap_byte_order(num);
	}

	template<class T>
	T fromMSB(T num) {
		if(!byte_order) byte_order=::get_byte_order();
		if(byte_order==2) {
			return num;
		}
		return swap_byte_order(num);
	}
	template<class T>
	T swap_byte_order(T num) {
		T ret=0;
		size_t l=sizeof(T);
		for(size_t s=0;s<l;s++) {
			T tmp=(num<<(((l-1)-s)*8))&0xFF;
			ret|=(tmp<<(s*8));
		}
		return ret;
	}
}
