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
#include <tables/sdt.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <acpi_os.h>
namespace acpi {
	namespace tables {
		RSDT *rsdt;
		XSDT *xsdt;

		SDT *load_table(void * tbl_phys_base) {
			uintptr_t alloc_size=0;
			SDT *sdt=(SDT *)os::get_virt_phys((uintptr_t)tbl_phys_base,sizeof(SDT),&alloc_size);
			int needed_length=sdt->length-sizeof(SDT);
			if(needed_length>alloc_size) {
				os::unget_phys((uintptr_t)tbl_phys_base,sizeof(SDT),(uintptr_t)sdt);
				sdt=(SDT *)os::get_virt_phys((uintptr_t)tbl_phys_base,sizeof(SDT)+needed_length,NULL);
			}
			if(!do_checksum(sdt)) {
				return NULL;
			}
			return sdt;
		}

		bool do_checksum(SDT * table) {
			pointer bytes=reinterpret_cast<pointer>(table);
			uint8_t checksum=0;
			for(int i=0; i<table->length; i++) {
				checksum+=bytes[i];
			}
			return !checksum;
		}
	}
}