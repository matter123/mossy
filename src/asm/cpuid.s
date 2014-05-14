;  Copyright 2013 Matthew Fosdick

;  Licensed under the Apache License, Version 2.0 (the "License");
;  you may not use this file except in compliance with the License.
;  You may obtain a copy of the License at

;    http://www.apache.org/licenses/LICENSE-2.0

;  Unless required by applicable law or agreed to in writing, software
;  distributed under the License is distributed on an "AS IS" BASIS,
;  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;  See the License for the specific language governing permissions and
;  limitations under the License.
#define asm
#include <arch.h>

#ifdef X86
[GLOBAL cpuid]
cpuid:
	push ebp
	mov ebp, esp
	push ebx
	mov eax, [ebp+8]
	cpuid
	cmp dword [ebp+12], 0 ;eax
	je .ret
	cmp dword [ebp+12], 1 ;ebx
	je .ret_ebx
	cmp dword [ebp+12], 2 ;ecx
	je .ret_ecx
	mov eax, edx ;edx
	jmp .ret
	.ret_ebx:
		mov eax, ebx
		jmp .ret
	.ret_ecx:
		mov eax, ecx
	.ret:
		pop ebx
		pop ebp
		ret
#endif

#ifdef X64
[GLOBAL cpuid]
cpuid:
	push rbp
	mov rbp, rsp
	push rbx
	xor rax, rax
	mov rax, rdi
	cpuid
	cmp qword rsi, 0 ;eax
	je .ret
	cmp qword rsi, 1 ;ebx
	je .ret_rbx
	cmp qword rsi, 2 ;ecx
	je .ret_rcx
	mov rax, rdx ;edx
	jmp .ret
	.ret_rbx:
		mov rax, rbx
		jmp .ret
	.ret_rcx:
		mov rax, rcx
	.ret:
		and qword rax, 0xFFFFFFFF ;cpuid is limited to 32 bit return, this removes excess junk in the regs
		pop rbx
		pop rbp
		ret
#endif