#include <acpi/acpi.h>
#include <arch/paging.h>
#include <stdio.h>
#include <cpuid.h>
#include <hal/memmap.h>
static uint8_t *bitmap;
static uintptr_t start;
static uintptr_t len;
static uintptr_t page_count;
static bool initalized=false;
#define IS_SET(a,i)    (a[(i)/8]&(1<<(i)%8))
#define SET(a,i)    do {a[(i)/8]|= (1<<((i)%8));}while(0)
#define UN_SET(a,i) do {a[(i)/8]&=~(1<<((i)%8));}while(0)
ACPI_STATUS AcpiOsInitialize() {
	if(initalized) {
		return AE_OK;
	}
	hal::mem_region *reg=nullptr;
	for(int i=0;i<hal::virtmem.region_count();i++) {
		reg=hal::virtmem.get_region(i);
		if(reg->type.firmware) {
			break;
		}
	}
	if(!reg||!reg->type.firmware) {
		Log(LOG_ERROR,"[ACPIOS]","no firmware region was found");
		return AE_NO_MEMORY;
	}
	start=reg->start;
	len=reg->end-start;
	uintptr_t bitmap_len=len/0x1000/8;
	bitmap=(uint8_t *)start;
	start+=bitmap_len;
	start=(start|0xFFF)+1;
	len=reg->end-start;
	page_count=len/0x1000;
	Log(LOG_DEBUG,"[ACPIOS]","sucessfully initalized");
	initalized=true;
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

ACPI_STATUS AcpiOsPhysicalTableOverride(
		ACPI_TABLE_HEADER *ExistingTable,
		ACPI_PHYSICAL_ADDRESS *NewAddress,
		UINT32 *NewTableLength) {
	NewAddress=nullptr;
	return AE_OK;
}
void *AcpiOsMapMemory(ACPI_PHYSICAL_ADDRESS phys, ACPI_SIZE length) {
	uintptr_t pa_phys=phys&~(0xFFF);
	uintptr_t pa_len=(length+(0x1000-length%0x1000))/0x1000+(phys%0x1000+length%0x1000)/0x1000;
	for(size_t s=0; s<page_count-(pa_len-1); s++) {
		for(size_t i=0; i<pa_len; i++) {
			if(IS_SET(bitmap,s+i)) {
				goto skip;
			}
		}
		for(size_t i=0; i<pa_len; i++) {
			SET(bitmap,s+i);
			map(pa_phys+i*0x1000, start+(s+i)*0x1000, PAGE_WRITE);
		}
		Log(LOG_DEBUG,"[ACPIOS]","mapping %.16p to %.16p",phys,((phys&0xFFF)+start+s*0x1000));
		return (void *)((phys&0xFFF)+start+s*0x1000);
		skip:;
	}
	return nullptr;
}

void AcpiOsUnmapMemory(void *where, ACPI_SIZE length) {
	uintptr_t virt=(uintptr_t)where;
	virt&=~0xFFF;
	size_t s=(virt-start)/0x1000;
	uintptr_t pa_len=(length+(0x1000-length%0x1000))/0x1000+(((uintptr_t)where)%0x1000+length%0x1000)/0x1000;
	for(size_t i=0; i<pa_len; i++) {
		unmap(virt);
		UN_SET(bitmap,s+i);
	}
}

ACPI_STATUS AcpiOsReadMemory(ACPI_PHYSICAL_ADDRESS Address,UINT64 *Value, UINT32 Width) {
	void *addr=AcpiOsMapMemory(Address, Width/8);
	switch(Width) {
	case 8:
		uint8_t *a;
		uint16_t *b;
		uint32_t *c;
		uint64_t *d;
		a=(uint8_t *)addr;
		*Value=*a;
		break;
	case 16:
		b=(uint16_t *)addr;
		*Value=*b;
		break;
	case 32:
		c=(uint32_t *)addr;
		*Value=*c;
		break;
	case 64:
		d=(uint64_t *)addr;
		*Value=*d;
		break;
	}
	AcpiOsUnmapMemory(addr, Width/8);
	return AE_OK;
}

ACPI_STATUS AcpiOsWriteMemory(ACPI_PHYSICAL_ADDRESS Address,UINT64 Value, UINT32 Width) {
	void *addr=AcpiOsMapMemory(Address, Width/8);
	switch(Width) {
	case 8:
		uint8_t *a;
		uint16_t *b;
		uint32_t *c;
		uint64_t *d;
		a=(uint8_t *)addr;
		*a=(uint8_t)Value&UINT8_MAX;
		break;
	case 16:
		b=(uint16_t *)addr;
		*b=(uint16_t)Value&UINT16_MAX;
		break;
	case 32:
		c=(uint32_t *)addr;
		*c=(uint32_t)Value&UINT32_MAX;
		break;
	case 64:
		d=(uint64_t *)addr;
		*d=Value;
		break;
	}
	AcpiOsUnmapMemory(addr, Width/8);
	return AE_OK;
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
	size_t Len=(length|0xFFF)+1;
	uintptr_t mem=(uintptr_t)memory&~0xFFF;
	while(Len>0) {
		if(!ismapped(mem)) {
			return false;
		}
		mem+=0x1000;
		Len-=0x1000;
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

void AcpiOsWaitEventsComplete() {
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

ACPI_STATUS AcpiOsCreateSemaphore(UINT32 MaxUnits, UINT32 InitalUnits, ACPI_SEMAPHORE *OutHandle) {
	OutHandle=nullptr;
	return AE_NO_MEMORY;
}

ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE Handle) {
	return AE_BAD_VALUE;
}

ACPI_STATUS AcpiOsWaitSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units, UINT16 Timeout) {
	return AE_BAD_VALUE;
}
ACPI_STATUS AcpiOsSignalSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units) {
	return AE_BAD_VALUE;
}

ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK *OutHandle) {
	OutHandle=nullptr;
	return AE_NO_MEMORY;
}

void AcpiOsDeleteLock(ACPI_SPINLOCK Handle) {
}

ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK Handle) {
	return 0;
}

void AcpiOsReleaseLock(ACPI_SPINLOCK Handle, ACPI_CPU_FLAGS Flags) {
}

ACPI_STATUS AcpiOsInstallInterruptHandler(UINT32 InterruptLevel, ACPI_OSD_HANDLER Handler, void *Context) {
	return AE_BAD_VALUE;
}

ACPI_STATUS AcpiOsRemoveInterruptHandler(UINT32 InterruptLevel, ACPI_OSD_HANDLER Handler) {
	return AE_BAD_VALUE;
}

ACPI_STATUS AcpiOsWritePort(ACPI_IO_ADDRESS Address,UINT32 Value, UINT32 Width) {
	switch(Width) {
	case 8:
		outb(Address, (uint8_t)Value&UINT8_MAX);
		break;
	case 16:
		outw(Address, (uint16_t)Value&UINT16_MAX);
		break;
	case 32:
		outd(Address, (uint32_t)Value&UINT32_MAX);
		break;
	}
	return AE_OK;
}

ACPI_STATUS AcpiOsReadPort(ACPI_IO_ADDRESS Address,UINT32 *Value, UINT32 Width) {
	switch(Width) {
	case 8:
		*Value=inb(Address);
		break;
	case 16:
		*Value=inw(Address);
		break;
	case 32:
		*Value=ind(Address);
		break;
	}
	return AE_OK;
}

ACPI_STATUS AcpiOsReadPciConfiguration(ACPI_PCI_ID *PciID, UINT32 Register, UINT64 *Value, UINT32 Width) {
	return AE_ACCESS;
}

ACPI_STATUS AcpiOsWritePciConfiguration(ACPI_PCI_ID *PciID, UINT32 Register, UINT64 Value, UINT32 Width) {
	return AE_ACCESS;
}

ACPI_STATUS AcpiOsSignal(UINT32 Function, void *Info) {
	return AE_OK;
}

UINT64 AcpiOsGetTimer() {
	return 0;
}
