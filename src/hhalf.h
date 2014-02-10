#ifndef __HIGH_HALF_H
#define __HIGH_HALF_H
#include <stdint.h>

#define HIGH_HALF_BASE_ADDR 0xC0000000

void init_higher_half();

void *em_page_max();
void em_page(void *virt,void * phys);

#endif
