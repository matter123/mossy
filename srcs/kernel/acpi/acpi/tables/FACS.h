/*
    Copyright 2014-15 Matthew Fosdick

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
namespace acpi {
	struct FACS {
			char signature[5];
			uint32_t hardware_signature;
			bool S4BIOS_supported;
			bool _64_bit_wake_supported;
			void install_waking_vector(void *vector);
			void *retrieve_waking_vector();
			void set_64_bit_wake(bool _64_bit_wake);
			bool acquire_global_lock();
			bool release_global_lock();
			bool OSPM_own_global_lock();
		private:
			void *FACS_firmware_addr;
			bool own_lock;
	};
}
