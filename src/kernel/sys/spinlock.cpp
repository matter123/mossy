#include <sys/spinlock.h>
#include <stdlib.h>
spinlock::spinlock() {
	lock=(int volatile *)malloc(sizeof(int));
}
void spinlock::aquire() {
	while(!__sync_bool_compare_and_swap(this->lock,0,1)) {
		 asm volatile ("pause");
	}
}
void spinlock::release() {
	__sync_lock_release(this->lock);
}

bool spinlock::check() {
	return *lock!=0;
}
