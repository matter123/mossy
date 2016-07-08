#include <acpi/acpi.h>
ACPI_TABLE_HEADER *GetACPITable(const char *Signature, int Instance) {
	ACPI_TABLE_HEADER *Table;
	ACPI_STATUS status = AcpiGetTable(const_cast<char *>(Signature), Instance, &Table);
	if(status == AE_OK) {
		return Table;
	} else if(status == AE_BAD_PARAMETER) {
		Log(LOG_WARNING, "[ACPI  ]", "GetACPITable: Signature is NULL");
	} else if(AE_NO_ACPI_TABLES) {
		Log(LOG_ERROR, "[ACPI  ]", "Could not find a RSDP");
	} else if(AE_NOT_FOUND) {
		Log(LOG_WARNING, "[ACPI  ]", "GetACPITable: %s is not loaded");
	} else if(AE_NO_MEMORY) {
		Log(LOG_WARNING, "[ACPI  ]", "no memory to load table");
	}
	return nullptr;
}
