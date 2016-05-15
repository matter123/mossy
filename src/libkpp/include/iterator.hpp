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
#ifndef __ITERATOR_HPP
#define __ITERATOR_HPP
/*NOTE: does not implement:
    istream_iterator
    ostream_iterator
    istreambuf_iterator
    ostreambuf_iterator
    rbegin(T[])
    rend(T[])
    operator==(istream_iterator)
    operator!=(istream_iterator)
    operator==(istreambuf_iterator)
    operator!=(istreambuf_iterator)
    */
#include <cstddef>
#include <memory>
namespace std {
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class Iterator> struct iterator_traits {
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::iterator_category iterator_category;
};
template <class T> struct iterator_traits<T *> {
	typedef std::ptrdiff_t difference_type;
	typedef T value_type;
	typedef T *pointer;
	typedef T &reference;
	typedef std::random_access_iterator_tag iterator_category;
};
template <class T> struct iterator_traits<const T *> {
	typedef std::ptrdiff_t difference_type;
	typedef T value_type;
	typedef const T *pointer;
	typedef const T &reference;
	typedef std::random_access_iterator_tag iterator_category;
};

template <class Category, class T, class Distance = std::ptrdiff_t, class Pointer = T *,
          class Reference = T &>
struct iterator {
	typedef Category iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef Pointer pointer;
	typedef Reference reference;
};
template <class InputIt, class Distance> void advance(InputIt &it, Distance n) {
	static_assert(
	    n > 0 ||
	        std::is_base_of<bidirectional_iterator_tag, typename InputIt::iterator_category>::value,
	    "negative distance can only be used on bidirectional iterators");
	it += n;
}
template <class InputIt>
typename std::iterator_traits<InputIt>::difference_type distance(InputIt first, InputIt last) {
	return last - first;
}
template <class ForwardIt>
ForwardIt next(ForwardIt it, typename std::iterator_traits<ForwardIt>::difference_type n = 1) {
	std::advance(it, n);
	return it;
}
template <class BidirIt>
BidirIt prev(BidirIt it, typename std::iterator_traits<BidirIt>::difference_type n = 1) {
	std::advance(it, -n);
	return it;
}
template <class C> auto begin(C &c) -> decltype(c.begin()) {
	return c.begin();
}
template <class C> auto begin(const C &c) -> decltype(c.begin()) {
	return c.begin();
}
template <class T, std::size_t N> constexpr T *begin(T (&array)[N]) {
	return std::addressof(array[0]);
}
template <class C> constexpr auto cbegin(const C &c) -> decltype(std::begin(c)) {
	return std::begin(c);
}
template <class C> auto end(C &c) -> decltype(c.end()) {
	return c.end();
}
template <class C> auto end(const C &c) -> decltype(c.end()) {
	return c.end();
}
template <class T, std::size_t N> constexpr T *end(T (&array)[N]) {
	return std::addressof(array[N]);
}
template <class C> auto rbegin(C &c) -> decltype(c.rbegin()) {
	return c.rbegin();
}
template <class C> auto rbegin(const C &c) -> decltype(c.rbegin()) {
	return c.rbegin();
}
template <class C> auto crbegin(const C &c) -> decltype(std::rbegin(c)) {
	return std::rbegin(c);
}
template <class C> auto rend(C &c) -> decltype(c.rend()) {
	return c.rend();
}
template <class C> auto rend(const C &c) -> decltype(c.rend()) {
	return c.rend();
}
template <class C> auto crend(const C &c) -> decltype(std::rend(c)) {
	return std::rend(c);
}
}
#include <_iterator/back_insert>
#include <_iterator/front_insert>
#include <_iterator/insert>
#include <_iterator/move>
#include <_iterator/reverse>
#endif
