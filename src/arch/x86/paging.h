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

#ifndef PAGING_H
#define PAGING_H
#include <struct.h>
namespace x86 {
	namespace paging {
		struct page_dir_e {
			int addr :20;
			int unused :4;
			bool big :1;
			bool zero :1;
			bool accessed :1;
			bool no_cache :1;
			bool write_through :1;
			bool user :1;
			bool read_write :1;
			bool present :1;
		}PACKED;

		struct page_table_e {
			int addr :20;
			int unused :3;
			bool global :1;
			bool zero :1;
			bool dirty :1;
			bool accessed :1;
			bool no_cache :1;
			bool write_through :1;
			bool user :1;
			bool read_write :1;
			bool present :1;
		}PACKED;

		struct page_dir {
			union {
				page_dir_e entries[1024];
				int entriesi[1024];
			};
		}PACKED;

		struct page_table {
			union {
				page_table_e entries[1024];
				int entriesi[1024];
			};
		}PACKED;

		void init_paging();
		void map_virt(page_dir * d,void *virt,void *(*alloc_a)());
		void map_addr(page_dir * d,void *phys,void *virt,void *(*alloc_a)());
	}
}

#endif
