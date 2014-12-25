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
#include <struct.h>
namespace acpi {
	namespace tables {
		struct RSDP {
			char sig[8];
			uint8_t checksum;
			char OEMID[6];
			uint8_t revision;
			uint32_t RSDTaddr;
		} PACKED;

		struct RSDP20 {
			RSDP begin;
			uint32_t length;
			uint64_t XSDTaddr;
			uint8_t ext_checksum;
			uint8_t resv[3];
		} PACKED;

		extern RSDP20 *rsdp_ptr;
		bool find_rsdp();
	}
}