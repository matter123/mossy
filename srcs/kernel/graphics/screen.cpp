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
		screen::screen(void *v, int w, int h):framebuffer(w,h) {
			this->otherbuf=reinterpret_cast<uint32_t *>(calloc(sizeof(uint32_t),w*h));
			this->vram=v;
		}
		screen::~screen() {
			//should never really  be called, ut just in case
			free(otherbuf);
		}
	}
}
