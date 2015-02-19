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
#include <acpi/tables/SDT.h>
#include <acpi/tables/FADT.h>
#include <acpi/tables/MADT.h>
#include <acpi/tables/DSDT.h>
namespace acpi {
	/**
	    @brief initalizes the static table portion of acpi
	    @details finds parses and extracts all tables found in the firmware
	    without executing and aml or enabling acpi mode
		fills out the acpi struct
		@param fail_on_4GB if set, the OSPM will not load the acpi struct,
		 if required to access an address larger than 4GB

	    @date created on 2014-11-03
	*/
	void init_tables(bool fail_on_4GB);

	/**
	 * @brief struct of acpi tables
	 * a struct of acpi tables for import types
	 */
	struct acpi {
		bool available;
		FADT *FADT;
		MADT *MADT;
		DSDT *DSDT;
		SSDT **SSDTs;
		int SSDT_length;
		SDT **SDTs;
		int SDT_length;
	};
	extern acpi acpi;
}
