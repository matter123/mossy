#include <sys/scheduler.h>
#include <sys/sync.h>
#include <memory>
#include <stdlib.h>
semaphore::semaphore(int inital) {
	this->r = inital;
	this->remain = std::addressof(this->r);
}
void semaphore::aquire() {
	while(true) {
		int val = *this->remain;
		if(val && __sync_bool_compare_and_swap(this->remain, val, val - 1)) { return; }
		yield();
	}
}
void semaphore::release() {
	__sync_add_and_fetch(this->remain, 1);
	yield();
}

bool semaphore::check() { return this->r == 0; }
