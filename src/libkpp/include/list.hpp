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
#ifndef __LIST_HPP
#define __LIST_HPP
/* NOTE: does not implement:
    merge
    splice
    remove
    remove_if
    reverse
    unique
    sort
*/
#include <iterator>
#include <memory>
#include <stdlib.h>
namespace std {
template <class T> class list {
	template <class U> struct node {
		U data;
		node *prev;
		node *next;
		node(node<T> *p, node<T> *n) : prev(p), next(n) {}
		node(U &&d, node<T> *p, node<T> *n) : prev(p), next(n) { data = std::move(d); }
	};
	node<T> *_front;
	node<T> *_back;
	size_t count;

  public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T &reference;
	typedef const T &const_reference;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef class list_iterator : std::iterator<std::bidirectional_iterator_tag, T> {
		node<T> *current;

	  public:
		bool operator==(list_iterator rhs) { return this->current == rhs.current; }
		bool operator!=(list_iterator rhs) { return !(*this == rhs); }
		reference operator*() { return current->data; }
		pointer operator->() { return std::addressof(current->data); }
		list_iterator &operator++() {
			current = current->next;
			return *this;
		}
		list_iterator &operator++(int) { return ++list_iterator(*this); }
		list_iterator &operator--() {
			current = current->prev;
			return *this;
		}
		list_iterator &operator--(int) { return --list_iterator(*this); }
	} iterator;
	typedef class const_list_iterator : std::iterator<std::bidirectional_iterator_tag, T> {
		node<T> *current;

	  public:
		bool operator==(const_list_iterator rhs) { return this->current == rhs.current; }
		bool operator!=(const_list_iterator rhs) { return !(*this == rhs); }
		reference operator*() { return current->data; }
		pointer operator->() { return std::addressof(current->data); }
		const_list_iterator &operator++() {
			current = current->next;
			return *this;
		}
		const_list_iterator &operator++(int) { return ++const_list_iterator(*this); }
		const_list_iterator &operator--() {
			current = current->prev;
			return *this;
		}
		const_list_iterator &operator--(int) { return --const_list_iterator(*this); }
	} const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	explicit list() {
		_front = new node<T>(nullptr, nullptr);
		_back = _front;
		count = 0;
	}
	explicit list(size_t c, const T &value) {
		this->count = c;
		_front = new node<T>(value, nullptr, nullptr);
		c--;
		node<T> *head = _front;
		for(int i = 0; i < c; i++) {
			head->next = new node<T>(value, head, nullptr);
			head = head->next;
		}
		_back = head;
		if(!_back->next) {
			_back->next = new node<T>();
			_back->next->prev = _back;
		}
	}
	explicit list(size_t c) {
		this->count = c;
		_front = new node<T>();
		c--;
		node<T> *head = _front;
		for(int i = 0; i < c; i++) {
			head->next = new node<T>();
			head->data();
			head->next->prev = head;
			head = head->next;
		}
		_back = head;
		if(!_back->next) {
			_back->next = new node<T>();
			_back->next->prev = _back;
		}
	}
	template <class InputIt> list(InputIt first, InputIt last) {
		_front = new node<T>();
		node<T> *head = _front;
		for(; first < last; first++) {
			head->next = new node<T>();
			head->data = *first;
			count++;
			head->next->prev = head;
			head = head->next;
		}
		_back = head;
		if(!_back->next) {
			_back->next = new node<T>();
			_back->next->prev = _back;
		}
	}
	list(const list &other) : list(other.cbegin(), other.cend()) {}
	~list() {
		node<T> *head = _front;
		while(head != nullptr) {
			node<T> temp = head;
			head = head->next;
			delete temp;
		}
	}
	list &operator=(const list &other) {
		clear();
		auto head = _front;
		for(auto i = other.begin(); i < other.end(); i++) {
			head->data = *i;
			head->next = new node<T>();
			count++;
			head->next->prev = head;
			head = head->next;
		}
	}
	list &operator=(list &&other) {
		_front = other._front;
		_back = other.back;
		count = other.count;
		other();
	}
	void assign(size_t c, const T &value) {
		// set the first c nodes to value creating them if otherwise they would not exist
		size_t left = c;
		c = 0;
		node<T> *head = _front;
		while(left) {
			head->data = value;
			c++;
			left--;
			if(left && head->next == nullptr) {
				head->next = new node<T>();
				head->next->prev = head;
			}
			head = head->next;
		}
		if(c > count) {
			_back = head;
			count = c;
		}
		if(!_back->next) {
			_back->next = new node<T>();
			_back->next->prev = _back;
		}
	}
	reference front() { return _front->data; }
	const_reference front() const { return _front->data; }

	reference back() { return _back->data; }
	const_reference back() const { return _back->data; }

	iterator begin() { return iterator(front()); }
	const_iterator cbegin() { return const_iterator(front()); }
	iterator end() { return iterator(_back->next); }
	const_iterator cend() { return const_iterator(_back->next); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator crbegin() { return const_reverse_iterator(cend()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator crend() { return const_reverse_iterator(cbegin()); }

	bool empty() const { return _back == _front || count == 0; }

	size_type size() const { return count; }
	size_type max_size() const { return SIZE_MAX / sizeof(node<T>); }

	void clear() {
		node<T> *head = _front;
		while(head != nullptr) {
			node<T> temp = head;
			head = head->next;
			delete temp;
		}
		_front = _back = new node<T>();
		count = 0;
	}

	void push_back(const T &value) {
		_back->next->data = value;
		_back = _back->next;
		_back->next = new node<T>();
		_back->next->prev = _back;
		count++;
	}
	void push_back(T &&value) {
		_back->next->data = std::move(value);
		_back = _back->next;
		_back->next = new node<T>();
		_back->next->prev = _back;
		count++;
	}
	template <class... Args> void emplace_back(Args &&... args) {
		_back->next->data = new(std::addressof(_back->next->data)) T(std::forward<Args>(args)...);
		_back = _back->next;
		_back->next = new node<T>();
		_back->next->prev = _back;
		count++;
	}
	void pop_back() {
		delete _back->next;
		_back = _back->prev;
		count--;
	}

	void push_front(const T &value) {
		_front->prev = new node<T>();
		_front->prev->next = _front;
		_front = _front->prev;
		_front->data = value;
		count++;
	}
	void push_front(T &&value) {
		_front->prev = new node<T>();
		_front->prev->next = _front;
		_front = _front->prev;
		_front->data = std::move(value);
		count++;
	}
	template <class... Args> void emplace_front(Args &&... args) {
		_front->prev = new node<T>();
		_front->prev->next = _front;
		_front = _front->prev;
		_front->data = new(std::addressof(_front->data)) T(std::forward<Args>(args)...);
		count++;
	}
	void pop_front() {
		auto temp = _front;
		_front = _front->next;
		_front->prev = nullptr;
		delete temp;
	}
	iterator insert(const_iterator pos, const T &value) {
		auto prev = pos.current->prev;
		prev->next = new node<T>();
		prev->next->data = value;
		prev->next->prev = prev;
		prev->next->next = pos.current;
		prev->next->next->prev = prev->next;
	}
	void resize(size_type c) {
		if(c > count) {
			while(count != c) {
				_back = _back->next;
				_back->next = new node<T>();
				_back->next->prev = _back;
				count++;
			}
		} else if(c < count) {
			while(count != c) { pop_back(); }
		}
	}
	void resize(size_type c, const value_type &value) {
		if(c > count) {
			while(count != c) { push_back(value); }
		} else if(c < count) {
			while(count != c) { pop_back(); }
		}
	}
	void swap(list &other) {
		auto tfront = other._front;
		auto tback = other._back;
		other._front = _front;
		other._back = _back;
		_front = tfront;
		_back = tback;
	}
};
}
#endif
