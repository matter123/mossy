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

#ifndef MULTIBOOT_H
#define MULTIBOOT_H
#include <stdint.h>
#include <struct.h>

namespace kernel {
	struct multiboot_t{
		uint32_t flags;
		uint32_t mem_low;
		uint32_t mem_high;
		uint32_t boot_device;
		uint32_t cmdline_ptr;
		uint32_t mods_count;
		uint32_t mods_ptr;
		uint32_t num;
		uint32_t size;
		uint32_t addr;
		uint32_t shndx;
		uint32_t mmap_length;
		uint32_t mmap_ptr;
		uint32_t drives_length;
		uint32_t drives_ptr;
		uint32_t config_table;
		uint32_t apm_table;
		uint32_t vbe_control_info_ptr;
		uint32_t vbe_mode_info_ptr;
		uint32_t vbe_mode;
		uint32_t vbe_api_seg;
		uint32_t vbe_api_off;
		uint32_t vbe_api_len;
	} PACKED;
}


#endif /* MULTIBOOT_H_ */
