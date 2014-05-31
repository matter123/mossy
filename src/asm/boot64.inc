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
[SECTION .MBOOT]
KERNEL_VMA equ 0xFFFFFFFF80000000 ;cant do assembler time magic on a scaler
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
	dd  MULTIBOOT_CHECKSUM+1
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
	gdt_end:
ALIGN 8
	gdtr_64_low:
	dw gdt_end-null_gdt-1
	dq null_gdt+KERNEL_VMA

GDTR_64 equ gdtr_64_low+KERNEL_VMA

ALIGN 4
	gdtr_32:
	dw gdt_end-null_gdt-1
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
pt_1:
resb 0x1000
low_bss_end:

LB_LEN equ (low_bss_end-low_bss_start)/4

[SECTION .low_text exec]
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
no_64:
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
	cmp eax, 0x36D76289
	jne no_mboot

	;check cpu supports AMD64
	mov esi, ebx
	mov eax,0x80000001
	cpuid
	mov ebx, esi
	bt edx,29
	jnc no_64

	;clear low bss
	mov edi, low_bss_start
	mov ecx, LB_LEN
	mov eax, 0
	rep stosd
	;use custom_temp gdt
	lgdt [gdtr_32]
	jmp 0x18:.cont_1

	.cont_1:
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

	mov edi, pt
	xor ecx, ecx
	mov esi, 512

	;id map 2mb, in this setup first 12 bits of KERNEL_LMA and KERNEL_VMA need to be zero
	;KERNEL_VMA must also be the offest from KERNEL_LMA
	.id_map:
		cmp ecx, esi
		je .cont
		mov eax, ecx
		mov edx, 0x1000
		mul edx
		or eax,3
		mov dword [edi+ecx*8],eax
		inc ecx
		jmp .id_map
	.cont:
	;skip 2mb then map the next 2 megabytes
	mov edi, pt_1
	mov ecx, 0
	mov esi, 512
	.id_map_1:
		cmp ecx, esi
		je .cont_2
		mov eax, ecx
		mov edx, 0x1000
		mul edx
		add eax, 0x200000
		or eax,3
		mov dword [edi+ecx*8],eax
		inc ecx
		jmp .id_map_1
	.cont_2:
	mov eax, pt
	or eax,3
	mov [pd],eax
	mov [pd + (((KERNEL_VMA>>21)&511)*8)],eax
	mov eax, pt_1
	or eax, 3
	mov [pd + (((KERNEL_VMA>>21)&511)*8)+8],eax

	mov eax, pd
	or eax,3
	mov [pdpt], eax
	mov [pdpt + (((KERNEL_VMA>>30)&511)*8)], eax

	mov eax, pdpt
	or eax,3
	mov [pml4], eax
	mov [pml4 + (((KERNEL_VMA>>39)&511)*8)], eax
	mov eax, pml4
	mov cr3, eax
	xor eax, eax

	;enable long mode
	mov ecx, 0xC0000080 ;EFER
	rdmsr
	bts eax, 8; Long Mode Enabled
	wrmsr
	mov eax, cr0
	bts eax, 31
	mov cr0, eax
	jmp 0x8:.low_high_conv

	[BITS 64]
	.low_high_conv:

	mov rax, bootstrap_comp
	jmp rax

	[SECTION .text]
	bootstrap_comp:
	mov rax, qword GDTR_64
	lgdt [rax]
	;dont need to reload the *s registers since the GDTR_64 points to the same structure

	;setup stack
	mov rax, qword stack_start
	mov rsp, rax
	mov rbp, rsp

	;turn on FPU
	mov rax, cr0
	and ax, 0xFFFD
	or ax, 0x10
	mov cr0, rax
	fninit

	;turn on SSE / SEE2
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
	mov r12, qword __CTOR_LIST__
	.call_ctor:
		cmp qword [r12], 0
		je .cont
		call [r12]
		add r12, 8
		jmp .call_ctor
	.cont:
	mov rax, rbx
	add rax, KERNEL_VMA
	mov rdi, rax
	call init_exec
	cli ;in case kernel renables it
	mov rdi, 0xB8000
	mov r13, qword shutd_1
	mov rax, [r13]
	mov r13, qword shutd_2
	mov rbx, [r13]
	mov rcx, 250
	.reloop:
		cmp rcx, 0
		je .cont_1
		mov [rdi], rax
		mov [rdi + 8], rbx
		add rdi, 16
		sub rcx,1
		jmp .reloop

	.cont_1:
	hlt

	[GLOBAL lidt]
	lidt:
	lidt [rdi]
	ret

	[SECTION .data]
	fillstr: db 'Y',0xF,'A',0xF,'Y',0xF,'!',0xF
	shutd_1: db 'S',0xF,'H',0xF,'U',0xF,'T',0xF
	shutd_2: db 'D',0xF,'O',0xF,'W',0xF,'N',0xF
	ldtr: dq 0
	[SECTION .bss]
	resb 0x4000
	stack_start: