#include <hal/memmap.h>
#include <hal/multiboot.h>
#include <hal/workspace.h>
#include <linker.h>
#include <string.h>
namespace hal {
static multiboot_header *head;
static multiboot_tag **tags;
static int tag_count;
void init_mboot(multiboot_header *mboot) {
	// make sure multiboot header is in high memory (after kernel)
	head = reinterpret_cast<multiboot_header *>(w_malloc(mboot->size, 8));
	if(head == nullptr) {
		PANIC("head is null");
	}
	memcpy((void *)head, (void *)mboot, mboot->size);
	// get count of multiboot tags
	pointer_t temp_ptr = reinterpret_cast<pointer_t>(head) + sizeof(multiboot_header);
	while(true) {
		multiboot_tag *tag = reinterpret_cast<multiboot_tag *>(temp_ptr);
		if(tag->type == t_end_marker && tag->size == 8) {
			break;
		} else {
			tag_count++;
			temp_ptr += tag->size;
			// multiboot 1.6 spec requires tags aligned to 8 bytes
			if((uintptr_t)temp_ptr % 8) {
				temp_ptr = (pointer_t)(((uintptr_t)temp_ptr & ~0x7) + 8);
			}
		}
	}
	temp_ptr = reinterpret_cast<pointer_t>(head) + sizeof(multiboot_header);
	tags = reinterpret_cast<multiboot_tag **>(w_malloc(sizeof(multiboot_tag *) * tag_count));
	if(tags == nullptr) {
		PANIC("tags is null");
	}
	// copy tags into the tags array
	for(int i = 0; i < tag_count; i++) {
		tags[i] = reinterpret_cast<multiboot_tag *>(temp_ptr);
		temp_ptr += tags[i]->size;
		// change 32 bit addresses into correct type
		if(tags[i]->type == t_module) {
			multiboot_module *tmp =
			    reinterpret_cast<multiboot_module *>(w_malloc(sizeof(multiboot_module)));
			multiboot_module_int *mmi = reinterpret_cast<multiboot_module_int *>(tags[i]);
			tmp->head.type = t_module;
			tmp->head.size = mmi->head.size;
			tmp->mod_start = static_cast<uintptr_t>(mmi->mod_start);
			tmp->mod_end = static_cast<uintptr_t>(mmi->mod_end);
			tags[i] = reinterpret_cast<multiboot_tag *>(tmp);
		}
		if((uintptr_t)temp_ptr % 8) {
			temp_ptr = (pointer_t)(((uintptr_t)temp_ptr & ~0x7) + 8);
		}
	}
	// copy the modules that we can into higher memory
	// too big ones should already be in the higher memory
	for(int i = 0; i < tag_count; i++) {
		if(get_tag(i)->type == t_module) {
			multiboot_module *module = reinterpret_cast<multiboot_module *>(get_tag(i));
			size_t module_size = module->mod_end - module->mod_start;
			// cannot be safely copied into the workspace
			if(module_size >= 0x19000) {
				continue;
			}
			// be safe, enforce 16 byte alignment
			void *start = w_malloc(module_size, 16);
			if(!start) {
				PANIC("could not allocate memory for module");
			}
			memmove(start, reinterpret_cast<void *>(module->mod_start), module_size);
			module->mod_start = reinterpret_cast<uintptr_t>(start);
			module->mod_end = module->mod_start + module_size;
		}
	}
}
int get_tag_count() {
	return tag_count;
}
multiboot_tag *get_tag(int count) {
	if(count < tag_count) {
		return tags[count];
	}
	return nullptr;
}
static void multiboot_hook(memmap *mem) {
	multiboot_module *module = NULL;
	for(int i = 0; i < get_tag_count(); i++) {
		hal::multiboot_tag *tag = get_tag(i);
		if(tag->type != t_module) {
			continue;
		}
		module = reinterpret_cast<hal::multiboot_module *>(tag);
		if(module->mod_end - module->mod_start >= 0x19000) {
			mem_type mod_type;
			mod_type.kernel = true;
			mod_type.resv_mem = true;
			if(mem == &physmem) {
				mem->add_region(module->mod_start, module->mod_end, mod_type);
			} else {
				mem->add_region(module->mod_start + KERNEL_VMA, module->mod_end + KERNEL_VMA,
				                mod_type);
			}
		}
	}
	if(mem == &physmem) {
		for(int i = 0; i < get_tag_count(); i++) {
			hal::multiboot_tag *tag = get_tag(i);
			if(tag->type != t_framebuffer) {
				continue;
			}
			multiboot_fb *fb = reinterpret_cast<hal::multiboot_fb *>(tag);
			mem_type fb_type;
			fb_type.videobuffer = true;
			mem->add_region(fb->addr, fb->addr + fb->pitch * fb->height, fb_type);
		}
	}
}
static region_hook rhookP(physmem, &multiboot_hook), rhookV(virtmem, &multiboot_hook);
}
