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
#include <hal/memregion.h>
#include <hal/memtype.h>
#include <hal/workspace.h>
#include <stddef.h>
#include <string.h>
namespace hal {
mem_regs *update(mem_regs *regs, bool dir) {
	for(size_t s = 0; s < regs->tag_count; s++) {
		if(dir) {
			regs->regions[s].new_end = regs->regions[s].end;
			regs->regions[s].new_start = regs->regions[s].start;
		} else {
			regs->regions[s].end = regs->regions[s].new_end;
			regs->regions[s].start = regs->regions[s].new_start;
		}
	}
	return regs;
}
mem_regs *remove_invalid(mem_regs *regs) {
	for(size_t s = 0; s < regs->tag_count; s++) {
		if(regs->regions[s].new_end <= regs->regions[s].new_start || regs->regions[s].type.to_u64() == 0) {
			memmove((void *)(regs->regions + s), (void *)(regs->regions + s + 1), regs->tag_count - s - 1);
			regs->tag_count--;
		}
	}
	return regs;
}
mem_regs *sort(mem_regs *regs) {
	if(regs->tag_count <= 1) return regs;
	mem_region temp;
	bool has_swap;
	do {
		has_swap = false;
		for(int i = 1; i < regs->tag_count; i++) {
			if(regs->regions[i - 1].new_start > regs->regions[i].new_start) {
				memcpy(&temp, &regs->regions[i], sizeof(mem_region));
				memcpy(&regs->regions[i], &regs->regions[i - 1], sizeof(mem_region));
				memcpy(&regs->regions[i - 1], &temp, sizeof(mem_region));
				has_swap = true;
			}
			if(regs->regions[i - 1].new_start == regs->regions[i].new_start) {
				if(regs->regions[i - 1].new_end > regs->regions[i].new_end) {
					memcpy(&temp, &regs->regions[i], sizeof(mem_region));
					memcpy(&regs->regions[i], &regs->regions[i - 1], sizeof(mem_region));
					memcpy(&regs->regions[i - 1], &temp, sizeof(mem_region));
					has_swap = true;
				}
			}
		}
	} while(has_swap);
	return regs;
}
/*
case 1
0x1000-0x1FFF 0x100
0x2000-0x2FFF 0x200
---
0x1000-0x1FFF 0x100
0x2000-0x2FFF 0x200

case 2(I)
0x1000-0x27FF 0x200
0x2000-0x2FFF 0x100
---
0x1000-0x27FF 0x200
0x2800-0x2FFF 0x100

case 2(II)
0x1000-0x27FF 0x100
0x2000-0x2FFF 0x200
---
0x1000-0x1FFF 0x100
0x2000-0x2FFF 0x200

case 3(I)
0x1000-0x2FFF 0x200
0x1400-0x27FF 0x100
---
0x1000-0x2FFF 0x200
0x3000-0x2FFF 0x100

case 3(II)
0x1000-0x2FFF 0x100
0x2000-0x27FF 0x200
---
0x1000-0x1FFF 0x100
0x2000-0x27FF 0x200
0x2800-0x2FFF 0x100
 */
mem_regs *split(mem_regs *regs) {
	if(regs->tag_count <= 1) return regs;
	for(int i = 1; i < regs->tag_count; i++) {
		// case 1: no contested space
		if(regs->regions[i - 1].new_end < regs->regions[i].new_start) { continue; }
		// case 2: partially contested space
		if(regs->regions[i - 1].new_end < regs->regions[i].new_end) {
			// left has contested space
			if(regs->regions[i - 1].type.is_more_restrictive(regs->regions[i].type)) {
				regs->regions[i].new_start = regs->regions[i - 1].new_end + 1;
				continue;
			}
			// right takes space
			regs->regions[i - 1].new_end = regs->regions[i].new_start - 1;
			continue;
		}
		// case 3: fully contested space
		// case 3(I): left is more restrictive
		if(regs->regions[i - 1].type.is_more_restrictive(regs->regions[i].type)) {
			regs->regions[i].new_start = regs->regions[i - 1].new_end + 1;
			regs->regions[i].new_end = regs->regions[i - 1].new_end + 1;
		}
		// case 3(II): right is more restrictive
		uint64_t old_end = regs->regions[i - 1].new_end;
		regs->regions[i - 1].new_end = regs->regions[i].new_start - 1;
		memmove((void *)(regs->regions + i + 2), (void *)(regs->regions + i + 1), regs->tag_count - i);
		memcpy((void *)(regs->regions + i + 1), (void *)(regs->regions + i - 1), sizeof(mem_region));
		regs->regions[i + 1].new_start = regs->regions[i].new_end + 1;
		regs->regions[i + 1].new_end = old_end;
		regs->tag_count++;
	}
	return regs;
}
/*mem_regs *oldsplit(mem_regs *regs) {
    if(regs->tag_count <= 1) return regs;
    mem_region *t_regions = (mem_region *)w_malloc(sizeof(mem_region));
    int ecount = 0;
    for(int i = 1; i < regs->tag_count; i++) {
        if(ecount == 0) { memcpy(&t_regions[ecount++], &regs->regions[0], sizeof(mem_region)); }
        // case 1
        if(t_regions[ecount - 1].new_end < regs->regions[i].new_start) {
            memcpy(&t_regions[ecount++], &regs->regions[i], sizeof(mem_region));
            continue;
        }
        // case 2
        if(t_regions[ecount - 1].new_end <= regs->regions[i].new_end) {
            // left takes conflict
            if(t_regions[ecount - 1].type.is_more_restrictive(regs->regions[i].type)) {
                memcpy(&t_regions[ecount++], &regs->regions[i], sizeof(mem_region));
                t_regions[ecount - 1].new_start = t_regions[ecount - 2].new_end + 1;
                if(t_regions[ecount - 1].new_end <= t_regions[ecount - 1].new_start) { ecount--; }
                continue;
            }
            // right takes conflict
            t_regions[ecount - 1].new_end = regs->regions[i].new_start - 1;
            if(t_regions[ecount - 1].new_end <= t_regions[ecount - 1].new_start) { ecount--; }
            memcpy(&t_regions[ecount++], &regs->regions[i], sizeof(mem_region));
            continue;
        }
        // case 3
        if(t_regions[ecount - 1].type.is_more_restrictive(regs->regions[i].type)) {
            // absorb (skip over and do nothing)
            continue;
        }
        // split
        uint64_t old_end = t_regions[ecount - 1].new_end;
        t_regions[ecount - 1].new_end = regs->regions[i].new_start - 1;
        memcpy(&t_regions[ecount++], &regs->regions[i], sizeof(mem_region));
        t_regions[ecount].new_start = t_regions[ecount - 1].new_end + 1;
        t_regions[ecount].new_end = old_end;
        t_regions[ecount].type = t_regions[ecount - 2].type;
        ecount++;
    }
    // regs->regions = t_regions;
    regs->tag_count = ecount;
    wksp_reset((pointer_t)regs->regions + regs->tag_count * sizeof(mem_region));
    return regs;
}*/
mem_regs *page_align(mem_regs *regs) {
#define START_MASK ~((uintptr_t)0xFFF)
	for(int i = 0; i < regs->tag_count; i++) {
		if((regs->regions[i].new_start & 0xFFF) != 0) {
			uint64_t old_start = regs->regions[i].new_start;
			if(regs->regions[i].type.can_grow()) {
				regs->regions[i].new_start &= START_MASK;
			} else {
				if(!regs->regions[i].type.can_shrink()) {
					Log(LOG_ERROR, "[MEMMAP]", "region cannot shrink nor grow and start is not page aligned %d",
					    regs->regions[i].type.to_u64());
				}
				if(!regs->regions[i].type.can_adjust_start()) {
					Log(LOG_ERROR, "[MEMMAP]", "region cannot have start adjusted, but start is not page aligned %d",
					    regs->regions[i].type.to_u64());
				}
				regs->regions[i].new_start &= START_MASK;
				regs->regions[i].new_start += 0x1000;
			}
			if(i && regs->regions[i - 1].new_end == (old_start - 1)) {
				regs->regions[i - 1].new_end = (regs->regions[i].new_start - 1);
			}
		}
		if(regs->regions[i].new_end == 0x100000000) { regs->regions[i].new_end -= 1; }
		if((regs->regions[i].new_end & 0xFFF) != 0xFFF) {
			uintptr_t old_end = regs->regions[i].new_end;
			if(regs->regions[i].type.can_grow()) {
				regs->regions[i].new_end = regs->regions[i].new_end | 0xFFF;
			} else {
				if(!regs->regions[i].type.can_shrink()) {
					PANIC("region cannot shrink nor grow and end is not page aligned");
				}
				// cut off to nearest page
				regs->regions[i].new_end &= START_MASK;
				regs->regions[i].new_end -= 1;
			}
			if((i + 1) < regs->tag_count && regs->regions[i + 1].new_start - old_end == 1) {
				regs->regions[i + 1].new_start = regs->regions[i].new_end + 1;
			}
		}
	}
	return regs;
}
mem_type ne;
mem_regs *fill(mem_regs *regs) {
	if(regs->tag_count <= 1) return regs;
	ne.no_exist = true;
	for(int i = 1; i < regs->tag_count; i++) {
		if((regs->regions[i].new_start - 1) > regs->regions[i - 1].new_end) {
			memmove((void *)(regs->regions + i + 1), (void *)(regs->regions + i), regs->tag_count - i - 1);
			regs->tag_count++;
			regs->regions[i].new_start = regs->regions[i - 1].new_end + 1;
			regs->regions[i].new_end = regs->regions[i + 1].new_start - 1;
			regs->regions[i].type = ne;
		}
	}
	return regs;
}
}
