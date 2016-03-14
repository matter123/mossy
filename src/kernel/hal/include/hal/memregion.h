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
#include <struct.h>
namespace hal {
	struct mem_region {
		uint64_t start;
		uint64_t end;
		mem_type type;
	} PACKED;
	struct mem_regs {
		mem_region *regions;
		uint64_t tag_count;
	};
	mem_regs *remove_invalid(mem_regs *regs);
	mem_regs *sort(mem_regs *regs);
	mem_regs *split(mem_regs *regs);
	mem_regs *page_align(mem_regs *regs);
	mem_regs *fill(mem_regs *regs);
}
