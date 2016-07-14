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
#ifndef __ALGORITHM_HPP
#define __ALGORITHM_HPP
namespace std {
template <class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
	for(; (first1 != last1) && (first2 != last2); first1++, (void)first2++) {
		if(*first1 < *first2) return true;
		if(*first2 < *first1) return false;
	}
	return (first1 == last1) && (first2 != last2);
}
template <class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, Compare comp) {
	for(; (first1 != last1) && (first2 != last2); first1++, (void)first2++) {
		if(comp(*first1, *first2)) return true;
		if(comp(*first2, *first1)) return false;
	}
	return (first1 == last1) && (first2 != last2);
}
template <class T> constexpr const T &min(const T &a, const T &b) { return a < b ? a : b; }
template <class T, class Compare> constexpr const T &min(const T &a, const T &b, Compare comp) {
	return comp(a, b) ? a : b;
}
template <class T> constexpr const T &max(const T &a, const T &b) { return a > b ? a : b; }
template <class T, class Compare> constexpr const T &max(const T &a, const T &b, Compare comp) {
	return !comp(a, b) ? a : b;
}
}
#endif
