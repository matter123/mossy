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

#ifndef MMAP_H_
#define MMAP_H_
#include "../../init/multiboot.h"
#include <struct.h>
#include <stdint.h>
namespace kernel {
	struct mmap_field_t{
		uint32_t size;
		uint64_t start;
		uint64_t end;
		/*bits 32-3 are a bit-field
		the type is 32 bits because of e820 memory map spacing
		bits 2-0 create a number from 0-7 for basic ram type
		0 invalid
		1 usable
		2 reclaimable
		3 reserved
		4 save on hibernate
		5 kernel ram
		6 unknown(reserved)
		7 unusable ram
		bit 3 - invalid entry
		bit 4-32 - undefined
		*/
		uint32_t type;
	}PACKED;
	multiboot_t *fix_tables(multiboot_t *mboot);
	void parse_mboot_mmap(multiboot_t *mboot);
	void init_page_frame_alloc();
	bool is_valid_mem(void *addr);
}
#endif
