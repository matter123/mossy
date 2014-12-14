/*
    Copyright 2014 Matthew Fosdick

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <arch.h>
#include <hal/mmap.h>
#include <hal/hal.h>
#include <hal/console.h>
#if TEST
#include <test.h>
#include <stdlib.h>
void test_heap()__attribute__((used));
static kernel::test_module test_module=kernel::test_module("HEAP    ",&test_heap);
void test_heap() {
	void *h1=malloc(4);
	free(h1);
	void *h2=malloc(4);
	unit_test("heap merge     #1",h2,h1);
	free(h2);
	void *bar[20];
	for(int i=0; i<20; i++) {
		bar[i]=malloc(4*(i+1));
	}
	for(int i=1; i<20; i+=2) {
		free(bar[i]);
	}
	for(int i=0; i<20; i+=2) {
		free(bar[i]);
	}
	h1=malloc(840);
	unit_test("heap merge     #2",h1,h2);
	free(h1);
}
#endif
namespace kernel {
	struct HEAD {
		uint     magic:24;
		bool     free:1;
		int      resv:7;
		uint32_t len;
		HEAD    *prev;
		HEAD    *next;
	};
	HEAD *start;
	bool heap_init() {
		hal::mem_region *region=NULL;
		for(int i=0; i<hal::get_virt_mem_regions(); i++) {
			hal::mem_region *reg=hal::get_virt_mem_region(i);
			if(reg->type.heap) {
				//for sanity only assume one section
				region=reg;
				break;
			}
		}
		if(!region) {
			return false;
		}
		start=reinterpret_cast<HEAD *>(region->start);
		memset(start,0,sizeof(HEAD));
		start->magic=0xC0FFEE;
		start->free=true;
		uint64_t len=region->end-region->start;
		if(len<=UINT32_MAX) {
			start->len=static_cast<uint32_t>(len);
		} else {
			start->len=UINT32_MAX-sizeof(HEAD);
			len-=UINT32_MAX;
			HEAD *cur=start;
			//hal::magic_break();
			while(len>UINT32_MAX) {
				pointer addr=(pointer)cur;
				addr+=UINT32_MAX;
				//hal::cout<<hal::address<<addr<<hal::endl;
				HEAD *next=(HEAD *)addr;
				memset(next,0,sizeof(HEAD));
				next->magic=0xC0FFEE;
				next->free=true;
				next->len=UINT32_MAX-sizeof(HEAD);
				len-=UINT32_MAX;
				cur->next=next;
				next->prev=cur;
				cur=cur->next;
			}
			if(len>sizeof(HEAD)) {
				pointer addr=(pointer)cur;
				addr+=UINT32_MAX;
				HEAD *next=(HEAD *)addr;
				memset(next,0,sizeof(HEAD));
				next->magic=0xC0FFEE;
				next->free=true;
				next->len=len-sizeof(HEAD);
				len=0;
				cur->next=next;
				next->prev=cur;
			}
		}
		return true;
	}
	static void split(HEAD *head, size_t new_size);
	static void dumb_merge(HEAD *left, HEAD *right);
	static void scoot(HEAD *prev, size_t prev_size);
	extern "C"
	void *malloc(size_t size) {
		if(size%sizeof(uintptr_t)) {
			size+=sizeof(uintptr_t)-(size%sizeof(uintptr_t));
		}
		if(size==0) {
			return NULL;
		}
		HEAD *cur=start;
		while(!cur->free||cur->len<size) {
			if(cur->next) {
				cur=cur->next;
				continue;
			}
			return NULL;
		}
		if(cur->len>(size+sizeof(HEAD))) {
			split(cur,size);
		}
		cur->free=false;
		return ((pointer)cur)+sizeof(HEAD);
	}
	extern "C"
	void free(void *ptr) {
		HEAD *head=(HEAD *)((pointer)ptr-sizeof(HEAD));
		if(head->magic!=0xC0FFEE) {
			return;
		}
		if(head->free) {
			return;
		}
		head->free=true;
		while(head->prev&&head->prev->free) {
			dumb_merge(head->prev,head);
			head=head->prev;
		}
		while(head->next&&head->next->free) {
			dumb_merge(head,head->next);
		}
	}
	extern "C"
	void *calloc(size_t num, size_t size) {
		void *addr=malloc(num*size);
		memset(addr,0,num*size);
		return addr;
	}
	extern "C"
	void *realloc(void *ptr,size_t size) {
		if(ptr==NULL) {
			return malloc(size);
		}
		HEAD *head=(HEAD *)((pointer)ptr-sizeof(HEAD));
		if(head->magic!=0xC0FFEE) {
			return NULL;
		}
		if(head->free) {
			return NULL;
		}
		if(size==0) {
			free(ptr);
			return NULL;
		}
		if(size%sizeof(uintptr_t)) {
			size+=sizeof(uintptr_t)-(size%sizeof(uintptr_t));
		}
		if(size<head->len) {
			if(head->len>(size+sizeof(HEAD))) {
				split(head,size);
				return ptr;
			} else {
				if(head->next&&head->next->free) {
					scoot(head,size);
					return ptr;
				}
				return ptr;
			}
		} else if(size>head->len) {
			size_t os=head->len;
			while(head->next&&head->next->free) {
				dumb_merge(head,head->next);
			}
			if(size<head->len) {
				//cheap trick to not repeat the shrinking logic above
				return realloc(head,size);
			} else {
				void *addr=malloc(size);
				memcpy(addr,ptr,os);
				free(head);
				return addr;
			}
		}
		return ptr;
	}
	void split(HEAD *head, size_t new_size) {
		HEAD *newh=(HEAD *)(((pointer)head)+sizeof(HEAD)+new_size);
		newh->len=head->len-(new_size+sizeof(HEAD));
		newh->magic=0xC0FFEE;
		newh->free=true;
		newh->prev=head;
		newh->next=head->next;
		head->next=newh;
		if(newh->next) {
			newh->next->prev=newh;
		}
	}
	void dumb_merge(HEAD *left, HEAD *right) {
		left->len+=sizeof(HEAD)+right->len;
		right->magic=0xBADBAD;
		left->next=right->next;
		if(left->next) {
			left->next->prev=left;
		}
	}
	void scoot(HEAD *head, size_t new_size) {
		HEAD *newh=(HEAD *)(((pointer)head)+sizeof(HEAD)+new_size);
		if(head->next) {
			memmove(newh,head->next,sizeof(HEAD));
			if(newh->next) {
				newh->next->prev=newh;
			}
			head->next=newh;
		}
	}
}
extern "C"
void *operator new(size_t size) {
	return kernel::malloc(size);
}

extern "C"
void *operator new[](size_t size) {
	return kernel::malloc(size);
}

extern "C"
void operator delete(void *p) {
	kernel::free(p);
}

extern "C"
void operator delete[](void *p) {
	kernel::free(p);
}