#include <hal/memmap.h>
#include <hal/memtype.h>
void add_virt(hal::memmap *mem) {
	Log(LOG_DEBUG,"[MEMMAP]","adding virtual memory regions");
	hal::mem_type types[5]={};
	types[0].userspace=true;
	types[1].videobuffer=true;
	types[2].heap=true;
	types[3].kthread_stacks=true;
	types[4].paging_struct=true;
	mem->add_regions(5);
	mem->add_region(0xFFFFFF0000000000,0xFFFFFF7FFFFFFFFF,types[4]); //the PML4 510 slot
	mem->add_region(0xFFFFFEFFE0000000,0xFFFFFEFFFFFFFFFF,types[3]); //131072 4kb threads
	mem->add_region(0xFFFFFEDFE0000000,0xFFFFFEFFDFFFFFFF,types[2]); //128GB of heap space
	mem->add_region(0xFFFFFEDFD0000000,0xFFFFFEDFDFFFFFFF,types[1]); //256MB of VRAM
	mem->add_region(0,(1L<<47)-1,types[0]); //bit 48 is the kerenel/userpace bit
}
hal::region_hook rhook(hal::virtmem, &add_virt);
