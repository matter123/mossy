/*
 * Copyright 2013 Matthew Fosdick

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

#include "mmap.h"
#include "../../monitor.h"


namespace kernel {
	void parse_mboot_mmap(multiboot_t *mboot) {
		std::cout<<std::bin<<"flags:"<<mboot->flags<<" meminfo valid:"<<((mboot->flags&(1<<6))?"yes":"no")<<std::endl;
		std::cout<<std::hex<<"mem info start:"<<mboot->mmap_ptr<<std::endl;
		std::cout<<std::dec<<"mem info len  :"<<mboot->mmap_length<<std::endl;
		std::cout<<mboot->mem_high<<std::endl;
		mmap_field_t *mmap=(mmap_field_t *)(mboot->mmap_ptr+0xC0000000);
		std::cout<<mmap->type<<std::endl<<std::bin<<1024;
	}
}
