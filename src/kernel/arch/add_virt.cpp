#include <hal/memmap.h>
#include <hal/memtype.h>
RUN_ONCE
void add_virt(hal::memmap *mem) {
	Log(LOG_DEBUG,"[MEMMAP]","adding virtual memory regions");
	hal::mem_type types[6]={};
	types[0].userspace=true;
	types[1].firmware=true;
	types[2].videobuffer=true;
	types[3].heap=true;
	types[4].kthread_stacks=true;
	types[5].paging_struct=true;
	mem->add_regions(6);
	mem->add_region(0xFFFFFF0000000000,0xFFFFFF7FFFFFFFFF,types[5]); //the PML4 510 slot
	mem->add_region(0xFFFFFEFFE0000000,0xFFFFFEFFFFFFFFFF,types[4]); //131072 4kb threads
	mem->add_region(0xFFFFFEDFE0000000,0xFFFFFEFFDFFFFFFF,types[3]); //128GB of heap space
	mem->add_region(0xFFFFFEDFD0000000,0xFFFFFEDFDFFFFFFF,types[2]); //256MB of VRAM
	mem->add_region(0xFFFFFEDFC0000000,0xFFFFFEDFCFFFFFFF,types[1]); //256MB for acpi tables
	mem->add_region(0,(1L<<47)-1,types[0]); //bit 48 is the kerenel/userpace bit
}
static hal::region_hook rhook(hal::virtmem, &add_virt);
