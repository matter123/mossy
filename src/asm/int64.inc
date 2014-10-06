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
push  gs
push  fs
push  rbp
push  rax
push  rbx
push  rcx
push  rdx
push  rsi
push  rdi
push  r8
push  r9
push  r10
push  r11
push  r12
push  r13
push  r14
push  r15
mov rax, cr0
push  rax
mov rax, cr2
push  rax
mov rax, cr3
push  rax
mov rax, cr4
push  rax

%endmacro

%macro pop_x64 0
add rsp, 32
pop  r15
pop  r14
pop  r13
pop  r12
pop  r11
pop  r10
pop  r9
pop  r8
pop  rdi
pop  rsi
pop  rdx
pop  rcx
pop  rbx
pop  rax
pop  rbp
pop  fs
pop  gs
%endmacro

%macro EXCEPT_C 2
[global exc%1]
exc%1:
	push qword %1
	push qword 0
	%ifdef DEBUG
	push rax
	mov rax, %2
	mov [rsp+8], rax
	pop rax
	%endif
	jmp exc_stub
	db 'FAILFAST'
%endmacro

%macro EXCEPT_NC 2
[global exc%1]
exc%1:
	push qword 0
	push qword %1
	push qword 0
	%ifdef DEBUG
	push rax
	mov rax, %2
	mov [rsp+8], rax
	pop rax
	%endif
	jmp exc_stub
	db 'FAILFAST'
%endmacro

[EXTERN _ZN3hal11call_handleEP9cpu_state]
[EXTERN _ZN6kernel8scheduleEP9cpu_state]
exc_stub:
	push_x64
	mov rdi, rsp
	mov ax, 0x10
	mov fs, ax
	mov gs, ax
	call _ZN3hal11call_handleEP9cpu_state
	;schedule new kernel task
	mov rdi, rsp
	call _ZN6kernel8scheduleEP9cpu_state
	mov rsp, rax
	;clean up stack
	pop_x64
	add rsp, 24
	iretq
[SECTION .rodata]
