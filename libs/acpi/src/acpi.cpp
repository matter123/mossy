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
#include <acpi_os.h>
#include <tables/rsdp.h>
#include <tables/sdt.h>
namespace acpi {
	static bool h_acpi=false;
	void init_tables() {
		os::init_acpi_os();
		tables::find_rsdp();
		if(tables::rsdp_ptr) {
			h_acpi=true;
		} else {
			return;
		}
		if(tables::rsdp_ptr->begin.revision==0) {
			tables::rsdt=(tables::RSDT *)tables::load_table(
			                 (void *)tables::rsdp_ptr->begin.RSDTaddr);
		} else {
			tables::xsdt=(tables::XSDT *)tables::load_table(
			                 (void *)tables::rsdp_ptr->XSDTaddr);
		}
	}

	bool has_acpi() {
		return h_acpi;
	}
}