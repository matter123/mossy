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
#ifndef __ITERATOR_INSERT_HPP
#define __ITERATOR_INSERT_HPP
// NOTE this fully implements insert_iterator
#include <iterator>
namespace std {
template <class Container>
class insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void> {
  protected:
	Container *container;
	typename Container::iterator iter;

  public:
	explicit insert_iterator(Container &c, typename Container::iterator i) {
		this->c = std::addressof(c);
		iter(i);
	}
	insert_iterator<Container> &operator=(typename Container::const_reference value) {
		iter = container->insert(iter, value);
		++iter;
	}
	insert_iterator<Container> &operator=(typename Container::value_type &&value) {
		iter = container->insert(iter, std::move(value));
		++iter;
	}
	insert_iterator &operator*() { return *this; }
	insert_iterator &operator++() { return *this; }
	insert_iterator &operator++(int) { return *this; }
};
template <class Container>
std::insert_iterator<Container> inserter(Container &c, typename Container::iterator i) {
	return std::insert_iterator<Container>(c, i);
}
}
#endif
