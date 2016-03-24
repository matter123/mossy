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
void pre_paging_init();
void paging_init();
const uint PAGE_WRITE = 1<<1;
const uint PAGE_USER  = 1<<2;
const uint PAGE_WT    = 1<<3;
const uint PAGE_NC    = 1<<4;
const uint PAGE_EXEC  = 1;

void map(uintptr_t phys,uintptr_t virt,uint flags);
bool ismapped(uintptr_t virt);
uintptr_t getphys(uintptr_t virt);
