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
		struct SDT {
			char Signature[4];
			uint32_t length;
			uint8_t revision;
			uint8_t checksum;
			char OEMID[6];
			char OEMTableID[8];
			uint32_t OEMrevision;
			uint32_t creatorID;
			uint32_t creator_revision;
		} PACKED;

		struct RSDT {
			SDT header;
			uint32_t tables[0];
		};

		struct XSDT {
			SDT header;
			uint64_t tables[0];
		};

		extern RSDT *rsdt;
		extern XSDT *xsdt;

		SDT *load_table(void *tbl_phys_base);

		bool do_checksum(SDT *table);
	}
}
