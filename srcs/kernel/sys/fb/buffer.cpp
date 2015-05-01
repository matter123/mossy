/*
	Copyright 2014 Matthew Fosdick

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

#include <sys/fb.h>
#include <string.h>

namespace kernel {
	buffer::buffer(int w, int h, packing p, write_policy write) {
		this->width=w;
		this->height=h;
		this->pack=p;
		this->wp=write;
		this->cow=nullptr;
		this->stride=4*width;
		if (p==BGRX_16||p==BGR_16||
		    p==XRGB_16||p==RGB_16) {
			stride=2*width;
		}
		this->addr=(pointer)malloc(this->stride*h);
		memset(this->addr, 0, this->stride*h);
	}

	buffer::buffer(buffer &b) {
		this->width=b.width;
		this->height=b.height;
		this->stride=b.stride;
		this->wp=READ_WRITE;
		this->pack=b.pack;
		if(b.cow) {
			this->addr=(pointer)malloc(this->stride*this->height);
			memcpy(this->addr, b.addr, this->stride*this->height);
			return;
		}
		this->addr=b.addr;
		this->cow=&b;
		b.cow=this;
	}

	buffer::~buffer() {
		if(this->cow) {
			this->cow->cow=nullptr;
			return;
		}
		free(this->addr);
		this->addr=nullptr;
	}
}
