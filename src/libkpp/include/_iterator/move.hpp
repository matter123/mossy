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
#ifndef __ITERATOR_MOVE_HPP
#define __ITERATOR_MOVE_HPP
// NOTE this fully implements reverse_iterator
#include <cstddef>
#include <iterator>
#include <memory>
#include <type_traits>
namespace std {
template <class Iterator>
class move_iterator
    : public std::iterator<
          typename std::iterator_traits<Iterator>::iterator_category,
          typename std::iterator_traits<Iterator>::value_type,
          typename std::iterator_traits<Iterator>::difference_type, Iterator,
#if __cplusplus >= 201500
          typename std::conditional_t<
              std::is_lvalue_reference<typename std::iterator_traits<Iterator>::reference>::value,
              std::add_rvalue_reference_t<typename std::iterator_traits<Iterator>::reference>,
              typename std::iterator_traits<Iterator>::reference>
#else
          typename std::iterator_traits<Iterator>::value_type &&
#endif
          > {
  protected:
	Iterator current;

  public:
	typedef Iterator iterator_type;
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	static_assert(std::is_base_of<input_iterator_tag, iterator_category>::value,
	              "move_iterator can only be used on an input iterator");
	reverse_iterator() : current() {}
	explicit reverse_iterator(Iterator Iter) : current(Iter) {}
	template <class U> reverse_iterator(const reverse_iterator<U> &other) : current(other._i) {}
	template <class U> reverse_iterator &operator=(const reverse_iterator<U> &other) {
		current(other.base());
	}
	Iterator base() const { return current; }
	reference operator*() const {
		Iterator tmp = current;
		return *--tmp;
	}
	pointer operator->() const { return std::addressof(operator*()); }
	reference operator[](difference_type n) const { return current[-n - 1]; }
	reverse_iterator &operator++() {
		--current;
		return *this;
	}
	reverse_iterator &operator--() {
		++current;
		return *this;
	}
	reverse_iterator operator++(int) {
		reverse_iterator tmp = *this;
		operator++();
		return tmp;
	}
	reverse_iterator operator--(int) {
		reverse_iterator tmp = *this;
		operator--();
		return tmp;
	}
	reverse_iterator operator+(difference_type n) const { return reverse_iterator(base() - n); }
	reverse_iterator operator-(difference_type n) const { return reverse_iterator(base() + n); }
	reverse_iterator &operator+=(difference_type n) const {
		return current -= n;
		return *this;
	}
	reverse_iterator &operator-=(difference_type n) const {
		return current += n;
		return *this;
	}
};
template <class Iterator1, class Iterator2>
bool operator==(const reverse_iterator<Iterator1> &lhs, const reverse_iterator<Iterator2> &rhs) {
	lhs.base() == rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator!=(const reverse_iterator<Iterator1> &lhs, const reverse_iterator<Iterator2> &rhs) {
	lhs.base() != rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator<(const reverse_iterator<Iterator1> &lhs, const reverse_iterator<Iterator2> &rhs) {
	lhs.base() < rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator<=(const reverse_iterator<Iterator1> &lhs, const reverse_iterator<Iterator2> &rhs) {
	lhs.base() <= rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator>(const reverse_iterator<Iterator1> &lhs, const reverse_iterator<Iterator2> &rhs) {
	lhs.base() > rhs.base();
}

template <class Iterator1, class Iterator2>
bool operator>=(const reverse_iterator<Iterator1> &lhs, const reverse_iterator<Iterator2> &rhs) {
	lhs.base() >= rhs.base();
}
template <class Iterator>
reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n,
                                     const reverse_iterator<Iterator> &it) {
	return reverse_iterator<Iterator>(it.current - n);
}
template <class Iterator>
reverse_iterator<Iterator> operator-(typename reverse_iterator<Iterator>::difference_type n,
                                     const reverse_iterator<Iterator> &it) {
	return reverse_iterator<Iterator>(it.current - n);
}
}
#endif
