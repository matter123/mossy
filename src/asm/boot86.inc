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

[BITS 32]
[section .mboot]

MULTIBOOT_HEADER_MAGIC	equ 0xE85250D6
MULTIBOOT_HEADER_ARCH	equ 0
MULTIBOOT_HEADER_LENGTH	equ (multiboot_header_end-multiboot_header)
MULTIBOOT_CHECKSUM	equ 0xFFFFFFFF-(MULTIBOOT_HEADER_MAGIC + \
	MULTIBOOT_HEADER_LENGTH)

ALIGN 8
multiboot_header:
	dd	MULTIBOOT_HEADER_MAGIC
	dd	MULTIBOOT_HEADER_ARCH
	dd  MULTIBOOT_HEADER_LENGTH
	dd  (MULTIBOOT_CHECKSUM)+1
ALIGN 8
	dw 1,0
	dd 28
	dd 1
	dd 3
	dd 6
	dd 7
	dd 8

	dd 0

	dw 6,0
	dd 12
	dd 0
ALIGN 8
	dw 0,0
	dd 8
multiboot_header_end:

[section .text]
[global start]
;entry point
start:
	cli
	lgdt [trickgdt]
	mov cx, 0x10
	mov ds, cx
	mov es, cx
	mov fs, cx
	mov gs, cx
	mov ss, cx

	jmp 0x08:higherhalf

[EXTERN init_exec]
[EXTERN _init]
higherhalf:
	mov esp, sys_stack
	mov ebp, sys_stack
	add ebx, 0xC0000000 ;move to higher half
	push ebx
	call _init
	call init_exec

	jmp $

[GLOBAL gdt_flush]
[EXTERN gp]
gdt_flush:
	lgdt [gp]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:flush2

flush2:
	ret

[GLOBAL lidt]
	lidt:
	mov eax, [esp+4]
	lidt [eax]
	ret

[SECTION .setup]
 trickgdt:
	dw gdt_end - gdt - 1
	dd gdt
 gdt:
	dd 0, 0
	db 0xFF, 0xFF, 0, 0, 0, 10011010b, 11001111b, 0x40
	db 0xFF, 0xFF, 0, 0, 0, 10010010b, 11001111b, 0x40
gdt_end:

[SECTION .bss]
resb 0x4000
sys_stack: