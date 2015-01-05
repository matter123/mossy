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
#include <stdint.h>
namespace hal {
	struct page_opt {
		bool user;
		bool exec;
		bool write;
	};
	void map_free_to_virt_cur(uintptr_t virt, page_opt options);

	void map_phys_to_virt_cur(uintptr_t virt, uint64_t phys, page_opt options);

	void unmap_virt_cur(uintptr_t virt);

	uintptr_t unmap_virt_phys_cur(uintptr_t virt);

	uintptr_t phys_from_virt(uintptr_t virt);

}
