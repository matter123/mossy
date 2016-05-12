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
#pragma once
#include <memory>
#include <stdlib.h>
namespace std {
template <class T, class Allocator = std::allocator<T>> class list {
	template <class U> struct node {
		U data;
		node *prev;
		node *next;
	};
	node<T> *_front;
	node<T> *_back;
	size_t count;
	allocator<node<T>> nodealloc;
	Allocator Talloc;

  public:
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef size_t size_type;
	typedef T &reference;
	typedef const T &const_reference;
	// more here
	list() : list(Allocator()) {}
	explicit list(const Allocator &alloc) {
		Talloc = alloc;
		nodealloc = decltype(alloc)::template rebind<node<T>>();
		_front = nullptr;
		_back = nullptr;
		count = 0;
	}
	explicit list(size_t c, const T &value, const Allocator &alloc = Allocator()) {
		Talloc = alloc;
		nodealloc = decltype(alloc)::template rebind<node<T>>();
		this->count = c;
		_front = nodealloc.allocate(1);
		_front();
		c--;
		node<T> *head = _front;
		head->data = value;
		for(int i = 0; i < c; i++) {
			head->next = nodealloc.allocate(1);
			head->next();
			head->next->prev = head;
			head = head->next;
			head->data = value;
		}
		_back = head;
	}
	explicit list(size_t c, const Allocator &alloc) {
		Talloc = alloc;
		nodealloc = decltype(alloc)::template rebind<node<T>>();
		this->count = c;
		_front = nodealloc.allocate(1);
		_front();
		c--;
		node<T> *head = _front;
		for(int i = 0; i < c; i++) {
			head->next = nodealloc.allocate(1);
			head->next();
			head->next->prev = head;
			head = head->next;
		}
		_back = head;
	}
	list(const list &other) { abort(); }
	list(const list &other, const Allocator &alloc) { abort(); }
	~list() {
		if(count == 0)
			return;
		node<T> *head = _front;
		while(head != nullptr) {
			node<T> temp = head;
			head = head->next;
			nodealloc.destroy(temp);
			nodealloc.deallocate(temp, 1);
		}
	}
	list &operator=(const list &other) { abort(); }
	list &operator=(list &&other) { abort(); }
	void assign(size_t c, const T &value) {
		// set the first c nodes to value creating them if otherwise they would not exist
		size_t left = c;
		c = 0;
		if(_front == nullptr) {
			_front = nodealloc.allocate(1);
			_front();
		}
		node<T> *head = _front;
		while(left) {
			head->data = value;
			c++;
			left--;
			if(left && head->next == nullptr) {
				head->next = nodealloc.allocate(1);
				head->next();
				head->next->prev = head;
			}
			head = head->next;
		}
		if(c > count) {
			_back = head;
			count = c;
		}
	}
	allocator_type get_allocator() { return Talloc; }
	reference front() { return _front->data; }
	const_reference front() const { return _front->data; }
	reference back() { return _back->data; }
	const_reference back() const { return _back->data; }
	bool empty() const { return _back == nullptr || count == 0; }
	size_type size() const { return count; }
	void clear() {
		if(count == 0)
			return;
		node<T> *head = _front;
		while(head != nullptr) {
			node<T> temp = head;
			head = head->next;
			nodealloc.destroy(temp);
			nodealloc.deallocate(temp, 1);
		}
	}
};
}
