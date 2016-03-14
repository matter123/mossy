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
		//get count of multiboot tags
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
		temp_ptr=reinterpret_cast<pointer>(head)+sizeof(multiboot_header);
		tags=reinterpret_cast<multiboot_tag **>(w_malloc(sizeof(multiboot_tag *)*tag_count));
		if(tags==nullptr) {
			PANIC("tags is null");
		}
		//copy tags into the tags array
		for(int i=0; i<tag_count; i++) {
			tags[i]=reinterpret_cast<multiboot_tag *>(temp_ptr);
			temp_ptr+=tags[i]->size;
			//change 32 bit addresses into correct type
			if(tags[i]->type==3) {
				multiboot_module *tmp=reinterpret_cast<multiboot_module *>(w_malloc(sizeof(multiboot_module)));
				multiboot_module_int *mmi=reinterpret_cast<multiboot_module_int *>(tags[i]);
				tmp->head.type=3;
				tmp->head.size=mmi->head.size;
				tmp->mod_start=static_cast<uintptr_t>(mmi->mod_start);
				tmp->mod_end=static_cast<uintptr_t>(mmi->mod_end);
				tags[i]=reinterpret_cast<multiboot_tag *>(tmp);
			}
			if((uintptr_t)temp_ptr%8) {
				temp_ptr=(pointer)(((uintptr_t)temp_ptr&~0x7)+8);
			}
		}
		//copy the modules that we can into higher memory
		//too big ones should already be in the higher memory
		for(int i=0; i<tag_count; i++) {
			if(get_tag(i)->type==3) {
				multiboot_module *module=reinterpret_cast<multiboot_module *>(get_tag(i));
				size_t module_size=module->mod_end-module->mod_start;
				//cannot be safely copied into the workspace
				if(module_size>=0x19000) {
					continue;
				}
				//be safe, enforce 16 byte alignment
				void *start=w_malloc(module_size,16);
				if(!start) {
					PANIC("could not allocate memory for module");
				}
				memmove(start,reinterpret_cast<void *>(module->mod_start),module_size);
				module->mod_start=reinterpret_cast<uintptr_t>(start);
				module->mod_end=module->mod_start+module_size;
			}
		}
	}
	int get_tag_count() {
		return tag_count;
	}
	multiboot_tag *get_tag(int count) {
		if(count<tag_count) {
			return tags[count];
		}
		return nullptr;
	}
}
