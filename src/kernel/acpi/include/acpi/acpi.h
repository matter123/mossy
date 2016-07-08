#ifndef ACPI_ACPI_H
#define ACPI_ACPI_H
extern "C" {
#include <acpica/acpi.h>
}

ACPI_TABLE_HEADER *GetACPITable(const char *Signature, int Instance = 1);
#endif
