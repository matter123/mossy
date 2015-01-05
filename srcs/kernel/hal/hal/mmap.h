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

#pragma once
#include <stdint.h>
#include <hal/mem_region.h>
namespace hal {
	////////////////////////////////////////
	// Initialize the physical memory map //
	//   and sort boot entries            //
	// Created on 2014-06-03              //
	// Return true on success             //
	////////////////////////////////////////
	bool init_phys_mem();

	////////////////////////////////////////
	// Get current count of memory        //
	//   regions                          //
	// Created on 2014-07-03              //
	////////////////////////////////////////
	int get_phys_mem_regions();

	////////////////////////////////////////
	// Get a region at index 'index'      //
	// Returns a region                   //
	// Created on 2014-07-13              //
	////////////////////////////////////////
	mem_region *get_phys_mem_region(int index);

	////////////////////////////////////////
	// Initialize the virtual memory map  //
	// Created on 2014-07-23              //
	// Return true on success             //
	////////////////////////////////////////
	bool init_virt_mem();

	////////////////////////////////////////
	// Get current count of memory        //
	//   regions                          //
	// Created on 2014-07-23              //
	////////////////////////////////////////
	int get_virt_mem_regions();

	////////////////////////////////////////
	// Get a region at index 'index'      //
	// Returns a region                   //
	// Created on 2014-07-23              //
	////////////////////////////////////////
	mem_region *get_virt_mem_region(int index);

	////////////////////////////////////////
	// Add an region to the physical      //
	//   memory map                       //
	// Created on 2014-06-11              //
	// Return true on success             //
	////////////////////////////////////////
	bool add_phys_region(uint64_t start, uint64_t len, mem_type type);

	////////////////////////////////////////
	// Declares number of regions adding  //
	// Created on 2014-06-11              //
	////////////////////////////////////////
	void add_phys_region(int count);

	////////////////////////////////////////
	// Add an region to the virtual       //
	//   memory map                       //
	// Created on 2014-06-11              //
	// Return true on success             //
	////////////////////////////////////////
	bool add_virt_region(uint64_t start, uint64_t len, mem_type type);

	////////////////////////////////////////
	// Declares number of regions adding  //
	// Created on 2014-06-11              //
	////////////////////////////////////////
	void add_virt_region(int count);
}
