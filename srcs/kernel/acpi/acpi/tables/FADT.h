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
#include <acpi/address_space.h>
#include <acpi/tables/SDT.h>
#include <acpi/tables/DSDT.h>
#include <acpi/tables/FACS.h>
namespace acpi {
	struct FADT:SDT {
			FACS *firmware_ctrl_addr;
			DSDT *DSDT;
			enum power_profile {
				UNSPECIFIED=0,
				DESKTOP=1,
				MOBILE=2,
				WORKSTATION=3,
				ENTERPRISE_SERVER=4,
				SOHO_SERVER=5,
				APPLIANCE_PC=6,
				PERFORMANCE_SERVER=7,
				TABLET=8,
			} preferred_power_profile;
			uint16_t SCI_interrupt_number;
			uint32_t SMI_command_port;
			uint8_t ACPI_enable_value;
			uint8_t ACPI_disable_value;
			uint8_t S4BIOS_request_value;
			uint8_t processor_state_control_value;
			generic_address_space PM1a_event_block;
			generic_address_space PM1b_event_block;
			generic_address_space PM1a_control_block;
			generic_address_space PM1b_control_block;
			generic_address_space PM2_control_block;
			generic_address_space PM_timer_block;
			generic_address_space GPE0_block;
			generic_address_space GPE1_block;
			uint8_t PM1_event_block_length();
			uint8_t PM1_control_block_length();
			uint8_t PM2_control_block_length();
			uint8_t PM_timer_block_length();
			uint8_t GPE0_block_length();
			uint8_t GPE1_block_length();
			uint8_t GPE1_base;
			uint8_t CST_control_value;
			uint8_t C2_max_latency;
			uint8_t C3_max_latency;
			uint16_t flush_size;
			uint16_t flush_stride;
			uint8_t duty_offset;
			uint8_t duty_width;
			uint8_t day_alarm_index;
			uint8_t month_alarm_index;
			uint8_t century_index;
			bool legacy_devices_present;
			bool PS2_controller_present;
			bool legacy_VGA_not_present;
			bool MSI_not_supported;
			bool no_PCIe_ASPM_controls;
			bool CMOS_not_present;
			bool WBINVD_supported;
			bool WBINVD_flush_all;
			bool C1_supported;
			bool C2_multiprocessor_supported;
			bool power_button_control_device;
			bool sleep_button_control_device;
			bool rtc_wake_fixed_not_supported;
			bool rtc_wake_from_S4;
			bool timer_value_32_bit;
			bool docking_supported;
			bool reset_register_supported;
			bool case_sealed;
			bool headless;
			bool PCIe_wake_enabled;
			bool use_platform_clock;
			bool RTC_STS_valid_from_S4;
			bool remote_power_on_capable;
			bool local_APIC_use_cluster;
			bool local_APIC_physical_dest_mode;
			bool hardware_reduced_ACPI;
			bool low_power_in_S0;
			generic_address_space reset_register;
			uint8_t reset_value;
			generic_address_space sleep_control_register;
			generic_address_space sleep_status_register;
		private:
			void *FADT_firmware_addr;
	};
}
