#include <sys/malloc.h>
#include <hal/memmap.h>
#include <string.h>
struct malloc_header {
	uint32_t       magic:24;
	uint8_t        inuse;
	uint32_t       size;
	malloc_header *linear_prev;
	malloc_header *linear_next;
	malloc_header *sorted_next;
};
static_assert(sizeof(malloc_header)%16==0,"malloc_header is not 16 byte aligned");

const uint32_t min_alloc_size=16; //must be a POT, must be 16 byte aligned
const uint32_t min_total_size=sizeof(malloc_header)+min_alloc_size;
#define alloc_offset(x) (sizeof(malloc_header)+(x))

malloc_header *head=nullptr;

void sorted_remove(malloc_header *cmp) {
	malloc_header *cur, *prev=head;
	cur=prev->sorted_next;
	while(cur!=nullptr&&cur!=cmp) {
		prev=cur;
		cur=cur->sorted_next;
	}
	if(cur==nullptr) {
		return;
	}
	prev->sorted_next=cur->sorted_next;
	cur->sorted_next=nullptr;
}
void sorted_insert(malloc_header *insert) {
	malloc_header *left=head;
	while(left->sorted_next!=nullptr&&left->sorted_next->size < insert->size) {
		left=left->sorted_next;
	}
	insert->sorted_next=left->sorted_next;
	left->sorted_next=insert;
}

malloc_header *get_header(void *vaddr) {
	uintptr_t addr=reinterpret_cast<uintptr_t>(vaddr);
	addr-=sizeof(malloc_header);
	malloc_header *a=reinterpret_cast<malloc_header *>(addr);
	return a;
}
void try_split(malloc_header *a,size_t keep_size) {
	if(a->size-keep_size > min_total_size) {
		//split it
		uintptr_t addr=reinterpret_cast<uintptr_t>(a);
		addr+=alloc_offset(keep_size);
		malloc_header *newh=reinterpret_cast<malloc_header *>(addr);
		memset(newh, 0, sizeof(malloc_header));
		newh->magic=0xC0FFEE;
		newh->inuse=false;
		newh->linear_next=a->linear_next;
		a->linear_next=newh;
		newh->linear_prev=a;
		newh->linear_next->linear_prev=newh;
		newh->size=a->size-alloc_offset(keep_size);
		a->size=keep_size;
		sorted_insert(newh);
	}
}
malloc_header *try_merge_left(malloc_header *a) {
	if(a->linear_prev==head) {
		return a;
	}
	if(!a->linear_prev->inuse) {
		//merge left
		sorted_remove(a->linear_prev);
		a->magic=0xDEADED;
		a->linear_prev->size+=alloc_offset(a->size);
		a->size=0;
		a->linear_prev->linear_next=a->linear_next;
		a->linear_next->linear_prev=a->linear_prev;
		a=a->linear_prev;
	}
	return a;
}
void try_merge_right(malloc_header *a) {
	if(!a->linear_next->inuse) {
		//merge right
		sorted_remove(a->linear_next);
		a->linear_next->magic=0xDEADED;
		a->size+=alloc_offset(a->linear_next->size);
		a->linear_next->size=0;
		a->linear_next=a->linear_next->linear_next;
		a->linear_next->linear_prev=a;
	}
}

void *malloc(size_t size) {
	if((size&(min_alloc_size-1))!=0) {
		size|=(min_alloc_size-1);
		size++;
	}
	Log(LOG_INFO,"[HEAP  ]","allocating for size %d",size);
	malloc_header *cur, *prev=head;
	cur=prev->sorted_next;
	while(cur!=nullptr&&cur->size<size) {
		Log(LOG_INFO, "[HEAP  ]", "MALLOC not using size=%d",cur->size);
		prev=cur;
		cur=cur->sorted_next;
	}
	if(cur==nullptr) {
		Log(LOG_ERROR,"[HEAP  ]","ran out of heap space for %d bytes",size);
	}
	Log(LOG_INFO, "[HEAP  ]", "MALLOC using size=%d",cur->size);
	try_split(cur, size);
	sorted_remove(cur);
	cur->inuse=true;
	cur->magic=0xDECAFF;
	Log(LOG_INFO, "[HEAP  ]", "MALLOC next: %d",cur->linear_next->size);
	uintptr_t addr=reinterpret_cast<uintptr_t>(cur);
	addr+=sizeof(malloc_header);
	return reinterpret_cast<void *>(addr);
}

void free(void *vaddr) {
	malloc_header *fhead=get_header(vaddr);
	if(fhead->magic!=0xDECAFF) {
		if(fhead->magic==0xC0FFEE||fhead->magic==0xDEADED) {
			Log(LOG_WARNING,"[HEAP  ]","double free");
			return;
		} else {
			Log(LOG_ERROR,"[HEAP  ]","malloc header corrupted");
		}
	}
	Log(LOG_INFO, "[HEAP  ]", "FREE size=%d",fhead->size);
	fhead=try_merge_left(fhead);
	try_merge_right(fhead);
	fhead->magic=0xC0FFEE;
	fhead->inuse=false;
	Log(LOG_INFO, "[HEAP  ]", "FREE inserting size=%d",fhead->size);
	sorted_insert(fhead);
}

void *realloc(void *vaddr,size_t newsize) {
	if(vaddr==nullptr) {
		return malloc(newsize);
	}
	malloc_header *fhead=get_header(vaddr);
	try_merge_right(fhead);
	if(fhead->size >= newsize) {
		return vaddr;
	}
	void *nvaddr=malloc(newsize);
	memmove(vaddr,nvaddr,fhead->size);
	free(vaddr);
	return nvaddr;
}
void *calloc(size_t num,size_t size) {
	void *vaddr=malloc(num*size);
	memset(vaddr,0,num*size);
	return vaddr;
}

void malloc_init() {
	hal::mem_region *reg=nullptr;
	for(int i=0;i<hal::virtmem.region_count();i++) {
		reg=hal::virtmem.get_region(i);
		if(reg->type.heap) {
			break;
		}
	}
	if(!reg||!reg->type.heap) {
		Log(LOG_ERROR,"[HEAP  ]","no heap region was found");
	}
	uintptr_t start=reg->start;
	Log(LOG_DEBUG, "[HEAP  ]", "head addr: %.16p", start);
	head=reinterpret_cast<malloc_header *>(start);
	memset(head, 0, sizeof(malloc_header));
	head->magic=0xC0FFEE;
	head->inuse=false;
	head->size=0;
	head->linear_prev=nullptr;
	start+=sizeof(malloc_header);
	malloc_header *cur, *prev=head;
	uint32_t newsize=min_alloc_size;
	while((start+alloc_offset(newsize))<reg->end) {
		cur=reinterpret_cast<malloc_header *>(start);
		memset(cur, 0, sizeof(malloc_header));
		cur->magic=0xC0FFEE;
		cur->inuse=false;
		cur->size=newsize;
		cur->linear_prev=prev;
		prev->linear_next=cur;
		sorted_insert(cur);
		start+=alloc_offset(newsize);
		if(newsize<0x80000000){
			newsize<<=1;
		}
		prev=cur;
	}
	Log(LOG_INFO, "[HEAP  ]", "heap initalized");
}

void *operator new(size_t size) {
	return malloc(size);
}

void *operator new[](size_t size) {
	return malloc(size);
}

void operator delete(void *p) {
	free(p);
}

void operator delete[](void *p) {
	free(p);
}
