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
#ifndef __ITERATOR_REVERSE_HPP
#define __ITERATOR_REVERSE_HPP
// NOTE this fully implements reverse_iterator
#include <cstddef>
#include <iterator>
#include <memory>
namespace std {
template <class Iterator>
class reverse_iterator
    : public std::iterator<typename std::iterator_traits<Iterator>::iterator_category,
                           typename std::iterator_traits<Iterator>::value_type,
                           typename std::iterator_traits<Iterator>::difference_type,
                           typename std::iterator_traits<Iterator>::pointer,
                           typename std::iterator_traits<Iterator>::reference> {
  protected:
	Iterator current;

  public:
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
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
	return reverse_iterator<Iterator>(it.current + n);
}
template <class Iterator> std::reverse_iterator<Iterator> make_reverse_iterator(Iterator i) {
	return std::reverse_iterator<Iterator>(i);
}
}
#endif
