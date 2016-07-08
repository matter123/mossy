#include <sys/sync.h>
#include <memory>
#include <stdlib.h>
spinlock::spinlock() {
	this->l = 0;
	this->lock = std::addressof(this->l);
}
void spinlock::aquire() {
	while(!__sync_bool_compare_and_swap(this->lock, 0, 1)) {
		// P6 can accidently mark as memory error and slow down, adding pause fixes this
		asm volatile("pause");
	}
}
void spinlock::release() { __sync_lock_release(this->lock); }

bool spinlock::check() { return this->l == 0; }
