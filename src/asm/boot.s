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
#include "../arch/arch.h"
#ifdef X86
[BITS 32]
[section .mboot]

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

ALIGN 4
multiboot_header:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM

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
#endif



#ifdef X64
[BITS 32]
[SECTION .mboot]
[EXTERN load_start]
[EXTERN load_end]
[EXTERN bss_end]
ALIGN 8
mboothead:
DD 0xE8525D6
DD 0
DD mboot_end-mboothead
;header length is missing from grub
DD -(0xE8525D6 + (mboot_end - mboothead))

;A.OUT KLUDGE
DW 2,0
DD 24
DD mboothead
DD load_start
DD load_end
DD bss_end

;ENTRY POINT
DW 3,0
DD 12
DD start
DD 0

;END OF TAGS
DW 0,0
DD 8
mboot_end:

[SECTION .boot]
[GLOBAL start]
[EXTERN stack]
start:
	cli
	cmp eax, 0x36d76289
	jne .fault
	mov eax, gdtr1 ;setup temp gdtr
	lgdt [eax]
	jmp 0x08:.gdt_ready
.fault:
	jmp $
.gdt_ready:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, stack
	mov ebp, esp
	call setup_paging_long_mode
	mov eax, gdtr2 ;setups lower half gdt
	lgdt [eax]
	jmp 0x08:.gdt2ready

[BITS 64]
;[EXTERN init_exec]
;[EXTERN _init]
.gdt2ready:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov qword rsp, stack + 0xFFFFFFFF80000000
	mov rbp, rsp
	mov qword rax, gdtr3 ;setups higher half gdt
	lgdt [rax]
	add rbx, 0xFFFFFFFF80000000
	mov rax, 'L O N G'
	mov [0x0B8000], rax
	jmp $
	;call _init
	mov rdi, rbx ;AMD64 System V ABI says to pass first argument in rdi
	;add a call to _init here later
	;call init_exec

	jmp $

[EXTERN pml4]
[EXTERN pdpt]
[EXTERN pd]
[BITS 32]
setup_paging_long_mode:
	mov eax, pdpt
	or eax, 1
	mov [pml4 + 0x000], eax ;setup lower half
	mov [pml4 + 0xFF8], eax ;and higher half

	mov eax,pd
	or eax,1
	mov [pdpt], eax
	mov [pdpt + 0xFF0], eax

	;install page tables
	mov dword [pd + 0x00], 0x000083
	mov dword [pd + 0x08], 0x200083
	mov dword [pd + 0x10], 0x400083
	mov dword [pd + 0x18], 0x600083

	;install pml4 and enable PAE
	mov eax, pml4
	mov cr3, eax
	mov eax, cr4
	or eax, 1 << 5
	mov cr4, eax

	;enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, 1 << 31
	wrmsr

	;enable paging
	mov eax, cr0
	or eax, 1 << 31
	mov cr0, eax
	ret

tmp_gdt:
	DQ 0x0000000000000000
	DQ 0x00CF9A000000FFFF
	DQ 0x00CF92000000FFFF
	DQ 0x0000000000000000
	DQ 0x00A09A0000000000
	DQ 0x00A0920000000000
gdtr1:
	DW 23
	DD tmp_gdt
gdtr2:
	DW 23
	DD tmp_gdt + 24
	DD 0
gdtr3:
	DW 23
	DQ tmp_gdt + 24 + 0xFFFFFFFF80000000
#endif