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
#pragma once
#include <arch.h>
#ifdef X86
#include <stdint.h>
#include <stdlib.h>
#include <struct.h>
#include <hal/paging.h>
namespace x86 {
	class recursive_paging {
			struct pde {
				bool present :1;
				bool read_write :1;
				bool user :1;
				bool write_through :1;
				bool no_cache :1;
				bool accessed :1;
				bool zero :1;
				bool big :1;
				int unused :4;
				int addr :20;
			} PACKED;
			struct pte {
				bool present :1;
				bool read_write :1;
				bool user :1;
				bool write_through :1;
				bool no_cache :1;
				bool accessed :1;
				bool dirty :1;
				bool zero :1;
				bool global :1;
				int unused :3;
				int addr :20;
			} PACKED;
			union pd {
				pde entries[1024];
				uint32_t entriesi[1024];
			} PACKED;
			union pt {
				pte entries[1024];
				uint32_t entriesi[1024];
			} PACKED;
			struct page_map {
				pt tables[1023];
				pd master;
			} PACKED;
			uintptr_t (*alloc_func)();
			page_map *pmap;
		public:
			void init(uintptr_t (*a_func)());
			void map(uintptr_t phys,uintptr_t virt,hal::page_opt options);
			uintptr_t unmap(uintptr_t virt);
			uintptr_t locate(uintptr_t virt);
	};
	bool init_paging();
	bool paging_ready();
}
#endif