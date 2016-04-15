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

#pragma once
#include <hal/memregion.h>
namespace hal {
	class memmap;
	struct region_hook{
		region_hook* next;
		void (*add_region_hook)(memmap *mmap);
		region_hook(memmap &map, void (add_region_hook)(memmap *mmap))RUN_ONCE;
	};
	class memmap {
		int add_count;
		region_hook *next;
	public:
		mem_regs regs;
		int region_count();
		mem_region *get_region(int index);
		void add_regions(int count);
		bool add_region(uint64_t start, uint64_t end,mem_type type);
		void add_region_hook(region_hook * rhook);
		bool init();
	};
	extern memmap virtmem;
	extern memmap physmem;
}
