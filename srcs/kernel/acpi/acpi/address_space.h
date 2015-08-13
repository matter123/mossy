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
namespace acpi {
	struct generic_address_space {
		enum address_space {
			MEMORY=0x00,
			IO=0x01,
			PCI=0x02,
			EMBEDDED_CONTROLLER=0x03,
			SMBUS=0x04,
			PLATFORM_COMMUNICATION_CHANNEL=0x0A,
			FUNCTIONAL_FIXED_HARDWARE=0x7F,
		} address_space;
		int reg_bit_width;
		int reg_bit_offset;
		int access_size;
		uint64_t address;
	};
}
