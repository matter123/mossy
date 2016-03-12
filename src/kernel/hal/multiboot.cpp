//#include <hal/hal.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
//#include <hal/mmap.h>
#include <string.h>
namespace hal {
	multiboot_header *head;
	multiboot_tag **tags;
	int tag_count;
	void init_mboot(multiboot_header *mboot) {
		//make sure multiboot header is in high memory (after kernel)
		head=reinterpret_cast<multiboot_header *>(w_malloc(mboot->size,8));
		if(head==nullptr) {
			PANIC("head is null");
		}
		memcpy((void *)head,(void *)mboot,mboot->size);
		//get count oh multiboot tags
		pointer temp_ptr=reinterpret_cast<pointer>(head)+sizeof(multiboot_header);
		while(true) {
			multiboot_tag *tag=reinterpret_cast<multiboot_tag *>(temp_ptr);
			if(tag->type==0&&tag->size==8) {
				break;
			} else {
				tag_count++;
				temp_ptr+=tag->size;
				//multiboot 1.6 spec requires tags aligned to 8 bytes
				if((uintptr_t)temp_ptr%8) {
					temp_ptr=(pointer)(((uintptr_t)temp_ptr&~0x7)+8);
				}
			}
		}
	}
}
