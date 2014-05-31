/*
    Copyright 2013 Matthew Fosdick

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
#include <hal/multiboot.h>
#include <hal/workspace.h>
#include <string.h>
namespace hal {
	multiboot_header *head;
	multiboot_tag **tags;
	int tag_count;

	void init_mboot(multiboot_header *mboot) {
		head=reinterpret_cast<multiboot_header *>(w_malloc(mboot->size));
		memmove((void *)head,(void *)mboot,mboot->size);;
		void *temp_ptr=reinterpret_cast<void *>(head)+sizeof(multiboot_header);
		while(true) {
			multiboot_tag *tag=reinterpret_cast<multiboot_tag *>(temp_ptr);
			if(tag->type!=0) {
				tag_count++;
				temp_ptr+=tag->size;
			} else {
				break;
			}
		}
		void *temp2_ptr=w_malloc(sizeof(multiboot_tag *)*tag_count);
		tags=reinterpret_cast<multiboot_tag **>(temp2_ptr);
		temp_ptr=reinterpret_cast<void *>(head)+sizeof(multiboot_header);
		for(int i=0; i<tag_count; i++) {
			tags[i]=reinterpret_cast<multiboot_tag *>(temp_ptr);
			temp_ptr+=tags[i]->size;
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
				void *start=w_malloc(module->mod_end-module->mod_start);
				memmove(start,reinterpret_cast<void *>(module->mod_start),
				        module->mod_end-module->mod_start);
				module->mod_start=reinterpret_cast<uintptr_t>(start);
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
		return reinterpret_cast<multiboot_tag *>(0);
	}
	multiboot_header *get_header() {
		return head;
	}
}