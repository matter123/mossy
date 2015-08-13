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
#include <graphics/framebuffer.h>
#include <stdlib.h>
#include <string.h>
namespace kernel {
	namespace graphics {
		framebuffer::framebuffer(int w, int h) {
			this->buffer=calloc(sizeof(uint32_t),w*h);
			this->width=w;
			this->height=h;
		}
		framebuffer::~framebuffer() {
			free(this->buffer);
		}
		void framebuffer::fill(uint32_t c) {
			memset32(this->buffer, c, this->width*this->height)
		}
		#define curpos(a) (a##y+y*this->width)+(a##x+x)
		void framebuffer::copy_rect(FrameBuffer *buf, int srcx, int srcy, int destx, int desty, int wi, int hi) {
			uint32_t *source=buf->buffer();
			for(int y=0;y<hi;y++) {
				for(int x=0;x<wi;x++) {
					this->buffer[(desty+y*this->width)+(destx+x)]=source[(srcy+y*this->width)+(srcx+x)]
				}
			}
		}
		BB_OP(OR,)
		void framebuffer::bit_blit(FrameBuffer *srcbuffer,
		                           int destx, int desty,
		                           bit_blit_op operation) {
			switch (operation) {
				case /* value */:
			}
		#define BB_OP(NAME, INS)\
		void BB_##NAME(uint32_t * src,\
		           int destx, int desty,\
		           int wi, int hi) {\
			uint32_t *dst=this->buffer;\
			int srcx, srcy;srcx=srcy=0;
			for(int y=0;y<hi;y++) {\
				for(int x=0;x<wi;x++) {\
					INS\
				}\
			}\
		}\
	}
}
