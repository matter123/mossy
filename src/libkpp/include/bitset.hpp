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
#ifndef __BITSET_HPP
#define __BITSET_HPP
#include <cstddef>
namespace std {
template <std::size_t N> class bitset {
  private:
	static constexpr int data_size = N / 8 + (N % 8 ? 1 : 0);
	uint8_t data[data_size];

  public:
	class reference {
		size_t _index, _offset;
		bitset &_bs;

	  public:
		reference(bitset &bs, size_t index, size_t offset)
		    : _index(index), _offset(offset), _bs(bs) {}

		reference &operator=(bool b) {
			if(b) {
				uint8_t mask = 1 << _offset;
				_bs.data[_index] |= mask;
			} else {
				uint8_t mask = ~(1 << _offset);
				_bs.data[_index] &= mask;
			}
			return *this;
		}
		operator bool() const { return _bs.data[_index] & (1 << _offset); }
		bool operator~() const { return !(*this); }
		reference &flip() { return (*this) = ~(*this); }
	};

	constexpr bitset() { data = {0}; }
	constexpr bitset(unsigned long long val) {
		for(int i = 0; i < (N > sizeof(val) ? sizeof(val) : N); i++) {
			(*this)[i] = val & 1;
			val = val >> 1;
		}
	}
	bool operator==(const bitset<N> &rhs) const {
		for(int i = 0; i < N; i++) {
			if((*this)[i] != rhs[i]) {
				return false;
			}
		}
		return true;
	}
	bool operator!=(const bitset<N> &rhs) const { return !(rhs == *this); }
	constexpr bool operator[](std::size_t pos) const {
		return this->data[pos / 8] & (1 << pos % 8);
	}
	reference operator[](std::size_t pos) { return reference(*this, pos / 8, pos % 8); }
	bool test(std::size_t pos) { return (*this)[pos]; }
	bool all() const {
		for(int i = 0; i < N; i++) {
			if((*this)[i] != true) {
				return false;
			}
		}
		return true;
	}
	bool any() const {
		for(int i = 0; i < N; i++) {
			if((*this)[i] == true) {
				return true;
			}
		}
		return false;
	}
	bool none() const { return !all(); };
	std::size_t count() const {
		std::size_t c = 0;
		for(int i = 0; i < N; i++) {
			if((*this)[i] == true) {
				c++;
			}
		}
		return c;
	}
	constexpr std::size_t size() const { return N; }
	bitset<N> &operator&=(const bitset<N> &other) {
		for(int i = 0; i < data_size; i++) { data[i] &= other.data[i]; }
		return *this;
	}
	bitset<N> &operator|=(const bitset<N> &other) {
		for(int i = 0; i < data_size; i++) { data[i] |= other.data[i]; }
		return *this;
	}
	bitset<N> &operator^=(const bitset<N> &other) {
		for(int i = 0; i < data_size; i++) { data[i] ^= other.data[i]; }
		return *this;
	}
	bitset<N> operator~() const {
		bitset<N> ret(*this);
		for(int i = 0; i < data_size; i++) { ret.data[i] = ~ret.data[i]; }
		return ret;
	}
	bitset<N> &operator<<=(std::size_t pos) {
		if(pos >= N) {
			for(int i = 0; i < N; i++) { (*this)[i] = false; }
		} else {
			for(int i = pos; i < N + pos; i++) {
				if(i < N) {
					(*this)[i - pos] = (*this)[i];
				} else {
					(*this)[i - pos] = false;
				}
			}
		}
		return *this;
	}
	bitset<N> operator<<(std::size_t pos) { return bitset<N>(*this) <<= pos; }; //; fixes parsing
	bitset<N> &operator>>=(std::size_t pos) {
		if(pos >= N) {
			for(int i = 0; i < N; i++) { (*this)[i] = false; }
		} else {
			for(int i = N - 1; i >= 0; i--) {
				if(i - (long)pos >= 0) {
					(*this)[i] = (*this)[i - pos];
				} else {
					(*this)[i] = 0;
				}
			}
		}
		return *this;
	}
	bitset<N> operator>>(std::size_t pos) { return bitset<N>(*this) <<= pos; }
	bitset<N> &set() {
		for(int i = 0; i < N; i++) { (*this)[i] = true; }
		return *this;
	}
	bitset<N> &set(std::size_t pos, bool val = true) {
		(*this)[pos] = val;
		return *this;
	}
	bitset<N> &reset() {
		for(int i = 0; i < N; i++) { (*this)[i] = false; }
		return *this;
	}
	bitset<N> &reset(std::size_t pos) {
		(*this)[pos] = false;
		return *this;
	}
	bitset<N> &flip() { return (*this) = ~(*this); }
};
}
#endif
