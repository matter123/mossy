#include <sys/threadstacks.h>
#include <hal/memmap.h>
#include <hal/commandline.h>
#include <numconv.h>
size_t stack_size = 0;

void thread_stack_size_init() {
	stack_size=std::strtonum(get_arg("StackSize"), 1);
	if(stack_size&(stack_size-1)) {
		Log(LOG_ERROR, "[THREAD]", "Stack size is not a POT");
	}
	Log(LOG_INFO, "[THREAD]", "hey");
	stack_size*=0x1000;
}

static size_t total_stacks;
static uintptr_t stacks_start;
static thread_info **stack_stack;
static size_t free_count=0;
void thread_stacks_init() {
	hal::mem_region *reg=nullptr;
	find_memregion(reg,hal::virtmem,reg->type.kthread_stacks,"[THREAD]");
	uint64_t total_length=(reg->end-reg->start)+1; //end will be 0xFFF, add one so we get mod%0x1000 == 0
	total_stacks=total_length/stack_size;
	size_t reserve_space=total_stacks*sizeof(thread_info *);
	reserve_space=(reserve_space|(stack_size-1))+1;
	total_stacks-=(reserve_space/stack_size);
	stacks_start=uintptr_t(reg->start+reserve_space);
	stack_stack=(thread_info **)reg->start;
	for(size_t s=0;s<total_stacks;s++) {
		*stack_stack=(thread_info *)(stacks_start+s*stack_size);
		stack_stack++;
		free_count++;
	}
	Log(LOG_INFO,"[THREAD]","%d stacks(using %#X sized stacks)",free_count,stack_size);
}
thread_info *get_next_stack() {
	if(free_count) {
		stack_stack--;
		free_count--;
		return *stack_stack;
	}
	return nullptr;
}

void free_stack(thread_info *stack) {
	*stack_stack=stack;
	stack_stack++;
	free_count++;
}
