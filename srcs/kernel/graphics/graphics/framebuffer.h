/*
    Copyright 2015 Matthew Fosdick

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
#include <stdint.h>
#include <graphics/shape.h>
#include <memory.h>
namespace kernel {
	namespace graphics {
		enum bit_blit_op {
			BB_COPY,
			BB_NOT_SRC,
			BB_NOT_DEST,
			BB_AND,
			BB_OR,
			BB_BLEND,
			BB_CHAN_ADD,
			BB_CHAN_MUL,
			BB_CHAN_DIV,
			BB_CHAN_SUB,
			BB_ADD,
			BB_MUL,
			BB_DIV,
			BB_SUB,
		};
		class FrameBuffer {
		protected:
			FrameBuffer() {}
			FrameBuffer(const FrameBuffer &) {}
			FrameBuffer & operator=(const FrameBuffer &) {return *this;}
		public:
			virtual ~FrameBuffer() {};
			virtual uint32_t *buffer()=0;
			virtual void fill(uint32_t color)=0;
			//TODO make shared pointer and make that a non pointer
			virtual void bit_blit(FrameBuffer *srcbuffer, int destx, int desty, bit_blit_op operation)=0;
			//TODO make shared pointer and make that a non pointer
			virtual void copy_rect(FrameBuffer *buffer, int srcx, int srcy, int destx, int desty, int width, int height)=0;
			virtual void draw(Shape shape, int x, int y);
			virtual void width()=0;
			virtual void height()=0;
		};
		class framebuffer::FrameBuffer {
			uint32_t *buffer;
			int width, height
		public:
			framebuffer(int width, int height);
		};
		class screen {
		public:
			virtual void flush()=0;
		};
	}
}
