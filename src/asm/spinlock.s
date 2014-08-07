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
#ifdef DEBUG
#undef DEBUG
%define DEBUG
#endif
#define asm
#include <arch.h>
#ifdef X86
[GLOBAL _ZN6kernel8spinlock7acquireEv]
_ZN6kernel8spinlock7acquireEv:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	pushf
	cli
	lock bts dword [eax], 0
	jnc .acquired

	.retest:
		pause
		test dword [eax], 1
		jnz .retest
		lock bts dword [eax], 0
		jc .retest

	.acquired:
		mov eax, [esp]
		add esp, 4
		pop ebp
		ret


[GLOBAL _ZN6kernel8spinlock7releaseEm]
_ZN6kernel8spinlock7releaseEm:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	mov dword [eax], 0
	push dword [ebp + 12]
	popf
	pop ebp
	ret


#endif

#ifdef X64
[GLOBAL _ZN6kernel8spinlock7acquireEv]
_ZN6kernel8spinlock7acquireEv:
	push rbp
	mov rbp, rsp
	pushfq
	cli
	lock bts dword [rdi], 0
	jnc .acquired

	.retest:
		pause
		test dword [rdi], 1
		jnz .retest
		lock bts dword [rdi], 0
		jc .retest

	.acquired:
		pop qword rax
		pop rbp
		ret


[GLOBAL _ZN6kernel8spinlock7releaseEm]
_ZN6kernel8spinlock7releaseEm:
	push rbp
	mov dword [rdi], 0
	push qword rsi
	popfq
	pop rbp
	ret

#endif