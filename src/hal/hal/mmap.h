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
#include <stdint.h>
#include <arch.h>
#include <struct.h>
namespace hal {
	class mem_type {
		private:
			uint32_t resv_1=0;
			uint16_t resv_2=0;
			uint resv_n:7;
		public:
			bool avil:1;
			bool resv_mem:1;
			bool kernel:1;
			bool bootinfo:1;
			bool firmware:1;
			bool videobuffer:1;
			bool dma:1;
			bool no_exist:1;
			bool pci_mmap:1;
			bool save_on_hib:1;
			bool is_more_restrictive(mem_type other);
			bool can_adjust_start();
			bool can_shrink();
			bool can_grow();
			bool operator==(const mem_type &other);
	} PACKED;
	struct mem_region_init {
		uint64_t start;
		uint64_t end;
		mem_type type;
	} PACKED;
	struct mem_region {
		uint64_t start;
		mem_type type;
	} PACKED;

	bool init_mem();

	int get_mem_regions();
}