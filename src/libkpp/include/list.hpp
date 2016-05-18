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
    list(std::initalizer_list<T>)
    operator=(std::initalizer_list<T>)
    assign(std::initalizer_list<T>)
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
		const bool nullitem;
		node(node<T> *p, node<T> *n) : prev(p), next(n), nullitem(p == n && p == nullptr) {}
		node(U &&d, node<T> *p, node<T> *n) : prev(p), next(n), nullitem(false) {
			data = std::move(d);
		}
	};
	node<T> *_front;
	node<T> *_back;
	node<T> nullnode = node<T>(nullptr, nullptr);
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
		node<T> get() { return current; }
		list_iterator() : current(nullptr) {}
		list_iterator(node<T> node) { current = node; }
		bool operator==(list_iterator rhs) const { return this->current == rhs.current; }
		bool operator!=(list_iterator rhs) const { return !(*this == rhs); }
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
		const node<T> get() const { return current; }
		const_list_iterator() : current(nullptr) {}
		const_list_iterator(node<T> node) { current = node; }
		const_list_iterator(list_iterator i) : current(i.get()) {}
		bool operator==(const_list_iterator rhs) const { return this->current == rhs.current; }
		bool operator!=(const_list_iterator rhs) const { return !(*this == rhs); }
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

	bool empty() { return !count; }
	bool size() { return count; }
	size_type max_size() { return SIZE_MAX / sizeof(node<T>); }
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

  private:
	node<T> &add_front() {
		_front->prev = new node<T>(&nullnode, _front);
		_front = _front->prev;
		count++;
		if(count == 1) {
			_back = _front;
		}
		return _front;
	}
	node<T> &add_back() {
		_back->next = new node<T>(_back, &nullnode);
		_back = _back->next;
		count++;
		if(count == 1) {
			_front = _back;
		}
		return _back;
	}
	node<T> &add_before(iterator i) {
		auto prev = i.get()->prev;
		prev->next = new node<T>(prev, i.current);
		i.get()->prev = prev->next;
		count++;
		return prev->next;
	}
	node<T> &add_after(iterator i) {
		auto next = i.get()->next;
		next->prev = new node<T>(i.current, next);
		i.get()->next = next->prev;
		count++;
		return next->prev;
	}

  public:
	void push_back(const T &value) { add_back()->data = value; }
	void push_back(T &&value) { add_back()->data = std::move(value); }
	void pop_back() {
		_back = _back->prev;
		count--;
		delete _back->next;
		_back->next = &nullnode;
	}
	template <class... Args> void emplace_back(Args &&... args) {
		auto back = add_back();
		back->data = new(std::addressof(back->data)) T(std::forward<Args>(args)...);
	}

	void push_front(const T &value) { add_front()->data = value; }
	void push_front(T &&value) { add_front()->data = std::move(value); }
	void pop_front() {
		_front = _front->next;
		count--;
		delete _front->prev;
		_front->prev = &nullnode;
	}
	template <class... Args> void emplace_front(Args &&... args) {
		auto front = add_front();
		front->data = new(std::addressof(front->data)) T(std::forward<Args>(args)...);
	}
	void clear() {
		if(!count) {
			return;
		}
		for(auto i = _front; i <= _back;) {
			auto temp = i->next;
			delete i;
			i = temp;
		}
		count = 0;
	}

	explicit list() : _front(&nullnode), _back(&nullnode), count(0) {}
	list(size_type c, const T &value) {
		for(size_t s = 0; s < c; s++) { push_back(value); }
	}
	explicit list(size_type c) {
		for(size_t s = 0; s < c; s++) { add_back(); }
	}
	template <class InputIt> list(InputIt first, InputIt last) {
		for(; first < last; first++) push_back(*first);
	}
	list(const list &other) : list(other.cbegin(), other.cend()) {}
	list(list &&other) {
		_front = other._front;
		_back = other._back;
		count = other.count;
		other._front = &nullnode;
		other._back = &nullnode;
		other.count = 0;
	}
	~list() { clear(); }
	list &operator=(const list &other) {
		clear();
		for(auto first = other.cbegin(); first < other.cend(); first++) push_back(*first);
	};
	list &operator=(list &&other) {
		clear();
		_front = other._front;
		_back = other._back;
		count = other.count;
		other._front = &nullnode;
		other._back = &nullnode;
		other.count = 0;
	}

	void assign(size_type c, const T &value) {
		clear();
		while(c) {
			push_back(value);
			c--;
		}
	}
	template <class InputIt> void assign(InputIt first, InputIt last) {
		clear();
		for(; first < last; first++) push_back(*first);
	}
	iterator erase(const_iterator pos) {
		auto ret = pos.get()->next;
		pos.get()->prev->next = ret;
		ret->prev = pos.get.prev();
		delete pos.get();
		count--;
		return iterator(ret);
	}
	iterator erase(const_iterator first, const_iterator last) {
		auto prev = first.get()->prev;
		auto next = last.get();
		prev->next = next;
		next->prev = prev;
		while(first < last) {
			delete(first++).get();
			count--;
		}
		return last;
	}
	void resize(size_type c) {
		while(c > count) { add_back(); }
		while(c < count) { pop_back(); }
	}
	void resize(size_type c, const value_type &value) {
		while(c > count) { push_back(value); }
		while(c < count) { pop_back(); }
	}
};
template <class T> bool operator==(const list<T> &lhs, const list<T> &rhs) {
	if(lhs.size() != rhs.size()) {
		return false;
	}
	if(lhs.empty())
		return true; // iterating over empty elements is not defined
	for(auto it1 = lhs.begin(), it2 = rhs.begin(); it1 < lhs.end(); it1++, it2++) {
		if(*it1 != *it2) {
			return false;
		}
	}
	return true;
}

template <class T> bool operator!=(const list<T> &lhs, const list<T> &rhs) { return !(lhs == rhs); }

template <class T> bool operator<(const list<T> &lhs, const list<T> &rhs) {
	if(rhs.empty())
		return false;
	if(lhs.empty())
		return true;
	return lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <class T> bool operator<=(const list<T> &lhs, const list<T> &rhs) { return !(lhs > rhs); }

template <class T> bool operator>(const list<T> &lhs, const list<T> &rhs) {
	if(lhs.empty())
		return false;
	if(rhs.empty())
		return true;
	return lexicographical_compare(rhs.cbegin(), rhs.cend(), lhs.cbegin(), lhs.cend());
}

template <class T> bool operator>=(const list<T> &lhs, const list<T> &rhs) { return !(lhs < rhs); }
}
#endif
