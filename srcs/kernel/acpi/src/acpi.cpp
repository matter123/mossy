/*
    Copyright 2015 Matthew Fosdick

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
#include <acpi/acpi_os.h>
#include <acpi/rsdp.h>
#include <acpi/sdt.h>
#include <struct.h>
namespace acpi {
	struct RSDT {
		sdt header;
		uint32_t tables[0];
	} PACKED *rsdt;
	struct XSDT {
		sdt header;
		uint64_t tables[0];
	} PACKED *xsdt;

	static bool do_checksum(sdt *table) {
		pointer bytes=reinterpret_cast<pointer>(table);
		uint8_t checksum=0;
		for(int i=0; i<table->length; i++) {
			checksum+=bytes[i];
		}
		return !checksum;
	}
	static sdt *load_table(void *tbl_phys_base) {
		uintptr_t alloc_size=0;
		sdt *table=(sdt *)os::get_virt_phys((uintptr_t)tbl_phys_base,sizeof(sdt),&alloc_size);
		int needed_length=table->length-sizeof(sdt);
		if(needed_length>alloc_size) {
			os::unget_phys((uintptr_t)tbl_phys_base,sizeof(sdt),(uintptr_t)table);
			table=(sdt *)os::get_virt_phys((uintptr_t)tbl_phys_base,sizeof(sdt)+needed_length,NULL);
		}
		if(!do_checksum(table)) {
			return NULL;
		}
		return table;
	}

	static bool h_acpi=false;
	bool has_acpi() {
		return h_acpi;
	}
	void init_tables() {
		os::init_acpi_os();
		RSDP20 *rsdp_ptr=find_rsdp();
		if(rsdp_ptr) {
			h_acpi=true;
		} else {
			return;
		}
		if(rsdp_ptr->begin.revision==0) {
			rsdt=(RSDT *)load_table((void *)(uintptr_t)rsdp_ptr->begin.RSDTaddr);
		} else {
			xsdt=(XSDT *)load_table((void *)(uintptr_t)rsdp_ptr->XSDTaddr);
		}
	}
}
