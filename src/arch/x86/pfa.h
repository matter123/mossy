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
#ifndef PFA_H
#define PFA_H
namespace kernel {
	namespace paging {
		void init_pfa();
		void *get_frame();
		void free_frame(void * frame);
		void free_frames(void * frames,size_t count);
		void *get_frames_adj(size_t count);
		void **get_frames(size_t count);
	}
}
#endif