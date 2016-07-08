#include <sys/pfa.h>
#include <sys/threadstacks.h>
#include <hal/commandline.h>
#include <hal/memmap.h>
#include <arch/paging.h>
#include <numconv.h>
#include <string.h>
size_t stack_size = 0x1000;
size_t s_size = 0; // holds the actual stack size

void thread_stack_size_init() {
	s_size = std::strtonum(get_arg("StackSize"), 1);
	if(s_size & (s_size - 1)) { Log(LOG_ERROR, "[THREAD]", "Stack size is not a POT"); }
	s_size *= 0x1000;
}

static size_t total_stacks;
static uintptr_t stacks_start;
static thread_info **stack_stack;
static size_t free_count = 0;
void thread_stacks_init() {
	hal::mem_region *reg = nullptr;
	find_memregion(reg, hal::virtmem, reg->type.kthread_stacks, "[THREAD]");
	uint64_t total_length = (reg->end - reg->start) + 1; // end will be 0xFFF, add one so we get mod%0x1000 == 0
	total_stacks = total_length / s_size;
	size_t reserve_space = total_stacks * sizeof(thread_info *);
	reserve_space = (reserve_space | (s_size - 1)) + 1;
	total_stacks -= (reserve_space / s_size);
	stacks_start = uintptr_t(reg->start + reserve_space);
	stack_stack = (thread_info **)reg->start;
	for(size_t s = 0; s < total_stacks; s++) {
		*stack_stack = (thread_info *)(stacks_start + s * s_size);
		stack_stack++;
		free_count++;
	}
	stack_size = s_size; // copy temporary into normal
	Log(LOG_INFO, "[THREAD]", "%d stacks(using %#X sized stacks)", free_count, stack_size);
}
thread_info *get_next_stack() {
	if(free_count) {
		stack_stack--;
		free_count--;
		thread_info *info = *stack_stack;
		for(size_t s = 0; s < stack_size; s += 0x1000) {
			map(get_page(), ((uintptr_t)info) + s, PAGE_WRITE);
			memset((void *)(((uintptr_t)info) + s), 0, 0x1000);
		}
		return info;
	}
	Log(LOG_ERROR, "[THREAD]", "ran out of stacks");
	return nullptr;
}

void free_stack(thread_info *stack) {
	*stack_stack = stack;
	stack_stack++;
	free_count++;
}
