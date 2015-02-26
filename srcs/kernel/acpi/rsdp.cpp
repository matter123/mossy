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

#include <acpi/rsdp.h>
#include <acpi/acpi_os.h>
#include <string.h>
#include <stdlib.h>
namespace acpi {
	static bool do_checksum(RSDP20 *rsdp_ptr) {
		pointer bytes=reinterpret_cast<pointer>(rsdp_ptr);
		uint8_t checksum=0;
		for(int i=0; i<sizeof(RSDP); i++) {
			checksum+=bytes[i];
		}
		if(checksum!=0) {
			return false;
		}
		if(rsdp_ptr->begin.revision==0) {
			return true;
		}
		//extended (ACPI v2) checksum
		for(int i=0; i<sizeof(RSDP20); i++) {
			checksum+=bytes[i];
		}
		return checksum==0;
	}
	RSDP20 *find_rsdp() {
		/**@note BIOS only*/
		RSDP20 *rsdp_ptr=NULL;
		uintptr_t bda=os::get_virt_phys(0x040E, 2,NULL);
		uintptr_t ebda_start=*(reinterpret_cast<uint16_t *>(bda))<<4;
		os::unget_phys(0x040E,2,bda);
		uintptr_t rsdp_start1=os::get_virt_phys(ebda_start,0x1000,NULL)&~(0xF);
		uintptr_t rsdp_start2=os::get_virt_phys(0xE0000,0x20000,NULL)&~(0xF);
		for(uintptr_t i=0; i<0x20000; i+=16) {
			//check both spots
			if(memcmp((void *)(rsdp_start2+i),"RSD PTR ",8)==0) {
				rsdp_ptr=reinterpret_cast<RSDP20 *>(rsdp_start2+i);
				break;
			}
			if(i<0x1000&&memcmp((void *)(rsdp_start1+i),"RSD PTR ",8)==0) {
				rsdp_ptr=reinterpret_cast<RSDP20 *>(rsdp_start1+i);
				break;
			}
		}
		if(!rsdp_ptr) {
			return NULL;
		}
		if(!do_checksum(rsdp_ptr)) {
			return NULL;
		}
		void *rsdp=malloc(sizeof(RSDP20));
		//memcpy the right amount of memory
		memcpy(rsdp,rsdp_ptr,rsdp_ptr->begin.revision==0?sizeof(RSDP):sizeof(RSDP20));
		rsdp_ptr=reinterpret_cast<RSDP20 *>(rsdp);
		//and free the firmware
		os::unget_phys(ebda_start,0x1000,rsdp_start1);
		os::unget_phys(0xE0000,0x20000,rsdp_start2);
		return rsdp_ptr;
	}
}
