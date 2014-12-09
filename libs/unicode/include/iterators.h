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
#include <stdint.h>
namespace unicode {
	class bytes {
	private:
		char *data;
		char *begin;
		char *end;
	public:
		bytes(char * string);
		uint8_t next();
		uint8_t prev();
		uint8_t value();
		uint8_t begin();
		uint8_t end();
		
		bool has_next();
		bool has_prev();
	};
	class codepoints {
	private:
		char *data;
		char *begin;
		char *end;
	public:
		codepoints(char * string);
		uint32_t next();
		uint32_t prev();
		uint32_t value();
		uint32_t begin();
		uint32_t end();

		bool has_next();
		bool has_prev();
	};
	class codeunits {
	private:
		char *data;
		char *begin;
		char *end;
	public:
		codeunits(char * string);
		char *next();
		char *prev();
		char *value();
		char *begin();
		char *end();

		bool has_next();
	};
}