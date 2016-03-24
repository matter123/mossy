#include <arch/paging.h>
#include <arch/recursivepaging.h>
#include <arch/int.h>
#include <sys/pfa.h>
recursive_paging rp;
static void paging_handler(cpu_state *s,void *sse,bool *in_use);
static void pre_paging_handler(cpu_state *s,void *sse,bool *in_use) {
	Log(LOG_DEBUG,"[PAGING]","pre paging handler called");
	paging_init();
}
void pre_paging_init() {
	rp.init(510);
	install_single_interrupt(0xE, &pre_paging_handler);
	Log(LOG_INFO,"[PAGING]","pre paging initalized");
}

static bool initalized=false;
void paging_init() {
	if(initalized)return;
	initalized=true;
	install_single_interrupt(0xE, &paging_handler);
	Log(LOG_INFO,"[PAGING]","paging initalized");
}
static void paging_handler(cpu_state *s,void *sse,bool *in_use) {
	Log(LOG_DEBUG,"[PAGING]","page fault virt addr:%.16p rip:%.16p",s->cr2,s->rip);
	rp.map(get_page(),s->cr2,PAGE_WRITE|PAGE_EXEC);
	while(true);
}
void map(uintptr_t phys, uintptr_t virt, uint flags) {
	rp.map(phys, virt, flags);
}
bool ismapped(uintptr_t virt) {
	return rp.locate(virt)!=0x1;
}
uintptr_t getphys(uintptr_t virt) {
	return rp.locate(virt);
}
