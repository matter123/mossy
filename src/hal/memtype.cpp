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
#include <stddef.h>
#include <stdint.h>
#include <hal/hal.h>
#include <hal/mmap.h>
#include <hal/console.h>

namespace hal {
	bool mem_type::operator==(const mem_type &other) const {
		return (this->to_u64())==(other.to_u64());
	}
	uint64_t mem_type::to_u64() const {
		uint64_t res=0;
		res|=(this->save_on_hib<<0);
		res|=(this->pci_mmap<<1);
		res|=(this->no_exist<<2);
		res|=(this->dma<<3);
		res|=(this->videobuffer<<4);
		res|=(this->firmware<<5);
		res|=(this->bootinfo<<6);
		res|=(this->kernel<<7);
		res|=(this->resv_mem<<8);
		res|=(this->avil<<9);
		return res;
	}
	mem_type::mem_type() {
	}
	mem_type::mem_type(mem_type &other) {
		this->save_on_hib=other.save_on_hib;
		this->pci_mmap=other.pci_mmap;
		this->no_exist=other.no_exist;
		this->dma=other.dma;
		this->videobuffer=other.videobuffer;
		this->firmware=other.firmware;
		this->bootinfo=other.bootinfo;
		this->kernel=other.kernel;
		this->resv_mem=other.resv_mem;
		this->avil=other.avil;
	}
	bool mem_type::is_more_restrictive(mem_type other) const {
		if(*this==other) {
			return false;
		}
		if(this->no_exist|other.no_exist) {
			return this->no_exist;
		}
		if(this->resv_mem^other.resv_mem) {
			return this->resv_mem;
		}
		if(this->kernel^other.kernel) {
			return this->kernel;
		}
		if(this->videobuffer^other.videobuffer) {
			return this->videobuffer;
		}
		if(this->pci_mmap^other.pci_mmap) {
			return this->pci_mmap;
		}
		if(this->dma^other.dma) {
			return this->dma;
		}
		if(this->firmware^other.firmware) {
			return this->firmware;
		}
		if(this->bootinfo^other.bootinfo) {
			return this->bootinfo;
		}
		if(this->save_on_hib^other.save_on_hib) {
			return this->save_on_hib;
		}
		if(this->avil^other.avil) {
			return this->avil;
		}
		return false;
	}

	bool mem_type::can_adjust_start() const {
		if(this->videobuffer|this->pci_mmap|this->dma) {
			return false;
		}
		return true;
	}

	bool mem_type::can_grow() const {
		if(this->no_exist||this->resv_mem||this->kernel) {
			return true;
		}
		return false;
	}
	bool mem_type::can_shrink() const {
		if(this->avil) {
			return true;
		}
		return false;
	}
}