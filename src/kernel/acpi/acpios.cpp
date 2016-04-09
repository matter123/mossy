#include <acpi/acpi.h>
#include <arch/paging.h>
#include <stdio.h>
#include <cpuid.h>
#include <hal/memmap.h>
#include <sys/spinlock.h>
static uint8_t *bitmap;
static uintptr_t start;
static uintptr_t len;
static uintptr_t page_count;
static bool initalized=false;
#define IS_SET(a,i)    (a[(i)/8]&(1<<(i)%8))
#define SET(a,i)    do {a[(i)/8]|= (1<<((i)%8));}while(0)
#define UN_SET(a,i) do {a[(i)/8]&=~(1<<((i)%8));}while(0)
//GOOD
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
//GOOD
ACPI_STATUS AcpiOsTerminate() {
	return AE_OK;
}
//GOOD
ACPI_PHYSICAL_ADDRESS AcpiOsGetRootPointer() {
	ACPI_PHYSICAL_ADDRESS ret;
	AcpiFindRootPointer(&ret);
	return ret;
}
//GOOD
ACPI_STATUS AcpiOsPredefinedOverride(const ACPI_PREDEFINED_NAMES *PredefinedObject, ACPI_STRING *NewValue) {
	*NewValue=nullptr;
	return AE_OK;
}
//GOOD
ACPI_STATUS AcpiOsTableOverride(ACPI_TABLE_HEADER *ExistingTable, ACPI_TABLE_HEADER **NewTable) {
	*NewTable=nullptr;
	return AE_OK;
}
//GOOD
ACPI_STATUS AcpiOsPhysicalTableOverride(
		ACPI_TABLE_HEADER *ExistingTable,
		ACPI_PHYSICAL_ADDRESS *NewAddress,
		UINT32 *NewTableLength) {
	*NewAddress=(ACPI_PHYSICAL_ADDRESS)NULL;
	*NewTableLength=0;
	return AE_OK;
}
//GOOD
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
//GOOD
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
//GOOD
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
//GOOD
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
//GOOD
ACPI_STATUS AcpiOsGetPhysicalAddress(void *LogicalAddress, ACPI_PHYSICAL_ADDRESS *PhysicalAddress) {
	uintptr_t l=(uintptr_t)LogicalAddress;
	uintptr_t offset=l&0xFFF;
	l&=~0xFFF;
	uintptr_t physical=getphys(l);
	physical+=offset;
	*PhysicalAddress=(ACPI_PHYSICAL_ADDRESS)physical;
	return AE_OK;
}
//GOOD
void *AcpiOsAllocate(ACPI_SIZE size) {
	return malloc((size_t)size);
}
//GOOD
void AcpiOsFree(void *memory) {
	free(memory);
}
//GOOD
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
//HALF STUB
BOOLEAN AcpiOsWritable(void *memory,ACPI_SIZE length) {
	return AcpiOsReadable(memory, length);
}
//STUB
ACPI_THREAD_ID AcpiOsGetThreadId() {
	return 1;
}
//STUB
ACPI_STATUS AcpiOsExecute(ACPI_EXECUTE_TYPE Type, ACPI_OSD_EXEC_CALLBACK Function, void *Context) {
	PANIC("can not execute");
	return AE_OK;
}
//STUB
void AcpiOsWaitEventsComplete() {
}
//STUB
void AcpiOsSleep(UINT64 ms) {
	AcpiOsStall(ms*1000);
}
//HALF STUB
void AcpiOsStall(UINT32 us) {
	while(us--) {
		asm (
		"nop\nnop\nnop\nnop"
		);
	}
}
//GOOD
void AcpiOsPrintf(const char *fmt,...) {
	va_list list;
	va_start(list, fmt);
	AcpiOsVprintf(fmt, list);
	va_end(list);
}
//GOOD
void AcpiOsVprintf(const char *fmt,va_list vlist) {
	Logv(LOG_DEBUG,"[ACPICA]",fmt,vlist);
}
//STUB
ACPI_STATUS AcpiOsCreateSemaphore(UINT32 MaxUnits, UINT32 InitalUnits, ACPI_SEMAPHORE *OutHandle) {
	OutHandle=nullptr;
	return AE_NO_MEMORY;
}
//STUB
ACPI_STATUS AcpiOsDeleteSemaphore(ACPI_SEMAPHORE Handle) {
	return AE_BAD_VALUE;
}
//STUB
ACPI_STATUS AcpiOsWaitSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units, UINT16 Timeout) {
	return AE_BAD_VALUE;
}
//STUB
ACPI_STATUS AcpiOsSignalSemaphore(ACPI_SEMAPHORE Handle, UINT32 Units) {
	return AE_BAD_VALUE;
}
//GOOD
ACPI_STATUS AcpiOsCreateLock(ACPI_SPINLOCK *OutHandle) {
	spinlock *spin=new spinlock();
	*OutHandle=(ACPI_SPINLOCK)spin;
	return AE_OK;
}
//GOOD
void AcpiOsDeleteLock(ACPI_SPINLOCK Handle) {
	delete (spinlock *)Handle;
}
//GOOD
ACPI_CPU_FLAGS AcpiOsAcquireLock(ACPI_SPINLOCK Handle) {
	((spinlock *)Handle)->aquire();
	return 0;
}
//GOOD
void AcpiOsReleaseLock(ACPI_SPINLOCK Handle, ACPI_CPU_FLAGS Flags) {
	((spinlock *)Handle)->release();
}
//STUB
ACPI_STATUS AcpiOsInstallInterruptHandler(UINT32 InterruptLevel, ACPI_OSD_HANDLER Handler, void *Context) {
	return AE_BAD_VALUE;
}
//STUB
ACPI_STATUS AcpiOsRemoveInterruptHandler(UINT32 InterruptLevel, ACPI_OSD_HANDLER Handler) {
	return AE_BAD_VALUE;
}
//GOOD
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
//GOOD
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
//STUB
ACPI_STATUS AcpiOsReadPciConfiguration(ACPI_PCI_ID *PciID, UINT32 Register, UINT64 *Value, UINT32 Width) {
	return AE_ACCESS;
}
//STUB
ACPI_STATUS AcpiOsWritePciConfiguration(ACPI_PCI_ID *PciID, UINT32 Register, UINT64 Value, UINT32 Width) {
	return AE_ACCESS;
}
//GOOD
ACPI_STATUS AcpiOsSignal(UINT32 Function, void *Info) {
	if(Function==ACPI_SIGNAL_FATAL) {
		ACPI_SIGNAL_FATAL_INFO *info=(ACPI_SIGNAL_FATAL_INFO *)Info;
		Log(LOG_ERROR, "[ACPICA]", "Fatal Signal: type %d, code %d, argument %d",info->Type,info->Code,info->Argument);
	} else if(Function==ACPI_SIGNAL_BREAKPOINT) {
		Log(LOG_DEBUG, "[ACPICA]", "Breakpoint Signal: %s",(char *)Info);
	}
	return AE_OK;
}
//STUB
UINT64 AcpiOsGetTimer() {
	return 0;
}
