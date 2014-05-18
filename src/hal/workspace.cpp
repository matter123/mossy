/*
    Copyright 2013 Matthew Fosdick

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

#include <hal/workspace.h>
namespace hal {
	extern "C" uint32_t k_end;      /*we have 128kb after this reserved*/
	extern "C" uint32_t k_data_end; /*end of working space, PFA and paging needs
                                to be up by then so we don't run out of room*/
	void *wksp_ptr=&k_end;
	void *w_malloc(size_t s) {
		void *temp=wksp_ptr;
		if(wksp_ptr+s>=&k_data_end) {
			return reinterpret_cast<void *>(0);
		}
		wksp_ptr+=s;
		return temp;
	}
	void  wksp_begin(void *begin) {
		wksp_ptr=begin;
	}
}