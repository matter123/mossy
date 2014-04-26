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
#if ARCH == x64
#define X64
#define UINTREG_MAX UINT64_MAX
#elif ARCH == x86
#define UINTREG_MAX UINT32_MAX
#define X86
#endif

#ifndef asm
#include <stdint.h>
#if ARCH == x64
#define X64
typedef uint64_t uintreg_t;
struct cpu_state {
	uintreg_t cr4, cr3, cr2, cr0;
	uintreg_t r15, r14, r13, r12, r11, r10, r9, r8;
	uintreg_t rdi, rsi, rdx, rcx, rbx, rax, rbp, fs, gs;
	uintreg_t mnemonic, int_num, code;
	uintreg_t rip, cs, rflags, rsp, ss;
};
#elif ARCH == x86
#define UINTREG_MAX UINT32_MAX
#define X86
typedef uint32_t uintreg_t;
struct cpu_state {
	uintreg_t cr4, cr3, cr2, cr0;
	uintreg_t edi, esi, edx, ecx, ebx, eax, ebp, ds, es, fs, gs;
	uintreg_t mnemonic, int_num, code;
	uintreg_t eip, cs, eflags, esp, ss;
};
#endif

uint8_t get_reg_count();
uint8_t get_creg_count();
uint16_t get_int_num(cpu_state *s);
char *get_mnemonic(cpu_state *s);
uintreg_t get_err_code(cpu_state *s);
uintreg_t get_reg(cpu_state *s, uint8_t reg);
uintreg_t get_creg(cpu_state *s, uint8_t creg);
void set_reg(cpu_state *s, uint8_t reg, uintreg_t value);
uintreg_t get_stack_pointer(cpu_state *s,bool userspace);
void set_stack_pointer(cpu_state *s,bool userspace,uintreg_t value);
#endif