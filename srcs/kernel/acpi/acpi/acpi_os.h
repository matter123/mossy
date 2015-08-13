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
#include <stddef.h>
namespace acpi {
	namespace os {
		/**
		    @brief initalizes acpi os interface
		    @details this is called by libacpi when initalizing itself
		    @return true on sucess false otherwise

		    @date created on 2014-11-03
		*/
		bool init_acpi_os();
		/**
		    @brief allocates firmware memory
		    @details allocates memory from the virtual firware block that
		    covers the area described by \a phys and \a len

		    @param phys base address of physical block of memory to acsess
		    @param len length of blovk of memory
		    @param[out] alloc_len if non-NULL will be filled with how much was over
		    allocated and can be safely used
		    @return address to start of the virtual firmware block allocated

		    @date created on 2014-11-03

		    @sa alloc_mem
		*/
		uintptr_t get_virt_phys(uintptr_t phys, uintptr_t len, uintptr_t *alloc_len);
		/**
		    @brief deallocates firmware memory
		    @details deallocates a block of virtual firmware memory

		    @param phys base addres of physical block of memory that was allocated, MUST
		    be the same as what was passed to get_virt_phys
		    @param len length of physical block of memory that was allocated, MUST be the
		    the same as what was passed to get_virt_phys
		    @param virt virtual address returned by get_virt_phys

		    @date created on 2014-11-03
		*/
		void unget_phys(uintptr_t phys, uintptr_t len, uintptr_t virt);
	}
}
