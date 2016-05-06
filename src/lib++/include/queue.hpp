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
#pragma once
#include <deque>
namespace std {
template <class T, class Container = std::deque<T>> class queue {
	explicit queue(const Container &cont);
	explicit queue(const Container &&cont = Container());
	// queue(const queue &other);  //implicitly declared
	// queue(const queue &&other);
	~queue();
	T &front();
	const T &front() const;
	void pop();
	T &back();
	const T &back() const;
	void push(const T &value);
	// void push(T &&value);
	bool empty();
	size_t size() const;
	void swap(queue &other);
};
}
