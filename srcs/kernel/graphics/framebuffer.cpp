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
#include <graphics/framebuffer.h>
#include <stdlib.h>
#include <string.h>
namespace kernel {
	namespace graphics {
		framebuffer::framebuffer(int w, int h) {
			this->buf=reinterpret_cast<uint32_t *>(calloc(sizeof(uint32_t),w*h));
			this->width=w;
			this->height=h;
		}
		framebuffer::~framebuffer() {
			free(this->buf);
		}
		void framebuffer::fill(uint32_t c) {
			memset32(this->buf, c, this->width*this->height);
		}
		#define curpos(a) ((a##y+y)*a##width)+(a##x+x)
		void framebuffer::copy_rect(FrameBuffer *sbuf, int srcx, int srcy, int destx, int desty, int wi, int hi) {
			uint32_t *source=sbuf->buffer();
			int destwidth=this->width;
			int srcwidth=sbuf->Width();
			for(int y=0;y<hi;y++) {
				for(int x=0;x<wi;x++) {
					this->buf[curpos(dest)]=source[curpos(src)];
				}
			}
		}
		#define BB_OP(NAME, INS)\
		void BBFN_##NAME(uint32_t * src,uint32_t * dst,  \
		                 int destx, int desty,           \
		                 int wi, int hi, int destwidth) {\
			int srcwidth=wi;                             \
			int srcx=0;                                  \
			int srcy=0;                                  \
			for(int y=0;y<hi;y++) {                      \
				for(int x=0;x<wi;x++) {                  \
					INS;                                 \
				}                                        \
			}                                            \
		}
		#pragma clang diagnostic push
		#pragma clang diagnostic ignored "-Wunused-variable"
		BB_OP(OR,dst[curpos(dest)]|=src[curpos(src)]);
		BB_OP(AND,dst[curpos(dest)]&=src[curpos(src)]);
		BB_OP(XOR,dst[curpos(dest)]^=src[curpos(src)]);
		BB_OP(NOT_DEST,dst[curpos(dest)]=~dst[curpos(dest)]);
		BB_OP(NOT_SRC,dst[curpos(dest)]=~src[curpos(src)]);
		#pragma clang diagnostic pop
		void framebuffer::bit_blit(FrameBuffer *srcbuffer,
		                           int destx, int desty,
		                           bit_blit_op operation) {
			int wi=srcbuffer->Width();
			int hi=srcbuffer->Height();
			switch (operation) {
				case BB_OR: return BBFN_OR(srcbuffer->buffer(),this->buf,destx,desty,wi,hi,this->width);
				case BB_AND: return BBFN_AND(srcbuffer->buffer(),this->buf,destx,desty,wi,hi,this->width);
				case BB_XOR: return BBFN_XOR(srcbuffer->buffer(),this->buf,destx,desty,wi,hi,this->width);
				case BB_NOT_DEST: return BBFN_NOT_DEST(srcbuffer->buffer(),this->buf,destx,desty,wi,hi,this->width);
				case BB_NOT_SRC: return BBFN_NOT_SRC(srcbuffer->buffer(),this->buf,destx,desty,wi,hi,this->width);
			}
		}
		int framebuffer::Width() {return this->width;}
		int framebuffer::Height() {return this->width;}
		uint32_t *framebuffer::buffer() {return this->buf;}
	}
}
