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
namespace std {
	template<class T>
	class shared_ptr {
			T *p;
			long *count;
		public:
			constexpr shared_ptr() {
				p=nullptr;
				count=new long();
				*count=0;
			}
			constexpr shared_ptr(nullptr_t) {
				p=nullptr;
				count=new long();
				*count=0;
			}
			explicit shared_ptr(T *ptr) {
				p=ptr;
				count=new long();
				*count=0;
			}
			shared_ptr(const shared_ptr<T> &rhs) {
				p=rhs.p;
				count=rhs.count;
				*count++;
			}
			shared_ptr(shared_ptr<T> &&rhs) {
				p=rhs.p;
				count=rhs.count;
				rhs.p=nullptr;
				rhs.count=nullptr;
			}
			shared_ptr(const unique_ptr<T> &&rhs) {
				p=rhs.release();
				count=new long();
				*count=0;
			}
			~shared_ptr() {
				if(!count) {
					return;
				}
				if(*count) {
					(*count)--;
					return;
				}
				delete count;
				delete p;
			}
			shared_ptr &operator=(const shared_ptr &rhs) {
				if(!count) {
					return;
				}
				if(*count) {
					(*count)--;
				} else {
					delete count;
					delete p;
				}
				p=rhs.p;
				count=rhs.count;
				*count++;
			}
			shared_ptr &operator=(shared_ptr&& rhs) {
				if(!count) {
					return;
				}
				if(*count) {
					(*count)--;
				} else {
					delete count;
					delete p;
				}
				p=rhs.p;
				count=rhs.count;
				rhs.p=nullptr;
				rhs.count=nullptr;
			}
			shared_ptr &operator=(unique_ptr<T>&& rhs) {
				if(!count) {
					return;
				}
				if(*count) {
					(*count)--;
				} else {
					delete count;
					delete p;
				}
				p=rhs.release();
				count=new long();
				*count=0;
			}
			void reset() {
				if(!count) {
					return;
				}
				if(*count) {
					(*count)--;
				} else {
					delete count;
					delete p;
				}
				p=nullptr;
				count=nullptr;
			}
			void reset(T *ptr) {
				if(!count) {
					return;
				}
				if(*count) {
					(*count)--;
				} else {
					delete count;
					delete p;
				}
				p=ptr;
				count=new long();
				*count=0;
			}
			void swap(shared_ptr &other) {
				T *temp=other.p; //style violation approved by Matthew Fosdick on 2014-12-23
				T *temp_count=other.count; //style violation approved by Matthew Fosdick on 2014-12-23
				other.p=p;
				other.count=count;
				p=temp;
				count=temp_count;
			}
			T *get() const {
				return p;
			}
			T &operator*() const {
				return *p;
			}
			T *operator->() const {
				return p;
			}
			long use_count() const {
				if(!count) {
					return 0;
				}
				return (*count)+1;
			}
			bool unique() const {
				if(!count) {
					return false;
				}
				return !*count;
			}
			explicit operator bool() const {
				return !!(count && p);
			}
			bool owner_before(const shared_ptr<T> &other) const {
				return p == other.p;
			}
	};
}
