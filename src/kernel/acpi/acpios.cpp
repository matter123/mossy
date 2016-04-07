#include <acpi/acpi.h>
#include <arch/paging.h>
#include <stdio.h>

ACPI_STATUS AcpiOsInitialize() {
	return AE_OK;
}

ACPI_STATUS AcpiOsTerminate() {
	return AE_OK;
}

ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer() {
	ACPI_PHYSICAL_ADDRESS ret;
	AcpiFindRootPointer(&ret);
	return ret;
}

ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES *PredefinedObject, ACPI_STRING *NewValue) {
	*NewValue=nullptr;
	return AE_OK;
}

ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_TABLE_HEADER **NewTable) {
	*NewTable=nullptr;
	return AE_OK;
}

void *AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS PhysicalAddress, ACPI_SIZE Length) {
	PANIC("map memory not implemented");
	return nullptr;
}

void AcpiOsUnmapMemory(void *where, ACPI_SIZE length) {
	return;
}

ACPI_STATUS AcpiOsGetPhysicalAddress(void *LogicalAddress, ACPI_PHYSICAL_ADDRESS *PhysicalAddress) {
	uintptr_t l=(uintptr_t)LogicalAddress;
	uintptr_t offset=l&0xFFF;
	l&=~0xFFF;
	uintptr_t physical=getphys(l);
	physical+=offset;
	*PhysicalAddress=(ACPI_PHYSICAL_ADDRESS)physical;
	return AE_OK;
}

void *AcpiOsAllocate(ACPI_SIZE size) {
	return malloc((size_t)size);
}

void AcpiOsFree(void *memory) {
	free(memory);
}

BOOLEAN AcpiOsReadable(void *memory,ACPI_SIZE length) {
	size_t len=(length|0xFFF)+1;
	uintptr_t mem=(uintptr_t)memory&~0xFFF;
	while(len>0) {
		if(!ismapped(mem)) {
			return false;
		}
		mem+=0x1000;
		len-=0x1000;
	}
	return true;
}

BOOLEAN AcpiOsWritable(void *memory,ACPI_SIZE length) {
	return AcpiOsReadable(memory, length);
}

ACPI_THREAD_ID AcpiOsGetThreadId() {
	return 1;
}

ACPI_STATUS AcpiOsExecute(ACPI_EXECUTE_TYPE Type, ACPI_OSD_EXEC_CALLBACK Function, void *Context) {
	PANIC("can not execute");
	return AE_OK;
}

void AcpiOsSleep(UINT64 ms) {
	AcpiOsStall(ms*1000);
}

//stall some amount of time
void AcpiOsStall(UINT32 us) {
	while(us--) {
		asm (
		"nop\nnop\nnop\nnop"
		);
	}
}

void AcpiOsPrintf(const char *fmt,...) {
	Log(LOG_INFO,"[ACPICA]",fmt);
}

void AcpiOsVprintf(const char *fmt,va_list vlist) {
	Log(LOG_INFO,"[ACPICA]",fmt);
}
