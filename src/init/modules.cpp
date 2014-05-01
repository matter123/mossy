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

#include <stdint.h>
#include "multiboot.h"
//#include "../arch/x86/mmap.h"
#include "modules.h"
#include <string.h>

namespace kernel {
	/*
	    void *fix(uintptr_t ptr) {
	    if(ptr<HIGH_HALF_BASE_ADDR)ptr+=HIGH_HALF_BASE_ADDR;
	    return reinterpret_cast<void *>(ptr);
	    }
	    int count;
	    mboot_module_t *map;
	    void load_modules(multiboot_t *mboot) {
	    //at this point the module structure is fixed but no the pointers in the structure
	    map=reinterpret_cast<mboot_module_t *>(mboot->mods_ptr);
	    uint8_t *pos=reinterpret_cast<uint8_t *>(workspace_alloc_ptr);
	    for(int i=0;i<mboot->mods_count;i++) {
	        //like mmap, size is really end, this fixes it
	        map[i].mod_size=map[i].mod_size-map[i].mod_start;
	        map[i].mod_start=reinterpret_cast<uintptr_t>(std::memmove(pos,fix(map[i].mod_start),map[i].mod_size));
	        pos+=map[i].mod_size;
	    }
	    set_workspace_begin(reinterpret_cast<void *>(pos));
	    count=mboot->mods_count;
	    }

	    int get_module_count() {
	    return count;
	    }

	    mboot_module_t *get_module(int index) {
	    return &map[index];
	    }

	    //all modules recognized by mossy have a magic number in the first 4 bytes this reads it
	    uint32_t get_magic(int index) {
	    return *(reinterpret_cast<uint32_t *>(map[index].mod_start));
	    }
	*/
}