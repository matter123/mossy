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
KERNEL_VMA equ 0xC0000000 ;cant do assembler time magic on a scaler
						  ;value, so keep this updated

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

LB_LEN equ (low_bss_end-low_bss_start)/4

[section .low_text exec]
no_mboot:
	mov eax, 0xB8000
	mov dword [eax], `N\x0FO\xF`
	add eax, 4
	mov dword [eax], ` \x0FM\xF`
	add eax, 4
	mov dword [eax], `B\x0FO\xF`
	add eax, 4
	mov dword [eax], `O\x0FT\xF`
	hlt
[global start]
;entry point
start:
	cli
	cmp eax, 0x36D76289
	jne no_mboot
	;mask PIC
	mov al, 0xFF
	out 0x21, al
	out 0xA1, al
	;acknowledge any impending Interrupts
	mov al, 0x20
	out 0x20, al
	out 0xA0, al
	;wait a bit
	mov ecx, 0x5000
	.wait_loop:
	cmp ecx, 0
	je .cont_wait
	nop
	nop
	nop
	nop
	nop
	nop
	dec ecx
	jmp .wait_loop
	.cont_wait:
	;and finally acknowledge any the came
	out 0x21, al
	out 0xA1, al
	;PIC is finally clean

	lgdt [gdtp]
	jmp 0x08:flush

flush:
	;clean up segment selectors
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov edi, low_bss_start
	mov ecx, LB_LEN
	mov eax, 0
	rep stosd

	mov edi, pt
	xor ecx, ecx
	mov esi, 1024

	;id map 4mb, in this setup first 12 bits of KERNEL_LMA and KERNEL_VMA need to be zero
	;KERNEL_VMA must also be the offest from KERNEL_LMA
	.id_map:
		cmp ecx, esi
		je .cont
		mov eax, ecx
		mov edx, 0x1000
		mul edx
		or eax,3
		mov dword [edi+ecx*4],eax
		inc ecx
		jmp .id_map
	.cont:
	mov eax, pt
	or eax,3
	mov [pd],eax
	mov [pd + ((KERNEL_VMA>>22)*4)],eax

	mov eax, pd
	mov cr3, eax
	mov eax, cr0
	bts eax, 31
	mov cr0, eax

	jmp higherhalf

[section .text]
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

[GLOBAL lidt]
	lidt:
	mov eax, [esp+4]
	lidt [eax]
	ret

[SECTION .low_data alloc noexec write align=4096]
gdtp:
	dw gdt_end - gdt - 1
	dd gdt
gdt:
	;NULL
	dd 0, 0
	;0 DPL CODE
	dw 0xFFFF, 0x0000
	db 0x00, 10011010b, 11001111b, 0x00
	;0 DPL DATA
	dw 0xFFFF, 0x0000
	db 0x00, 10010010b, 11001111b, 0x00
	;3 DPL CODE
	dw 0xFFFF, 0x0000
	db 0x00, 11111010b, 11001111b, 0x00
	;3 DPL DATA
	dw 0xFFFF, 0x0000
	db 0x00, 11110010b, 11001111b, 0x00
gdt_end:

[SECTION .low_bss alloc noexec write nobits align=4096]
low_bss_start:
pd:
resb 0x1000
pt:
resb 0x1000
low_bss_end:

[SECTION .bss]
resb 0x4000
sys_stack: