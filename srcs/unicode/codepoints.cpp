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

#include <stdint.h>
#include <iterators.h>
#include <utf8.h>
namespace unicode {
	codepoints::codepoints(char *string) {
		this->data=string;
		this->_begin=string;
		this->_end=NULL;
	}
	uint32_t codepoints::next() {
		data=utf8::next_char(data);
		return utf8::decode_char(data);
	}
	uint32_t codepoints::prev() {
		do {
			data--;
		} while((*data&0b11000000)==0b10000000);
		return utf8::decode_char(data);
	}
	uint32_t codepoints::value() {
		return utf8::decode_char(data);
	}
	uint32_t codepoints::begin() {
		data=_begin;
		return utf8::decode_char(data);
	}
	uint32_t codepoints::end() {
		if(_end==NULL) {
			while(*data) {
				data++;
			}
			_end=data;
		}
		return utf8::decode_char(data);
	}

	bool codepoints::has_next() {
		if(*utf8::next_char(data)) {
			return true;
		}
		_end=data;
		return false;
	}
	bool codepoints::has_prev() {
		return data>_begin;
	}
}
