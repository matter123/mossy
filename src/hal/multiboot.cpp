/*
    Copyright 2013-2014 Matthew Fosdick

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
#include <hal/hal.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
#include <hal/mmap.h>
#include <string.h>
#include <hal/console.h>
namespace hal {
	multiboot_header *head;
	multiboot_tag **tags;
	int tag_count;
	void init_mboot(multiboot_header *mboot) {
		head=reinterpret_cast<multiboot_header *>(w_malloc(mboot->size,8));
		if(!head) {
			print_boot_msg("Init Multiboot",false,true);
		}
		memcpy((void *)head,(void *)mboot,mboot->size);
		if(head->size!=mboot->size) {
			print_boot_msg("Init Multiboot",false,true);
		}
		pointer temp_ptr=reinterpret_cast<pointer>(head)+sizeof(multiboot_header);
		while(true) {
			multiboot_tag *tag=reinterpret_cast<multiboot_tag *>(temp_ptr);
			if(tag->type==0&&tag->size==8) {
				break;
			} else {
				tag_count++;
				temp_ptr+=tag->size;
				if((uintptr_t)temp_ptr%8) {
					temp_ptr=(pointer)(((uintptr_t)temp_ptr&~0x7)+8);
				}
			}
		}
		void *temp2_ptr=w_malloc(sizeof(multiboot_tag *)*tag_count);
		if(!temp2_ptr) {
			print_boot_msg("Init Multiboot",false,true);
		}
		tags=reinterpret_cast<multiboot_tag **>(temp2_ptr);
		temp_ptr=reinterpret_cast<pointer>(head)+sizeof(multiboot_header);
		for(int i=0; i<tag_count; i++) {
			tags[i]=reinterpret_cast<multiboot_tag *>(temp_ptr);
			temp_ptr+=tags[i]->size;
			if(tags[i]->type==3) {
				multiboot_module *tmp=reinterpret_cast<multiboot_module *>
				                      (w_malloc(sizeof(multiboot_module)));
				multiboot_module_int *mmi=reinterpret_cast<multiboot_module_int *>(tags[i]);
				tmp->head.type=3;
				tmp->mod_start=static_cast<uintptr_t>(mmi->mod_start);
				tmp->mod_end=static_cast<uintptr_t>(mmi->mod_end);
				tags[i]=reinterpret_cast<multiboot_tag *>(tmp);
			}
			if((uintptr_t)temp_ptr%8) {
				temp_ptr=(pointer)(((uintptr_t)temp_ptr&~0x7)+8);
			}
		}
		//total size of all modules should be less than ~70% of workspace area
		for(int i=0; i<tag_count; i++) {
			if(get_tag(i)==0) {
				break;
			}
			if(get_tag(i)->type==3) {
				//relocate this module
				multiboot_module *module=reinterpret_cast<multiboot_module *>
				                         (get_tag(i));
				void *start;
				if(module->mod_end-module->mod_start>=0x19000) {
					hal::cout<<hal::address<<module->mod_start;
					//hal::magic_break();
					//skip over big modules
					continue;
				}
				start=w_malloc(module->mod_end-module->mod_start);
				if(!start) {
					print_boot_msg("Init Multiboot",false,true);
				}
				memmove(start,reinterpret_cast<void *>(module->mod_start),
				        module->mod_end-module->mod_start);
				module->mod_start=reinterpret_cast<uintptr_t>(start);
			}
		}
		print_boot_msg("Init Multiboot",true,false);
		print_boot_msg("Init MMAP",init_phys_mem(),true);
	}

	int get_tag_count() {
		return tag_count;
	}

	multiboot_tag *get_tag(int count) {
		if(count<tag_count) {
			return tags[count];
		}
		return reinterpret_cast<multiboot_tag *>(0);
	}
	multiboot_header *get_header() {
		return head;
	}
}
