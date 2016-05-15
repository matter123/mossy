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
#ifndef __ARRAY_HPP
#define __ARRAY_HPP
#include <cstddef>
#include <cstdlib>
#include <iterator>
/*NOTE: does not implement
    operator<
    operator>
    operator<=
    operator>=
    std::swap<std::array>
    std::tuple_size<std::array>
    std::tuple_element<std::array>
*/
namespace std {
template <class T, std::size_t N> class array {
  public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type &reference;
	typedef const value_type &const_reference;
	typedef value_type *pointer;
	typedef const value_type *const_pointer;
	typedef class array_iterator : std::iterator<random_access_iterator_tag, T> {
	  private:
		array<T, N> *copy;
		size_type index;

	  public:
		array_iterator &operator+=(difference_type n) {
			index += n;
			return *this;
		}
		array_iterator operator+(difference_type n) { return array_iterator(*this) += n; }
		friend array_iterator operator+(difference_type n, array_iterator other) {
			return array_iterator(other) += n;
		}
		array_iterator &operator-=(difference_type n) {
			index += -n;
			return *this;
		}
		array_iterator operator-(difference_type n) { return array_iterator(*this) -= n; }
		difference_type operator-(array_iterator b) { return this->index - b.index; }
		reference operator[](difference_type n) { return copy[n]; }
		reference operator*() { return copy[index]; }
		pointer operator->() { return std::addressof(copy[index]); }
		bool operator<(array_iterator b) { return b - *this > 0; }
		bool operator>(array_iterator b) { return b < *this; }
		bool operator<=(array_iterator b) { return !(*this > b); }
		bool operator>=(array_iterator b) { return !(*this < b); }
		array_iterator(array<T, N> &arr, size_type idx) {
			copy = &arr;
			index = idx;
		}
		array_iterator(array_iterator &r) {
			copy = r.copy;
			index = r.index;
		}
		constexpr array_iterator() {}
	} iterator;
	typedef class const_array_iterator : std::iterator<random_access_iterator_tag, T> {
	  private:
		const array<T, N> *copy;
		size_type index;

	  public:
		const_array_iterator &operator+=(difference_type n) {
			index += n;
			return *this;
		}
		const_array_iterator operator+(difference_type n) { return array_iterator(*this) += n; }
		friend const_array_iterator operator+(difference_type n, array_iterator other) {
			return array_iterator(other) += n;
		}
		const_array_iterator &operator-=(difference_type n) {
			index += -n;
			return *this;
		}
		const_array_iterator operator-(difference_type n) { return array_iterator(*this) -= n; }
		difference_type operator-(array_iterator b) { return this->index - b.index; }
		const_reference operator[](difference_type n) { return copy[n]; }
		const_reference operator*() { return copy[index]; }
		const_pointer operator->() { return std::addressof(copy[index]); }
		bool operator<(array_iterator b) { return b - *this > 0; }
		bool operator>(array_iterator b) { return b < *this; }
		bool operator<=(array_iterator b) { return !(*this > b); }
		bool operator>=(array_iterator b) { return !(*this < b); }
		const_array_iterator(array<T, N> &arr, size_type idx) {
			copy = &arr;
			index = idx;
		}
		const_array_iterator(const_array_iterator &r) {
			copy = r.copy;
			index = r.index;
		}
		constexpr const_array_iterator() {}
	} const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  private:
	T _data[N];

  public:
	reference at(size_type pos) {
		if(pos < N)
			return _data[pos];
		abort();
	}
	constexpr const_reference at(size_type pos) const {
		if(pos < N)
			return _data[pos];
		abort();
	}
	reference operator[](size_type pos) { return _data[pos]; }
	constexpr const_reference operator[](size_type pos) const { return _data[pos]; }

	reference front() { return _data[0]; }
	constexpr reference front() const { return _data[0]; }
	reference back() { return _data[N - 1]; }
	constexpr reference back() const { return _data[N - 1]; }
	pointer data() { return _data; }
	const pointer data() const { return _data; }
	iterator begin() { return iterator(*this, 0); }
	const_iterator cbegin() { return const_iterator(*this, 0); }
	iterator end() { return iterator(*this, N); }
	const_iterator cend() { return const_iterator(*this, N); }
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator crbegin() { return const_reverse_iterator(cend()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator crend() { return const_reverse_iterator(cbegin()); }
	constexpr bool empty() const { return !N; }
	constexpr bool size() const { return N; }
	constexpr bool max_size() const { return N; }
	void fill(const T &value) {
		for(size_type i = 0; i < N; i++) { _data[i] = value; }
	}
	void swap(array &other) {
		pointer temp = other._data;
		other._data = _data;
		_data = temp;
	}
};
template <class T, std::size_t N> bool operator==(const array<T, N> &lhs, const array<T, N> &rhs) {
	for(typename decltype(lhs)::size_type i = 0; i < N; i++) {
		if(lhs[i] != rhs[i])
			return false;
	}
	return true;
}
template <class T, std::size_t N> bool operator!=(const array<T, N> &lhs, const array<T, N> &rhs) {
	return !(lhs == rhs);
}
template <size_t I, class T, size_t N> constexpr T &get(array<T, N> &a) {
	return a.at(I);
}
template <size_t I, class T, size_t N> constexpr T &&get(array<T, N> &&a) {
	return a.at(I);
}
template <size_t I, class T, size_t N> constexpr const T &get(const array<T, N> &a) {
	return a.at(I);
}
}
#endif
