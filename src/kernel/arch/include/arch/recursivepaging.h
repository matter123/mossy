/*
	Copyright 2016 Matthew Fosdick

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
#pragma once
inline uintptr_t PML4_OFF(uintptr_t index) {
	if(index<256) {
		return 0x40000000UL*512*index;
	}
	//sign extend
	index=512-index;
	return 0xFFFFFFFFFFFFFFFFUL-(0x40000000UL*512*index-1);
}
inline uintptr_t PDP_OFF(uintptr_t index) {
	return 0x40000000UL*index;
}
inline uintptr_t PD_OFF(uintptr_t index) {
	return 0x200000UL*index;
}
inline uintptr_t PT_OFF(uintptr_t index) {
	return 0x1000UL*index;
}
struct pte {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool dirty :1;
	bool pat :1;
	bool global :1;
	int flags :3;
	uint64_t addr :40;
	int flags_1 :10;
	bool nx :1;
} PACKED;
struct pde {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool ign :1;
	int zero :2;
	int flags :3;
	uint64_t addr :40;
	int flags_1 :10;
	bool nx :1;
} PACKED;
struct pdpe {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool ign :1;
	int zero :2;
	int flags :3;
	uint64_t addr :40;
	int flags_1 :10;
	bool nx :1;
} PACKED;
struct pml4e {
	bool present :1;
	bool read_write :1;
	bool user :1;
	bool write_through :1;
	bool no_cache :1;
	bool accessed :1;
	bool ign :1;
	int zero :2;
	int flags :3;
	uint64_t addr :40;
	int flags_1 :10;
	bool nx :1;
} PACKED;
union pt {
	pte entries[512];
	uint64_t entriesi[512];
} PACKED;
union pd {
	pde entries[512];
	uint64_t entriesi[512];
} PACKED;
union pdp {
	pdpe entries[512];
	uint64_t entriesi[512];
} PACKED;
union pml4 {
	pml4e entries[512];
	uint64_t entriesi[512];
} PACKED;

class recursive_paging {
	uintptr_t base;
	uint index;
	public:
	void init(uint index);
	void map(uintptr_t phys,uintptr_t virt,uint flags);
	uintptr_t unmap(uintptr_t virt);
	uintptr_t locate(uintptr_t virt);
	pte *page_info(uintptr_t virt);
};
