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
%macro push_x64 0
push qword gs
push qword fs
push qword rbp
push qword rax
push qword rbx
push qword rcx
push qword rdx
push qword rsi
push qword rdi
push qword r8
push qword r9
push qword r10
push qword r11
push qword r12
push qword r13
push qword r14
push qword r15
%endmacro

%macro pop_x64 0
pop qword r15
pop qword r14
pop qword r13
pop qword r12
pop qword r11
pop qword r10
pop qword r9
pop qword r8
pop qword rdi
pop qword rsi
pop qword rdx
pop qword rcx
pop qword rbx
pop qword rax
pop qword rbp
pop qword fs
pop qword gs
%endmacro

%macro EXCEPT_C 2
[global exc%1]
exc%1:
	push qword %1
	push qword %2
	jmp exc_stub
%endmacro

%macro EXCEPT_NC 2
[global exc%1]
exc%1:
	push qword 0
	push qword %1
	push qword %2
	jmp exc_stub
%endmacro

EXCEPT_NC 0  e00
EXCEPT_NC 1  e01
EXCEPT_NC 2  e02
EXCEPT_NC 3  e03
EXCEPT_NC 4  e04
EXCEPT_NC 5  e05
EXCEPT_NC 6  e06
EXCEPT_NC 7  e07
EXCEPT_C  8  e08
EXCEPT_NC 9  e09
EXCEPT_C  10 e0A
EXCEPT_C  11 e0B
EXCEPT_C  12 e0C
EXCEPT_C  13 e0D
EXCEPT_C  14 e0E
EXCEPT_NC 15 e0F
EXCEPT_NC 16 e10
EXCEPT_NC 17 e11
EXCEPT_NC 18 e12
EXCEPT_NC 19 e13
EXCEPT_NC 20 e14
EXCEPT_NC 21 e15
EXCEPT_NC 22 e16
EXCEPT_NC 23 e17
EXCEPT_NC 24 e18
EXCEPT_NC 25 e19
EXCEPT_NC 26 e1A
EXCEPT_NC 27 e1B
EXCEPT_NC 28 e1C
EXCEPT_NC 29 e1D
EXCEPT_NC 30 e1E
EXCEPT_NC 31 e1F

[EXTERN call_handle]
exc_stub:
	push_x64
	xchg bx, bx
	call call_handle
	pop_x64
	add rsp, 24
	iretq


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

