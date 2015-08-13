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

#include <iterators.h>
#include <utf8.h>
namespace unicode {
	codeunits::codeunits(char *string) {
		this->data=string;
		this->_begin=string;
		this->_end=NULL;
	}
	char *codeunits::next() {
		data=utf8::next_char(data);
		return data;
	}
	char *codeunits::prev() {
		do {
			data--;
		} while((*data&0b11000000)==0b10000000);
		return data;
	}
	char *codeunits::value() {
		return data;
	}
	char *codeunits::begin() {
		data=_begin;
		return data;
	}
	char *codeunits::end() {
		if(_end==NULL) {
			while(*data) {
				data++;
			}
			_end=data;
		}
		return data;
	}

	bool codeunits::has_next() {
		if(*utf8::next_char(data)) {
			return true;
		}
		_end=data;
		return false;
	}
	bool codeunits::has_prev() {
		return data>_begin;
	}
}
