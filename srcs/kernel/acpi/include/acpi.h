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
namespace acpi {
	/**
	    @brief initalizes the static table portion of acpi
	    @details finds parses and extracts all tables found in the firmware
	    without executing and aml or enabling acpi mode

	    @date created on 2014-11-03
	*/
	void init_tables();

	/**
	 * @brief determines if acpi exists
	 * @details should be called after \a init_tables and before
	 * attempting to use any acpi feature
	 *
	 * @return true is acpi support was found
	 * @date created on 2014-11-19
	 */
	bool has_acpi();
}