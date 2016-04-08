#include <arch/recursivepaging.h>
#include <arch/paging.h>
#include <sys/pfa.h>
#include <string.h>
struct pte {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool dirty :1;
	bool pat :1;
	bool global :1;
	int unused :3;
	uint64_t addr :40;
	int unused_1 :10;
	bool nx :1;
} PACKED;
struct pde {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool ign :1;
	int zero :2;
	int unused :3;
	uint64_t addr :40;
	int unused_1 :10;
	bool nx :1;
} PACKED;
struct pdpe {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool ign :1;
	int zero :2;
	int unused :3;
	uint64_t addr :40;
	int unused_1 :10;
	bool nx :1;
} PACKED;
struct pml4e {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool ign :1;
	int zero :2;
	int unused :3;
	uint64_t addr :40;
	int unused_1 :10;
	bool nx :1;
} PACKED;
union pt {
	pte entries[512];
	uint64_t entriesi[512];
} PACKED;
union pd {
	pde entries[512];
	uint64_t entriesi[512];
} PACKED;
union pdp {
	pdpe entries[512];
	uint64_t entriesi[512];
} PACKED;
union pml4 {
	pml4e entries[512];
	uint64_t entriesi[512];
} PACKED;
void invlpage(uintptr_t addr) {
		asm volatile("invlpg (%0)" ::"r"(addr) : "memory");
	}
void recursive_paging::init(uint idx) {
	this->base=PML4_OFF(idx);
	this->index=idx;
		Log(LOG_DEBUG,"[PAGING]","base addr: %.16p index: %d",this->base,this->index);
}
void recursive_paging::map(uintptr_t phys,uintptr_t virt,uint flags) {
	virt&=~0xFFF;
	phys&=~0xFFF;
	uint pml4i=(virt>>39)&0x1FF;
	uint pdpi=(virt>>30)&0x1FF;
	uint pdi=(virt>>21)&0x1FF;
	uint pti=(virt>>12)&0x1FF;
	if(pml4i==this->index) {
		Log(LOG_WARNING, "[PAGING]", "page addr %.16p",virt);
		Log(LOG_ERROR,"[PAGING]","map called in internal paging address");
	}
	pml4 *PML4 = (pml4 *)(this->base+PDP_OFF(this->index)+PD_OFF(this->index)+PT_OFF(this->index));
	pdp  *PDP  = (pdp  *)(this->base+PDP_OFF(this->index)+PD_OFF(this->index)+PT_OFF(pml4i));
	pd   *PD   = (pd   *)(this->base+PDP_OFF(this->index)+PD_OFF(pml4i)+PT_OFF(pdpi));
	pt   *PT   = (pt   *)(this->base+PDP_OFF(pml4i)+PD_OFF(pdpi)+PT_OFF(pdi));
	if(!PML4->entries[pml4i].present) {
		uint64_t address=get_page()&0x0000FFFFFFFFF000;
		PML4->entriesi[pml4i]=address|PAGE_WRITE|PAGE_USER|0x1;
		invlpage((uintptr_t)PDP);
		memset((void *)PDP,0,0x1000);
	}
	if(!PDP->entries[pdpi].present) {
		uint64_t address=get_page()&0x0000FFFFFFFFF000;
		PDP->entriesi[pdpi]=address|PAGE_WRITE|PAGE_USER|0x1;
		invlpage((uintptr_t)PD);
		memset(PD,0,0x1000);
	}
	if(!PD->entries[pdi].present) {
		uint64_t address=get_page()&0x0000FFFFFFFFF000;
		PD->entriesi[pdi]=address|PAGE_WRITE|PAGE_USER|0x1;
		invlpage((uintptr_t)PT);
		memset(PT,0,0x1000);
	}
	bool exec=flags&0x1;
	flags&=~0x1;
	PT->entriesi[pti]=(phys&0x0000FFFFFFFFF000)|flags|0x1;
	PT->entries[pti].nx=!exec;
	invlpage((uintptr_t)virt);
}

uintptr_t recursive_paging::locate(uintptr_t virt) {
	virt&=~0xFFF;
	uint pml4i=(virt>>39)&0x1FF;
	uint pdpi=(virt>>30)&0x1FF;
	uint pdi=(virt>>21)&0x1FF;
	uint pti=(virt>>12)&0x1FF;
	pml4 *PML4 = (pml4 *)(this->base+PDP_OFF(this->index)+PD_OFF(this->index)+PT_OFF(this->index));
	pdp  *PDP  = (pdp  *)(this->base+PDP_OFF(this->index)+PD_OFF(this->index)+PT_OFF(pml4i));
	pd   *PD   = (pd   *)(this->base+PDP_OFF(this->index)+PD_OFF(pml4i)+PT_OFF(pdpi));
	pt   *PT   = (pt   *)(this->base+PDP_OFF(pml4i)+PD_OFF(pdpi)+PT_OFF(pdi));
	if(!PML4->entries[pml4i].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PML4:%.16p",PML4);
	if(!PDP->entries[pdpi].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PDP: %.16p",PDP);
	if(!PD->entries[pdi].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PD:  %.16p",PD);
	if(!PT->entries[pti].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PT:  %.16p",PT);
	return PT->entriesi[pti]&0x0000FFFFFFFFF000;
}

uintptr_t recursive_paging::unmap(uintptr_t virt) {
	virt&=~0xFFF;
	uint pml4i=(virt>>39)&0x1FF;
	uint pdpi=(virt>>30)&0x1FF;
	uint pdi=(virt>>21)&0x1FF;
	uint pti=(virt>>12)&0x1FF;
	pml4 *PML4 = (pml4 *)(this->base+PDP_OFF(this->index)+PD_OFF(this->index)+PT_OFF(this->index));
	pdp  *PDP  = (pdp  *)(this->base+PDP_OFF(this->index)+PD_OFF(this->index)+PT_OFF(pml4i));
	pd   *PD   = (pd   *)(this->base+PDP_OFF(this->index)+PD_OFF(pml4i)+PT_OFF(pdpi));
	pt   *PT   = (pt   *)(this->base+PDP_OFF(pml4i)+PD_OFF(pdpi)+PT_OFF(pdi));
	if(!PML4->entries[pml4i].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PML4:%.16p",PML4);
	if(!PDP->entries[pdpi].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PDP: %.16p",PDP);
	if(!PD->entries[pdi].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PD:  %.16p",PD);
	if(!PT->entries[pti].present)return 0x1;
	//Log(LOG_DEBUG,"[PAGING]","L PT:  %.16p",PT);
	PT->entries[pti].present=false;
	uintptr_t addr=PT->entriesi[pti]&0x0000FFFFFFFFF000;
	invlpage((uintptr_t)virt);
	for(int i=0;i<512;i++) {
		if(PT->entries[i].present) {
			return addr;
		}
	}
	//dealloc PT
	PD->entries[pdi].present=false;
	add_free_page(PD->entriesi[pdi]&0x0000FFFFFFFFF000);
	invlpage((uintptr_t)PT);
	for(int i=0;i<512;i++) {
		if(PD->entries[i].present) {
			return addr;
		}
	}
	//dealloc PD
	PDP->entries[pdpi].present=false;
	add_free_page(PDP->entriesi[pdpi]&0x0000FFFFFFFFF000);
	invlpage((uintptr_t)PD);
	for(int i=0;i<512;i++) {
		if(PDP->entries[i].present) {
			return addr;
		}
	}
	//dealloc PDP
	PML4->entries[pml4i].present=false;
	add_free_page(PML4->entriesi[pml4i]&0x0000FFFFFFFFF000);
	invlpage((uintptr_t)PDP);
	return addr;
}
