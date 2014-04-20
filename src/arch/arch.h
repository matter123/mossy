/*
 * Copyright 2013 Matthew Fosdick

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
#ifndef asm
#include <stdint.h>
#endif
#if ARCH == x64
#define X64
#define UINTREG_MAX UINT64_MAX
#ifndef asm
typedef uint64_t uintreg_t;
struct cpu_state{
	uintreg_t r15, r14, r13, r12, r11, r10, r9, r8;
	uintreg_t rdi, rsi, rdx, rcx, rbx, rax, rbp, ds, fs, gs;
	uintreg_t int_num, code;
	uintreg_t cs, rip, ss, rsp;
};
#endif
#elif ARCH == x86
#define UINTREG_MAX UINT64_MAX
#define X86
#ifndef asm
typedef uint32_t uintreg_t;
#endif
#endif