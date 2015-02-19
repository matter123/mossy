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
#include <acpi/tables/SDT.h>
namespace acpi {
	struct MADT_entry {
		uint8_t type;
	};
	struct local_APIC:MADT_entry {
		uint8_t processer_ID;
		uint8_t APIC_ID;
		bool enabled;
	};
	struct IO_APIC:MADT_entry {
		void *addr;
		void *interrupt_base;
		uint8_t APIC_ID;
	};
	struct interrupt_override:MADT_entry {
		uint8_t IRQ_source;
		uint32_t global_dest;
		enum polarity {
			POLARITY_CONFORMS=0,
			ACTIVE_HIGH=1,
			ACTIVE_LOW=3,
		} polarity;
		enum trigger_mode {
			TRIGGER_MODE_CONFORMS=0,
			EDGE_TRIGGERED=1,
			LEVEL_TRIGGERED=3,
		} trigger_mode;
	};
	struct unknown_MADT_entry:MADT_entry {
		uint8_t length;
		void *raw_table;
	};
	struct MADT:SDT {
		void *local_controller_addr;
		MADT_entry **entries;
		int entry_length;
		bool pc_compatibile;
	};
}
