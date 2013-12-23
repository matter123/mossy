/*
 * Copyright 2013 Matthew Fosdick

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

#ifndef STRCLASS_HPP
#define STRCLASS_HPP
#include <stddef.h>
namespace std {
	template<int SIZE>
	class nm_string {
		private:
			char buf[SIZE];
			int len=0;
		public:
			size_t size() {
				return len;
			}
			size_t length() {
				return len;
			}
			size_t max_size() {
				return SIZE;
			}
			void clear() {
				len=0;
			}
			char& operator[] (size_t pos) {
				if(pos<len) {
					return buf[pos];
				}
				return '\0';
			}
			char& at (size_t pos) {
				if(pos<len) {
					return buf[pos];
				}
				return '\0';
			}

			nm_string (c_cstring s) {
				std::strcpy(buf,s);
				len=strlen(buf);
			}
			nm_string() {
				buf[0]='\0';
				len=0;
			}
			nm_string (const nm_string& str) {
				if(str->max_size()<=SIZE) {
					str->buf[str->len]='\0';
					std::strcpy(buf,str->buf);
					len=str->len;
				}else {
					buf[0]='\0';
					len=0;
				}
			}
			nm_string& operator+= (const nm_string& str) {
				if(len+str->len<SIZE) {
					buf[len]='\0';
					str->buf[str->len]='\0';
					std::strcat(buf,str->buf);
					len+=str->len;
				}
				return *this;
			}
			nm_string& operator+= (c_cstring str) {
				int strl=std::strlen(str);
				if(len+strl<SIZE) {
					buf[len]='\0';
					std::strcat(buf,str);
					len+=strl;
				}
				return *this;
			}
			nm_string& operator+= (char c) {
				if(len+1<SIZE) {
					buf[len]=c;
					len++;
				}
				return *this;
			}
	};
}
#endif
