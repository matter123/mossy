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
#ifndef __VECTOR_HPP
#define __VECTOR_HPP
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>
namespace std {
template <class T, bool integral = std::is_integral<T>::value> class vector {
  public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef class vector_iterator : std::iterator<random_access_iterator_tag, T> {
	  private:
		char *copy;
		size_type index;

	  public:
		vector_iterator &operator+=(difference_type n) {
			index += n;
			return *this;
		}
		vector_iterator operator+(difference_type n) { return vector_iterator(*this) += n; }
		friend vector_iterator operator+(difference_type n, vector_iterator other) {
			return vector_iterator(other) += n;
		}
		vector_iterator &operator-=(difference_type n) {
			index += -n;
			return *this;
		}
		vector_iterator operator-(difference_type n) { return vector_iterator(*this) -= n; }
		difference_type operator-(vector_iterator b) { return this->index - b.index; }
		reference operator[](difference_type n) { return *reinterpret_cast<T *>(&copy[n * sizeT]); }
		reference operator*() { return *reinterpret_cast<T *>(&copy[index * sizeT]); }
		pointer operator->() { return reinterpret_cast<T *>(&copy[index * sizeT]); }
		bool operator<(vector_iterator b) { return b - *this > 0; }
		bool operator>(vector_iterator b) { return b < *this; }
		bool operator<=(vector_iterator b) { return !(*this > b); }
		bool operator>=(vector_iterator b) { return !(*this < b); }
		vector_iterator(vector<T> &vec, size_type idx) {
			copy = vec._data;
			index = idx;
		}
		vector_iterator(vector_iterator &r) {
			copy = r.copy;
			index = r.index;
		}
		constexpr vector_iterator() {}
	} iterator;
	typedef class const_vector_iterator : std::iterator<random_access_iterator_tag, T> {
	  private:
		char *copy;
		size_type index;

	  public:
		const_vector_iterator &operator+=(difference_type n) {
			index += n;
			return *this;
		}
		const_vector_iterator operator+(difference_type n) { return array_iterator(*this) += n; }
		friend const_vector_iterator operator+(difference_type n, vector_iterator other) {
			return array_iterator(other) += n;
		}
		const_vector_iterator &operator-=(difference_type n) {
			index += -n;
			return *this;
		}
		const_vector_iterator operator-(difference_type n) { return array_iterator(*this) -= n; }
		difference_type operator-(vector_iterator b) { return this->index - b.index; }
		const_reference operator[](difference_type n) { return *reinterpret_cast<T *>(&copy[n * sizeT]); }
		const_reference operator*() { return *reinterpret_cast<T *>(&copy[index * sizeT]); }
		const_pointer operator->() { return reinterpret_cast<T *>(&copy[index * sizeT]); }
		bool operator<(vector_iterator b) { return b - *this > 0; }
		bool operator>(vector_iterator b) { return b < *this; }
		bool operator<=(vector_iterator b) { return !(*this > b); }
		bool operator>=(vector_iterator b) { return !(*this < b); }
		const_vector_iterator(vector<T> &vec, size_type idx) {
			copy = vec._data;
			index = idx;
		}
		const_vector_iterator(const_vector_iterator &r) {
			copy = r.copy;
			index = r.index;
		}
		constexpr const_vector_iterator() {}
	} const_iterator;

  private:
	char *_data;
	size_type _capacity;
	size_type _size;
	static const size_type sizeT = sizeof(T);
	reference get_item(size_type pos) { return *reinterpret_cast<T *>(&_data[pos * sizeT]); }
	const_reference get_item(size_type pos) const { return *reinterpret_cast<T *>(&_data[pos * sizeT]); }
	void *get_addr(size_type pos, char *d) const { return (void *)((uintptr_t)d * sizeT * pos); }
	void resize_cap(size_type new_cap) {
		char *new_data(new char[sizeT * new_cap]);
		for(size_type i = 0; i < _size; i++) { new(get_addr(i, _data)) T(std::move(get_item(i))); }
		_capacity = new_cap;
		_data = new_data;
		delete[] _data;
	}

  public:
	// element access
	reference at(size_type pos) { return get_item(pos); }
	const_reference at(size_type pos) const { return get_item(pos); }
	reference operator[](size_type pos) { return get_item(pos); }
	const_reference operator[](size_type pos) const { return get_item(pos); }
	reference front() { return get_item(0); }
	const_reference front() const { return get_item(0); }
	reference back() { return get_item(_size - 1); }
	const_reference back() const { return get_item(_size - 1); }
	T *data() { return &front(); }
	const T *data() const { return &front(); }

	// capacity
	bool empty() { return _size == 0; }
	size_type size() { return _size; }
	size_type max_size() { return (UINTPTR_MAX - sizeof(vector<T>)) / sizeof(T); }
	void reserve(size_type new_cap) {
		if(new_cap <= _capacity) return;
		resize_cap(new_cap);
	}
	size_type capacity() { return _capacity; }
	void shrink_to_fit() { resize_cap(max(_size, (size_type)16)); }

	// modifiers
	void clear() {
		for(size_type i = 0; i < _size; i++) { get_item(i).~T(); }
		_size = 0;
	}
	iterator insert(const_iterator pos, const T &value) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		for(int i = _size - 1; i >= pos.index; i--) { get_item(i + 1) = std::move(get_item(i)); }
		get_item(pos.index) = value;
	}
	iterator insert(const_iterator pos, T &&value) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		for(int i = _size - 1; i >= pos.index; i--) { get_item(i + 1) = std::move(get_item(i)); }
		get_item(pos.index) = std::move(value);
	}
	iterator insert(const_iterator pos, size_type count, const T &value) {
		while(_size - 1 + count >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		for(int i = _size - 1; i >= pos.index; i--) { get_item(i + count) = std::move(get_item(i)); }
		while(count--) {
			get_item(pos.index) = value;
			pos++;
		}
	}
	template <class InputIt,
	          std::enable_if_t<std::is_base_of<std::input_iterator_tag,
	                                           typename std::iterator_traits<InputIt>::iterator_category>::value,
	                           int> = 0>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		for(; first < last; first++) {
			insert(pos, *first);
			++pos;
		}
	}
	template <class... Args> iterator emplace(const_iterator pos, Args &&... args) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		for(int i = _size - 1; i >= pos.index; i--) { get_item(i + 1) = std::move(get_item(i)); }
		new(get_addr(pos.index, _data)) T(std::forward<Args>(args)...);
	}
	iterator erase(const_iterator pos) {
		for(int i = pos.index; i < _size; i++) { get_item(i) = std::move(get_item(i + i)); }
		get_item(_size).~T();
	}
	iterator erase(const_iterator first, const_iterator last) { assert(true, false, "not implemented"); }
	void push_back(const T &value) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		new(get_addr(_size, _data)) T(value);
		++_size;
	}
	template <class... Args> void emplace_back(Args &&... args) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		new(get_addr(_size, _data)) T(std::forward<Args>(args)...);
		++_size;
	}
	void pop_back() {
		get_item(_size - 1).~T();
		--_size;
	}
	void resize(size_type count) {
		while(count > _size) {
			if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
			new(get_addr(_size, _data)) T();
			++_size;
		}
		while(count < _size) { pop_back(); }
	}
	void resize(size_type count, const value_type &value) {
		while(count > _size) { push_back(value); }
		while(count < _size) { pop_back(); }
	}
	void swap(vector &other) {
		char *d = _data;
		size_type s = _size, c = _capacity;
		_size = other.size;
		_capacity = other._capacity;
		_data = other._data;
		other._data = d;
		other._capacity = c;
		other._size = s;
	}

	// constructors
	explicit vector() : _size(0), _capacity(16), _data(new char[sizeT * _capacity]) {}
	explicit vector(size_type count, const T &value)
	    : _size(count), _capacity(count), _data(new char[sizeT * _capacity]) {
		for(size_type i = 0; i < _size; i++) { new(get_addr(i, _data)) T(value); }
	}
	explicit vector(size_type count)
	    : _size(count), _capacity(max(count, (size_type)16)), _data(new char[sizeT * _capacity]) {
		for(size_type i = 0; i < _size; i++) { new(get_addr(i, _data)) T(); }
	}
	template <class InputIt,
	          std::enable_if_t<std::is_base_of<std::input_iterator_tag,
	                                           typename std::iterator_traits<InputIt>::iterator_category>::value,
	                           int> = 0>
	vector(InputIt first, InputIt last) : _size(0), _capacity(16), _data(new char[sizeT * _capacity]) {
		for(; first < last; first++) push_back(*first);
	}
	vector(const vector &other) : _size(other._size), _capacity(other._capacity), _data(new char[sizeT * _capacity]) {
		for(size_type i = 0; i < _size; i++) { new(get_addr(i, _data)) T(other[i]); }
	}
	vector(const vector &&other) : _size(other._size), _capacity(other._capacity), _data(other._data) {
		// reset the other
		other._size = 0;
		other._capacity = 16;
		other._data = new char[sizeT * other._capacity];
	}
	// deconstructor
	~vector() {
		clear();
		delete[] _data;
	}
};
}
#endif
