/*
 * Copyright 2014 Matthew Fosdick

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
namespace kernel {
	namespace paging {
		uintptr_t *stack;
		ssize_t top;
		size_t max;
		void init_pfa() {
			stack=reinterpret_cast<uintptr_t *>(workspace_alloc_ptr);
			top=0;
			max=(static_cast<uintptr_t>(em_page_max())-static_cast<uintptr_t>(workspace_alloc_ptr))/sizeof(uintptr_t);
		}
		void *get_frame() {
			if(top<0) {
				return static_cast<void *>stack[top--]
			}
			kernel::panic("cannot fill request, not enough free frames");
		}
		void free_frame(void * frame) {
			stack[top++]=static_cast<uintptr_t>(frame);
			if(top>max) {
				//request more room
				if(paging_enabled()) {
					map_virt(&stack[top]);
					max+=4096/sizeof(uintptr_t);
				}else {
					em_page(&stack[top],frame);
					top--;
					max+=4096/sizeof(uintptr_t);
				}
			}
		}
		void free_frames(void * frames,size_t count) {
			uintptr_t f=static_cast<uintptr_t>(frames);
			for(size_t s=0;s<count;s++) {
				top_frame(static_cast<void *>(f));
				f+=0x1000;
			}
		}
		void *get_frames_adj(size_t frames) {
			if(frames>top)kernel::panic("cannot fill request, not enough free frames");
			kernel::panic("cannot fill request, adjacent frames not implemented");
		}
		void **get_frames(size_t frames) {
			if(frames>top)kernel::panic("cannot fill request, not enough free frames");
			kernel::panic("cannot fill request, multiple frames not implemented,requires a heap");
		}
	}
}