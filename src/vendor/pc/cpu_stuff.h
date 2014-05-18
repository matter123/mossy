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
#pragma once
#include <vendor.h>
#ifdef PC
#include <stdint.h>
extern "C" uint32_t cpuid(uint32_t eax, int retreg) __attribute__((const));

static inline void wrmsr(uint32_t msr_id, uint64_t msr_value) {
	asm volatile("wrmsr" : : "c"(msr_id), "A"(msr_value));
}
static inline uint64_t rdmsr(uint32_t msr_id) {
	uint64_t msr_value;
	asm volatile("rdmsr" : "=A"(msr_value) : "c"(msr_id));
	return msr_value;
}
static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
static inline void outw(uint16_t port, uint16_t val) {
	asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}
#endif