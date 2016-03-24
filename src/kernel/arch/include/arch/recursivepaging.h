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
class recursive_paging {
	uintptr_t base;
	uint index;
	public:
	void init(uint index);
	void map(uintptr_t phys,uintptr_t virt,uint flags);
	uintptr_t unmap(uintptr_t virt);
	uintptr_t locate(uintptr_t virt);
};
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
