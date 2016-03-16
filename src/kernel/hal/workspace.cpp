/*
	Copyright 2016 Matthew Fosdick
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
//workspace is for when you need memory pre heap, cannot de allocate so be careful
namespace hal {
	extern "C" uint32_t k_end;      /*we have 128kb after this reserved*/
	extern "C" uint32_t k_data_end;
	static pointer wksp_ptr=(pointer) &k_end;
	void *w_malloc(size_t s,size_t align=1) {
		if(s>=sizeof(uintptr_t)) { //ensure alignment for large types
			align=sizeof(uintptr_t);
		}
		if(align!=1 && (align&(align-1))==0) {
			wksp_ptr=(pointer)(((uintptr_t)wksp_ptr&~(align-1))+align);
		}
		void *temp=wksp_ptr;
		if(wksp_ptr+s>(pointer)&k_data_end) {
			return reinterpret_cast<void *>(0);
		}
		wksp_ptr+=s;
		return temp;
	}
	void wksp_reset(void *reset) {
		if(reset>&k_data_end) {
			reset=&k_data_end;
		}
		wksp_ptr=(pointer)reset;
	}
}
