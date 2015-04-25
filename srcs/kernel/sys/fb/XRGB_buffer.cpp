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
class XRGB_buffer:buffer {

};
void XRGB_buffer::fill(color c) {
	uintptr_t addr=this->buffer;
	for(int i=0;i<this->height;i++) {
		for(int w=0;w<this->width;j++) {
			*static_cast<uint32_t *>(addr+w*4)=c.red<<16|c.green<<8|c.blue;
		}
		addr+=this->stride;
	}
	
}
