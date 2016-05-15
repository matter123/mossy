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
#ifndef __ITERATOR_BACK_INSERT_HPP
#define __ITERATOR_BACK_INSERT_HPP
// NOTE this fully implements back_insert_iterator
#include <iterator>
namespace std {
template <class Container>
class back_insert_iterator
    : public std::iterator<std::output_iterator_tag, void, void, void, void> {
  protected:
	Container *container;

  public:
	explicit back_insert_iterator(Container &c) { this->c = std::addressof(c); }
	back_insert_iterator<Container> &operator=(typename Container::const_reference value) {
		container->push_back(value);
	}
	back_insert_iterator<Container> &operator=(typename Container::value_type &&value) {
		container->push_back(std::move(value));
	}
	back_insert_iterator &operator*() { return *this; }
	back_insert_iterator &operator++() { return *this; }
	back_insert_iterator &operator++(int) { return *this; }
};
template <class Container> std::back_insert_iterator<Container> back_inserter(Container &c) {
	return std::back_insert_iterator<Container>(c);
}
}
#endif
