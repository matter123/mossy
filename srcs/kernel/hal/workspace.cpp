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
#include <hal/workspace.h>
#include <hal/console.h>
#include <stdlib.h>
#include <string.h>
namespace hal {
	extern "C" uint32_t k_end;      /*we have 128kb after this reserved*/
	extern "C" uint32_t k_data_end; /*end of working space, PFA and paging needs
                                to be up by then so we don't run out of room*/
	pointer wksp_ptr=(pointer) &k_end;
	void *w_malloc(size_t s) {
		if(s>sizeof(uintptr_t))wksp_ptr=
			    (pointer)(((uintptr_t)wksp_ptr&~(sizeof(uintptr_t)-1))+sizeof(uintptr_t));
		void *temp=wksp_ptr;
		if(wksp_ptr+s>(pointer)&k_data_end) {
			return reinterpret_cast<void *>(0);
		}
		wksp_ptr+=s;
		return temp;
	}
	void *w_malloc(size_t s, size_t align) {
		if(!(align&(align-1))) {
			wksp_ptr=(pointer)(((uintptr_t)wksp_ptr&~(align-1))+align);
		}
		void *temp=wksp_ptr;
		if(wksp_ptr+s>(pointer)&k_data_end) {
			return reinterpret_cast<void *>(0);
		}
		wksp_ptr+=s;
		return temp;
	}
	void  wksp_begin(void *begin) {
		if(begin>&k_data_end) {
			begin=&k_data_end;
		}
		wksp_ptr=(pointer)begin;
		uintptr_t diff=((pointer)&k_data_end-wksp_ptr)/1024;
		char msg[40]="wksp_begin called, ";
		itoa(diff,msg+19,10);
		strcat(msg," KiBs left");
		print_boot_msg("INFO","W_MALLOC",msg);
	}
}
