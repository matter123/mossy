;  Copyright 2015 Matthew Fosdick

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
[section .MBOOT]
KERNEL_VMA equ 0xFFFFFFFF80000000 ;cant do assembler time magic on a scaler
								  ;value, so keep this updated

MULTIBOOT_HEADER_MAGIC	equ 0xE85250D6
MULTIBOOT_HEADER_ARCH	equ 0
MULTIBOOT_HEADER_LENGTH	equ (multiboot_header_end-multiboot_header)
MULTIBOOT_CHECKSUM	equ 0xFFFFFFFF-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_LENGTH)

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
	dw 0,0
	dd 8
multiboot_header_end:

	null_gdt:
	dq 0
	code_64_gdt:
	dd 0
	db 0x0, 10011010b
	db 00100000b, 0x00
	data_gdt:
	dw 0xFFFF,0x0000
	db 0x00, 10010010b
	db 11001111b, 0x00
	code_32_gdt:
	dw 0xFFFF,0x0000
	db 0x00, 10011010b
	db 11001111b, 0x00
	code_64_user_gdt:
	dd 0
	db 0x0, 10011010b
	db 0x0, 10011010b
	gdt_end:
ALIGN 8
	gdtr_64_low:
	dw gdt_end-null_gdt-1
	dq null_gdt+KERNEL_VMA

GDTR_64 equ gdtr_64_low+KERNEL_VMA

ALIGN 4
	gdtr_32:
	dw (gdt_end-null_gdt)-1
	dd null_gdt

[SECTION .low_bss alloc noexec write nobits align=4096]
low_bss_start:
pml4:
resb 0x1000
pdpt:
resb 0x1000
pd:
resb 0x1000
pt:
resb 0x1000
low_bss_end:
LB_LEN equ (low_bss_end-low_bss_start)/4

[SECTION .low_text exec]
no_mboot: ;prints NO MBOOT if magic number not provided
	mov eax, 0xB8000
	mov dword [eax], `N\x0FO\xF`
	add eax, 4
	mov dword [eax], ` \x0FM\xF`
	add eax, 4
	mov dword [eax], `B\x0FO\xF`
	add eax, 4
	mov dword [eax], `O\x0FT\xF`
	hlt
no_64: ;prints NO 64BIT on x86 pcs
	mov eax, 0xB8000
	mov dword [eax], `N\x0FO\x0F`
	add eax, 4
	mov dword [eax], ` \x0F6\x0F`
	add eax, 4
	mov dword [eax], `4\x0FB\x0F`
	add eax, 4
	mov dword [eax], `I\x0FT\x0F`
	hlt
[GLOBAL start]
start:
	cli

	;check MBOOT
	cmp eax, 0x36d76289
	jne no_mboot
	;check MBOOT
	;AMD64 detection
	xor eax, eax
	mov esi, ebx ;keep ebx safe
	mov eax, 0x80000001
	cpuid
	mov ebx, esi ; restore ebx
	bt edx, 29
	jnc no_64
	;AMD64 detection

	;clear low bss
	mov edi, low_bss_start
	mov ecx, LB_LEN
	mov eax, 0
	rep stosd
	;clear low bss

	lgdt [gdtr_32] ;load own gdtr and farjump
	jmp 0x18:.jump

	.jump: ;using own gdt now
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	;enable PAE
	mov eax, cr4
	bts eax, 5
	mov cr4, eax
	;enable PAE

	;map PT
	mov edi, pt
	xor ecx, ecx
	.id_map:
		cmp ecx, 512
		je .fin
		mov eax, ecx
		mov edx, 0x1000
		mul edx   ;calculate addr
		or eax, 3 ;set present and writable
		mov dword [edi+ecx*8], eax
		inc ecx
		jmp .id_map
	.fin:
	or edi, 3 ;map pt
	mov [pd],edi
	mov [pd + (((KERNEL_VMA>>21)&511)*8)], edi
	mov edi, pd ;map pd
	or edi, 3
	mov [pdpt],edi;
	mov [pdpt + (((KERNEL_VMA>>30)&511)*8)], edi
	mov edi, pdpt ; map pdpt
	or edi, 3
	mov [pml4],edi
	mov [pml4 + (((KERNEL_VMA>>39)&511)*8)], edi

	mov edi, pml4 ;recursive map pml4
	or edi, 3
	mov [pml4 + 510*8], edi

	mov edi, pml4
	mov cr3, edi

	;enable long mode
	mov ecx, 0xC0000080 ;EFER
	rdmsr
	bts eax, 8
	wrmsr
	;enable paging
	mov eax, cr0
	bts eax, 31
	mov cr0, eax

	jmp 0x8:.go_high

	[BITS 64]
	.go_high:
	mov rax, long_mode
	jmp rax

[SECTION .text]
long_mode:
	mov rax, qword GDTR_64
	lgdt[rax]
	mov rax, sys_stack
	mov rsp, rax

	mov rax, cr0
	and ax, 0xFFFD
	or ax, 0x10
	mov cr0, rax
	fninit

	mov rax, cr0
	and ax, 0xFFFB
	or ax, 0x2
	mov cr0, rax
	mov rax, cr4
	or ax, 3 << 9
	mov cr4, rax
	jmp load_kernel

	[EXTERN init_exec]
	[EXTERN __CTOR_LIST__]
	load_kernel:
	;when get good random use it hear to update __stack_chk_guard
	mov r12, qword __CTOR_LIST__
	.call_ctor:
		cmp qword [r12], 0
		je .cont
		call [r12]
		add r12, 8
		jmp .call_ctor
	.cont:
	mov rdi, rbx
	add rdi, KERNEL_VMA
	call init_exec
	jmp $


[GLOBAL lidt]
lidt:
	lidt [rdi]
	ret
[SECTION .bss]
resb 0x4000
[GLOBAL sys_stack]
sys_stack:
[SECTION .data]
[GLOBAL __stack_chk_guard]
__stack_chk_guard DQ 0xD5000AFF0DCD12D5
