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
[SECTION .text]
%macro push_x86 0
push word gs
push word fs
push word es
push word ds
push dword ebp
push dword eax
push dword ebx
push dword ecx
push dword edx
push dword esi
push dword edi
mov  eax, cr0
push eax
mov  eax, cr2
push eax
mov  eax, cr3
push eax
mov  eax, cr4
push eax
%endmacro

%macro pop_x86 0
add esp, 16
pop dword edi
pop dword esi
pop dword edx
pop dword ecx
pop dword ebx
pop dword eax
pop dword ebp
pop word ds
pop word es
pop word fs
pop word gs
%endmacro

%macro EXCEPT_C 2
[global exc%1]
exc%1:
	push dword %1
	%ifdef DEBUG
	push dword %2
	%else
	push dword 0
	%endif
	jmp exc_stub
%endmacro

%macro EXCEPT_NC 2
[global exc%1]
exc%1:
	push dword 0
	push dword %1
	%ifdef DEBUG
	push dword %2
	%else
	push dword 0
	%endif
	jmp exc_stub
%endmacro

EXCEPT_NC 0,  e00
EXCEPT_NC 1,  e01
EXCEPT_NC 2,  e02
EXCEPT_NC 3,  e03
EXCEPT_NC 4,  e04
EXCEPT_NC 5,  e05
EXCEPT_NC 6,  e06
EXCEPT_NC 7,  e07
EXCEPT_C  8,  e08
EXCEPT_NC 9,  e09
EXCEPT_C  10, e0A
EXCEPT_C  11, e0B
EXCEPT_C  12, e0C
EXCEPT_C  13, e0D
EXCEPT_C  14, e0E
EXCEPT_NC 15, e0F
EXCEPT_NC 16, e10
EXCEPT_NC 17, e11
EXCEPT_NC 18, e12
EXCEPT_NC 19, e13
EXCEPT_NC 20, e14
EXCEPT_NC 21, e15
EXCEPT_NC 22, e16
EXCEPT_NC 23, e17
EXCEPT_NC 24, e18
EXCEPT_NC 25, e19
EXCEPT_NC 26, e1A
EXCEPT_NC 27, e1B
EXCEPT_NC 28, e1C
EXCEPT_NC 29, e1D
EXCEPT_NC 30, e1E
EXCEPT_NC 31, e1F

[EXTERN _ZN3hal11call_handleEP9cpu_state]
exc_stub:
	push_x86
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	push esp
	call _ZN3hal11call_handleEP9cpu_state
	add esp, 4
	pop_x86
	add esp, 12
	iretd

[SECTION .rodata]
e00: db "#DE",0x0
e01: db "#DB",0x0
e02: db "#NMI",0x0
e03: db "#BP",0x0
e04: db "#OF",0x0
e05: db "#BR",0x0
e06: db "#UD",0x0
e07: db "#NM",0x0
e08: db "#DF",0x0
e09: db "#RSV09",0x0
e0A: db "#TS",0x0
e0B: db "#NP",0x0
e0C: db "#SS",0x0
e0D: db "#GP",0x0
e0E: db "#PF",0x0
e0F: db "#RSV0F",0x0
e10: db "#MF",0x0
e11: db "#AC",0x0
e12: db "#MC",0x0
e13: db "#XF",0x0
e14: db "#RSV14",0x0
e15: db "#RSV15",0x0
e16: db "#RSV16",0x0
e17: db "#RSV17",0x0
e18: db "#RSV18",0x0
e19: db "#RSV19",0x0
e1A: db "#RSV1A",0x0
e1B: db "#RSV1B",0x0
e1C: db "#RSV1C",0x0
e1D: db "#RSV1D",0x0
e1E: db "#SX",0x0
e1F: db "#RSV1F",0x0