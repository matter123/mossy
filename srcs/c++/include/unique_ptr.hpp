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
#ifndef __MEM
#pragma error this file should only be included by memory.h
#endif
#pragma once
namespace std {
	template<class T>
	class unique_ptr {
			T *p;
		public:
			constexpr unique_ptr() {
				p=nullptr;
			}
			constexpr unique_ptr(nullptr_t) {
				p=nullptr;
			}
			explicit unique_ptr(T *ptr) {
				p=ptr;
			}
			unique_ptr(unique_ptr<T> &&u) {
				if(this==u) {
					return;
				}
				if(p) {
					delete p;
				}
				p=u.p;
				u.p=nullptr;
			}
			~unique_ptr() {
				if(p) {
					delete p;
				}
			}
			unique_ptr &operator=(unique_ptr&& r) {
				if(this==r) {
					return;
				}
				if(p) {
					delete p;
				}
				p=r.p;
				r.p=nullptr;
			}
			template<class U>
			unique_ptr &operator=(unique_ptr<U>&& rhs) {
				if(this==rhs) {
					return;
				}
				if(p) {
					delete p;
				}
				p=rhs.p;
				rhs.p=nullptr;
			}
			unique_ptr &operator=(nullptr_t) {
				if(p) {
					delete p;
				}
				p=nullptr;
			}
			T *release() {
				T *op=p;
				p=nullptr;
				return op;
			}
			void reset(T *ptr) {
				T *op=p;
				p=ptr;
				if(op) {
					delete op;
				}
			}
			void swap(unique_ptr &other) {
				T *temp=other.p; //style violation approved by Matthew Fosdick on 2014-12-22
				other.p=p;
				p=temp;
			}
			T *get() const {
				return p;
			}
			explicit operator bool() const {
				return !!p;
			}
			T &operator*() const {
				return *p;
			}
			T *operator->() const {
				return p;
			}
	};
	template<class T>
	class unique_ptr<T []> {
			T *p;
		public:
			constexpr unique_ptr() {
				p=nullptr;
			}
			constexpr unique_ptr(nullptr_t) {
				p=nullptr;
			}
			explicit unique_ptr(T *ptr) {
				p=ptr;
			}
			template<class U>
			unique_ptr(unique_ptr<U> &&u) {
				if(this==u) {
					return;
				}
				if(p) {
					delete[] p;
				}
				p=u.p;
				u.p=nullptr;
			}
			~unique_ptr() {
				if(p) {
					delete[] p;
				}
			}
			unique_ptr &operator=(unique_ptr<T>&& rhs) {
				if(this==rhs) {
					return;
				}
				if(p) {
					delete[] p;
				}
				p=rhs.p;
				rhs.p=nullptr;
			}
			unique_ptr &operator=(nullptr_t) {
				if(p) {
					delete[] p;
				}
				p=nullptr;
			}
			T *release() {
				T *op=p;
				p=nullptr;
				return op;
			}
			void reset(T *ptr) {
				T *op=p;
				p=ptr;
				delete[] op;
			}
			template<class U>
			void reset(U)=delete;
			void reset(nullptr_t) {
				if(p) {
					delete[] p;
				}
				p=nullptr;
			}
			void swap(unique_ptr &other) {
				T *temp=other.p; //style violation approved by Matthew Fosdick on 2014-12-22
				other.p=p;
				p=temp;
			}
			T *get() const {
				return p;
			}
			explicit operator bool() const {
				return !!p;
			}
			T &operator[](size_t i) const {
				return p[i];
			}
	};
	template<class T1, class T2>
	bool operator==(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
		return x.get()==y.get();
	}
	template<class T1, class T2>
	bool operator!=(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
		return x.get()!=y.get();
	}
	template<class T1, class T2>
	bool operator<(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
		return x.get()<y.get();
	}
	template<class T1, class T2>
	bool operator<=(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
		return x.get()<=y.get();
	}
	template<class T1, class T2>
	bool operator>(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
		return x.get()>y.get();
	}
	template<class T1, class T2>
	bool operator>=(const unique_ptr<T1> &x, const unique_ptr<T2> &y) {
		return x.get()>=y.get();
	}
	template <class T>
	bool operator==(const unique_ptr<T> &x, nullptr_t) {
		return x.get()==nullptr;
	}
	template <class T>
	bool operator==(nullptr_t, const unique_ptr<T> &x) {
		return x.get()==nullptr;
	}
	template <class T>
	bool operator!=(const unique_ptr<T> &x, nullptr_t) {
		return x.get()!=nullptr;
	}
	template <class T>
	bool operator!=(nullptr_t, const unique_ptr<T> &x) {
		return x.get()!=nullptr;
	}
	template <class T>
	bool operator<(const unique_ptr<T> &x, nullptr_t) {
		return x.get()<nullptr;
	}
	template <class T>
	bool operator<(nullptr_t, const unique_ptr<T> &y) {
		return nullptr<y.get();
	}
	template <class T>
	bool operator<=(const unique_ptr<T> &x, nullptr_t) {
		return x.get()<=nullptr;
	}
	template <class T>
	bool operator<=(nullptr_t, const unique_ptr<T> &y) {
		return nullptr<=y.get();
	}
	template <class T>
	bool operator>(const unique_ptr<T> &x, nullptr_t) {
		return x.get()>nullptr;
	}
	template <class T>
	bool operator>(nullptr_t, const unique_ptr<T> &y) {
		return nullptr>y.get();
	}
	template <class T>
	bool operator>=(const unique_ptr<T> &x, nullptr_t) {
		return x.get()>=nullptr;
	}
	template <class T>
	bool operator>=(nullptr_t, const unique_ptr<T> &y) {
		return nullptr>=y.get();
	}
}
