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

#pragma once
#include <graphics/shape.h>
namespace kernel {
	namespace graphics {
		enum bit_blit_op {
			BB_NOT_SRC,
			BB_NOT_DEST,
			BB_AND,
			BB_OR,
			BB_XOR,
			BB_BLEND,
			BB_CHAN_ADD,
			BB_CHAN_MUL,
			BB_CHAN_DIV,
			BB_CHAN_SUB,
			BB_CHAN_DIFF,
			BB_ADD,
			BB_MUL,
			BB_DIV,
			BB_SUB,
			BB_DIFF,
		};
		class FrameBuffer {
			//FrameBuffers and colors are XRGB8888 format
		protected:
			FrameBuffer() {}
			FrameBuffer(const FrameBuffer &) {}
			FrameBuffer & operator=(const FrameBuffer &) {return *this;}
		public:
			virtual ~FrameBuffer() {}
			virtual uint32_t *buffer()=0;
			virtual void fill(uint32_t color)=0;
			virtual void bit_blit(FrameBuffer *srcbuffer, int destx, int desty, bit_blit_op operation)=0;
			virtual void copy_rect(FrameBuffer *buffer, int srcx, int srcy, int destx, int desty, int width, int height)=0;
			void draw(Shape shape, int x, int y) {shape.draw(this,x,y);}
			virtual int Width()=0;
			virtual int Height()=0;
		};
		class framebuffer:public FrameBuffer {
		protected:
			uint32_t *buf;
			int width, height;
			framebuffer() {}
		public:
			virtual ~framebuffer();
			framebuffer(int width, int height);
			virtual uint32_t *buffer();
			virtual void fill(uint32_t color);
			virtual void bit_blit(FrameBuffer *srcbuffer, int destx, int desty, bit_blit_op operation);
			virtual void copy_rect(FrameBuffer *buffer, int srcx, int srcy, int destx, int desty, int width, int height);
			virtual int Width();
			virtual int Height();
		};
		class screen:public framebuffer {
		protected:
			uint32_t *otherbuf;
			void *vram;
		public:
			virtual ~screen();
			screen(void * vram,int width, int height);
			virtual void flush()=0;
		};
		class wraparrayFB: public framebuffer {
		public:
			wraparrayFB(const wraparrayFB &rhs) {this->buf=rhs.buf;this->width=rhs.width;this->height=rhs.height;}
			wraparrayFB & operator=(const wraparrayFB &) {return *this;}
			virtual ~wraparrayFB();
			wraparrayFB(uint32_t *arr,int width, int height);
			void usenew(uint32_t *arr,int width, int height);
		};
	}
}
