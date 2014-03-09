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
#include "multiboot.h"
#include "modules.h"
#include <stdint.h>
#include <math.hpp>
#include <struct.h>
#include "../monitor.h"

namespace debug {
	kernel::mboot_module_t *symtable;
	struct symbol_entry {
		uint32_t magic;
		uintptr_t addr;
		char message[40];
	}PACKED;
	symbol_entry* symbols;
	int count;
	bool load_symbols() {
		for(int i=0; i<kernel::get_module_count(); i++) {
			if(kernel::get_magic(i)==0x5677AB1E)symtable=kernel::get_module(i);
		}
		if(symtable==nullptr)return false;
		count=*reinterpret_cast<int *>(symtable->mod_start+4);
		symbols=reinterpret_cast<symbol_entry *>(symtable->mod_start+8);
		for(int i=0;i<count;i++) {
			if(symbols[i].magic!=0x00E67126)return false;
			//if(i%10==0)std::cout<<symbols[1].message<<std::hex<<" "<<symbols[0].addr<<std::endl;
		}
		return true;
	}

	int symbol_count() {
		return count;
		//return 0;
	}

	char* get_symbol(uintptr_t instruction,uint32_t &off) {
		int pindex=-1;
		for(int i=0;i<count;i++) {
			if(symbols[i].addr<=instruction) {
				if(pindex==-1||symbols[pindex].addr<symbols[i].addr) {
					pindex=i;
				}
			}
		}
		if(pindex==-1)pindex=0;
		off=static_cast<uint32_t>(instruction-symbols[pindex].addr);
		return symbols[pindex].message;
	}
}