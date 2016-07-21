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
#ifndef __VECTOR_INTEGRAL_HPP
#define __VECTOR_INTEGRAL_HPP
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <vector>
namespace std {
template <class T> class vector<T, true> {
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef class vector_iterator : std::iterator<random_access_iterator_tag, T> {
	  private:
		T *copy;
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
		reference operator[](difference_type n) { return copy[n]; }
		reference operator*() { return copy[index]; }
		pointer operator->() { return copy[index]; }
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
		T *copy;
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
		const_reference operator[](difference_type n) { return copy[n]; }
		const_reference operator*() { return copy[index]; }
		const_pointer operator->() { return copy[index]; }
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
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  private:
	T *_data;
	size_type _capacity;
	size_type _size;
	void resize_cap(size_type new_cap) {
		T *new_data = new T[new_cap];
		memcpy(new_data, _data, sizeof(T) * _size);
		_capacity = new_cap;
		_data = new_data;
		delete[] _data;
	}

  public:
	// element access
	reference at(size_type pos) { return _data[pos]; }
	const_reference at(size_type pos) const { return _data[pos]; }
	reference operator[](size_type pos) { return _data[pos]; }
	const_reference operator[](size_type pos) const { return _data[pos]; }
	reference front() { return _data[0]; }
	const_reference front() const { return _data[0]; }
	reference back() { return _data[_size - 1]; }
	const_reference back() const { return _data[_size - 1]; }
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
		memset(_data, 0, _capacity * sizeof(T));
		_size = 0;
	}
	iterator insert(const_iterator pos, const T &value) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		memmove(&_data[pos.index + 1], &_data[pos.index], (_size - pos.index + 1) * sizeof(T));
		_data[pos.index] = value;
		++_size;
	}
	iterator insert(const_iterator pos, T &&value) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		memmove(&_data[pos.index + 1], &_data[pos.index], (_size - pos.index + 1) * sizeof(T));
		_data[pos.index] = std::move(value);
		++_size;
	}
	iterator insert(const_iterator pos, size_type count, const T &value) {
		while(_size - 1 + count >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		memmove(&_data[pos.index + 1], &_data[pos.index], (_size - pos.index + 1) * sizeof(T));
		while(count--) {
			_data[pos.index] = value;
			++pos;
			++_size;
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
		memmove(&_data[pos.index + 1], &_data[pos.index], _size - pos.index + 1 * sizeof(T));
		new(&_data[pos.index]) T(std::forward<Args>(args)...);
	}
	iterator erase(const_iterator pos) {
		memmove(&_data[pos.index], &_data[pos.index + 1], (_size - pos.index) * sizeof(T));
		--_size;
		_data[_size] = 0;
	}
	iterator erase(const_iterator first, const_iterator last) { assert(true, false, "not implemented"); }
	void push_back(const T &value) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		_data[_size] = value;
		++_size;
	}
	template <class... Args> void emplace_back(Args &&... args) {
		if(_size >= _capacity) { resize_cap((size_type)(_capacity * 1.5f)); }
		new(&_data[_size]) T(std::forward<Args>(args)...);
		++_size;
	}
	void pop_back() {
		_data[_size - 1] = 0;
		--_size;
	}
	void resize(size_type count) {
		if(count > _size) {
			if(_size + count >= _capacity) { resize_cap(std::max((size_type)(_capacity * 1.5f), _size + count)); }
			memset(&_data[_size], 0, sizeof(T) * count);
			_data[_size] = 0;
			++_size;
		}
		while(count < _size) { pop_back(); }
	}
	void resize(size_type count, const value_type &value) {
		while(count > _size) { push_back(value); }
		while(count < _size) { pop_back(); }
	}
	void swap(vector &other) {
		T *d = _data;
		size_type s = _size, c = _capacity;
		_size = other.size;
		_capacity = other._capacity;
		_data = other._data;
		other._data = d;
		other._capacity = c;
		other._size = s;
	}
};
}
#endif
