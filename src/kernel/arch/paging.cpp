#include <sys/pfa.h>
#include <arch/int.h>
#include <arch/paging.h>
#include <arch/recursivepaging.h>
recursive_paging rp;
static void paging_handler(cpu_state *s);
static void pre_paging_handler(cpu_state *s) {
	Log(LOG_DEBUG, "[PAGING]", "pre paging handler called");
	paging_init();
}
void pre_paging_init() {
	rp.init(510);
	install_exception(0xE, &pre_paging_handler);
	Log(LOG_INFO, "[PAGING]", "pre paging initalized");
}

static bool initalized = false;
void paging_init() {
	if(initalized)
		return;
	initalized = true;
	install_exception(0xE, &paging_handler);
	Log(LOG_INFO, "[PAGING]", "paging initalized");
}
extern "C" uintptr_t zero_page;
static void paging_handler(cpu_state *s) {
	rp.map(get_page(), s->cr2, PAGE_WRITE);
	return;
	pte *PT = rp.page_info(s->cr2);
	// previously reserved page
	if(!PT->read_write && PT->flags == 1) {
		uintptr_t page = get_page();
		Log(LOG_DEBUG, "[PAGING]", "filling reserve page %.16p", page);
		rp.map(page, s->cr2, PAGE_WRITE);
	} else if(s->cr2 >> 63) {
		// if the last bit is set the fault was in kernel space, assume i'm lazy (malloc)
		// check if it was a write and would have refaulted if a read only mapped
		if(s->err_code & 2) {
			rp.map(get_page(), s->cr2, PAGE_WRITE);
		} else {
			Log(LOG_DEBUG, "[PAGING]", "reserving page");
			// reserve a page by mapping the zero page
			rp.map((uintptr_t)&zero_page, s->cr2, 0);
			PT->flags = 1;
		}
	} else {
		// this is a userspace page fault
		if((s->cr2 & ~0xFFF) == 0) {
			// null pointer
			PANIC("null pointer accsess");
		}
		// assume broken code
		PANIC("not sure what to do atm");
	}
}
void map(uintptr_t phys, uintptr_t virt, uint flags) {
	rp.map(phys, virt, flags);
}
uintptr_t unmap(uintptr_t virt) {
	return rp.unmap(virt);
}
bool ismapped(uintptr_t virt) {
	return rp.locate(virt) != 0x1;
}
uintptr_t getphys(uintptr_t virt) {
	return rp.locate(virt);
}
