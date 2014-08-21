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
#include <stddef.h>
#include <string.h>
namespace std {
	template<class T>
	class vector {
		private:
			T *arr = NULL;
			size_t count = 0;
			size_t capacity = 0;
		public:
			vector(int init_cap=10) {
				count=0;
				capacity=init_cap;
				arr=(T *)malloc(capacity*sizeof(T));
			}

			~vector() {
				free(arr);
			}

			vector(const vector<T> &x) {
				if(arr) {
					free(arr);
				}
				arr=(T *)malloc(x.count*sizeof(T));
				memcpy(arr,x.arr,sizeof(T)*x.count);
				count=x.count;
				capacity=count;
			}

			T at(int index) {
				if(index>count||index<0) {
					return T();
				}
				return arr[index];
			}

			T operator[](int index) {
				return this->at(index);
			}

			void operator=(vector<T> &x) {
				if(capacity>x.count) {
					memcpy(arr,x.arr,x.count*sizeof(T));
				} else {
					free(arr);
					arr=malloc(x.capacity*sizeof(T));
					memcpy(arr,x.arr,x.count*sizeof(T));
				}
			}

			void clear() {
				//for now assume type T is trivially destructible
				memset(arr,0,count*sizeof(T));
			}

			void push_back(T item) {
				if((count+1)==capacity) {
					capacity=capacity + capacity/2;
					if(capacity%2) {
						capacity++;
					}
					arr=(T *)realloc(arr,capacity*sizeof(T));
					if(arr==NULL) {
						return;
					}
				}
				arr[count++]=item;
			}

			T pop_back() {
				if(count==0) {
					return 0;
				}
				return arr[--count];
			}

			size_t size() {
				return count;
			}

			int erase(int index) {
				if(index>count||index<0) {
					return 0;
				}
				memmove((void *)(arr[index]),(void *)(arr[index+1]),(count-index)*sizeof(T));
				count--;
				return -1;
			}
	};
}
