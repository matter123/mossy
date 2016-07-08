#include <sys/scheduler.h>
#include <sys/sync.h>
#include <memory>
#include <stdlib.h>
remutex::remutex() {
	c = 0;
	t = 0;
	this->count = std::addressof(c);
	this->thread_id = std::addressof(t);
}
void remutex::aquire() {
	uint32_t thread = get_current_thread_info()->thread_id;
	while(true) {
		if(__sync_bool_compare_and_swap(this->thread_id, 0, thread) ||
		   __sync_bool_compare_and_swap(this->thread_id, thread, thread)) {
			__sync_add_and_fetch(this->count, 1);
			return;
		}
		yield();
	}
}
void remutex::release() {
	if(!__sync_sub_and_fetch(this->count, 1)) {
		__sync_lock_release(this->thread_id);
		yield();
	}
}

bool remutex::check() {
	// we can always lock if we own the mutex
	return (!*this->thread_id) || (*this->thread_id == get_current_thread_info()->thread_id);
}
