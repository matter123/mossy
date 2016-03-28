;  Copyright 2016 Matthew Fosdick

;  Licensed under the Apache License, Version 2.0 (the "License");
;  you may not use this file except in compliance with the License.
;  You may obtain a copy of the License at

;    http://www.apache.org/licenses/LICENSE-2.0

;  Unless required by applicable law or agreed to in writing, software
;  distributed under the License is distributed on an "AS IS" BASIS,
;  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;  See the License for the specific
[section .bss]
[GLOBAL IDT]
ALIGN 4096
IDT:
resb 256*16
ALIGN 16
sse_save:
resb 512
in_use:
resb 4


[section .data]
[GLOBAL jump_table]
[GLOBAL jump_table2]
[GLOBAL IDTR]
jump_table:
times 256 dq fail_fast
jump_table2:
times 256 dq fail_fast
IDTR:
dw (256*16) - 1
dq IDT

[section .text]
;global execption handlers need to call pop rax as first instruction
%macro EXCEPT_C 2
[GLOBAL exc%1]
exc%1:
	;CPL change forces ss to be zero check later if an issue
	push qword %1
	;push qword %2
	;push friendly name
	push qword 0
	push rax
	mov rax, %2
	mov [rsp+8], rax
	;call JT1 handler
	mov rax, [qword jump_table + %1 * 8]
	jmp rax
%endmacro

%macro EXCEPT_NC 2
[GLOBAL exc%1]
exc%1:
	push qword 0
	push qword %1
	;push qword %2
	;push friendly name
	push qword 0
	push rax
	mov rax, %2
	mov [rsp+8], rax
	;call JT1 handler
	mov rax, [qword jump_table + %1 * 8]
	jmp rax
%endmacro

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
push 0

%endmacro

%macro pop_x64 0
add rsp, 40
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
[GLOBAL lidtr]
lidtr:
	mov rax, IDTR
	lidt [rax]
	ret

[GLOBAL fail_fast]
fail_fast:
hlt
xchg bx, bx
jmp .past
	.flidtr:
	dw 0
	dq 0
	.past:
	mov rax, .flidtr ;load fake lidt
	;lidt [rax]
	xchg bx, bx ;magic break
	mov bx, 0
	div bx ;div by 0 - triple fault
	iretq ;dont drop off

[GLOBAL def_handler]
def_handler:
	pop rax
	push_x64
	;save sse registers
	mov rax, [qword in_use]
	test rax, rax
	jnz .cont
	mov rax, 1
	mov [qword in_use], rax
	mov rax, qword sse_save
	fxsave [rax]
	;call handler in JT2
	.cont:
	mov rax, [rsp + 23 * 8]
	mov rcx, 8
	mul rcx
	mov rbx, qword jump_table2 ; load function call
	add rax, rbx
	mov rdi, rsp
	mov rsi, sse_save
	mov rdx, in_use
	call [rax]
	;restore sse registers
	mov rax, qword sse_save
	fxrstor [rax]
	mov rax, 0
	mov [qword in_use], rax
	;return from interrupt here is where we would call get_next if so desired
	pop_x64
	add rsp, 24
	iretq

[GLOBAL C0_handler]
[EXTERN get_next]
C0_handler:
	pop rax
	push_x64
	mov rdi, rsp
	call get_next
	mov rsp, rax
	pop_x64
	add rsp, 24
	iretq
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
EXCEPT_NC 32, e20
EXCEPT_NC 33, e21
EXCEPT_NC 34, e22
EXCEPT_NC 35, e23
EXCEPT_NC 36, e24
EXCEPT_NC 37, e25
EXCEPT_NC 38, e26
EXCEPT_NC 39, e27
EXCEPT_NC 40, e28
EXCEPT_NC 41, e29
EXCEPT_NC 42, e2A
EXCEPT_NC 43, e2B
EXCEPT_NC 44, e2C
EXCEPT_NC 45, e2D
EXCEPT_NC 46, e2E
EXCEPT_NC 47, e2F
EXCEPT_NC 48, e30
EXCEPT_NC 49, e31
EXCEPT_NC 50, e32
EXCEPT_NC 51, e33
EXCEPT_NC 52, e34
EXCEPT_NC 53, e35
EXCEPT_NC 54, e36
EXCEPT_NC 55, e37
EXCEPT_NC 56, e38
EXCEPT_NC 57, e39
EXCEPT_NC 58, e3A
EXCEPT_NC 59, e3B
EXCEPT_NC 60, e3C
EXCEPT_NC 61, e3D
EXCEPT_NC 62, e3E
EXCEPT_NC 63, e3F
EXCEPT_NC 64, e40
EXCEPT_NC 65, e41
EXCEPT_NC 66, e42
EXCEPT_NC 67, e43
EXCEPT_NC 68, e44
EXCEPT_NC 69, e45
EXCEPT_NC 70, e46
EXCEPT_NC 71, e47
EXCEPT_NC 72, e48
EXCEPT_NC 73, e49
EXCEPT_NC 74, e4A
EXCEPT_NC 75, e4B
EXCEPT_NC 76, e4C
EXCEPT_NC 77, e4D
EXCEPT_NC 78, e4E
EXCEPT_NC 79, e4F
EXCEPT_NC 80, e50
EXCEPT_NC 81, e51
EXCEPT_NC 82, e52
EXCEPT_NC 83, e53
EXCEPT_NC 84, e54
EXCEPT_NC 85, e55
EXCEPT_NC 86, e56
EXCEPT_NC 87, e57
EXCEPT_NC 88, e58
EXCEPT_NC 89, e59
EXCEPT_NC 90, e5A
EXCEPT_NC 91, e5B
EXCEPT_NC 92, e5C
EXCEPT_NC 93, e5D
EXCEPT_NC 94, e5E
EXCEPT_NC 95, e5F
EXCEPT_NC 96, e60
EXCEPT_NC 97, e61
EXCEPT_NC 98, e62
EXCEPT_NC 99, e63
EXCEPT_NC 100, e64
EXCEPT_NC 101, e65
EXCEPT_NC 102, e66
EXCEPT_NC 103, e67
EXCEPT_NC 104, e68
EXCEPT_NC 105, e69
EXCEPT_NC 106, e6A
EXCEPT_NC 107, e6B
EXCEPT_NC 108, e6C
EXCEPT_NC 109, e6D
EXCEPT_NC 110, e6E
EXCEPT_NC 111, e6F
EXCEPT_NC 112, e70
EXCEPT_NC 113, e71
EXCEPT_NC 114, e72
EXCEPT_NC 115, e73
EXCEPT_NC 116, e74
EXCEPT_NC 117, e75
EXCEPT_NC 118, e76
EXCEPT_NC 119, e77
EXCEPT_NC 120, e78
EXCEPT_NC 121, e79
EXCEPT_NC 122, e7A
EXCEPT_NC 123, e7B
EXCEPT_NC 124, e7C
EXCEPT_NC 125, e7D
EXCEPT_NC 126, e7E
EXCEPT_NC 127, e7F
EXCEPT_NC 128, e80
EXCEPT_NC 129, e81
EXCEPT_NC 130, e82
EXCEPT_NC 131, e83
EXCEPT_NC 132, e84
EXCEPT_NC 133, e85
EXCEPT_NC 134, e86
EXCEPT_NC 135, e87
EXCEPT_NC 136, e88
EXCEPT_NC 137, e89
EXCEPT_NC 138, e8A
EXCEPT_NC 139, e8B
EXCEPT_NC 140, e8C
EXCEPT_NC 141, e8D
EXCEPT_NC 142, e8E
EXCEPT_NC 143, e8F
EXCEPT_NC 144, e90
EXCEPT_NC 145, e91
EXCEPT_NC 146, e92
EXCEPT_NC 147, e93
EXCEPT_NC 148, e94
EXCEPT_NC 149, e95
EXCEPT_NC 150, e96
EXCEPT_NC 151, e97
EXCEPT_NC 152, e98
EXCEPT_NC 153, e99
EXCEPT_NC 154, e9A
EXCEPT_NC 155, e9B
EXCEPT_NC 156, e9C
EXCEPT_NC 157, e9D
EXCEPT_NC 158, e9E
EXCEPT_NC 159, e9F
EXCEPT_NC 160, eA0
EXCEPT_NC 161, eA1
EXCEPT_NC 162, eA2
EXCEPT_NC 163, eA3
EXCEPT_NC 164, eA4
EXCEPT_NC 165, eA5
EXCEPT_NC 166, eA6
EXCEPT_NC 167, eA7
EXCEPT_NC 168, eA8
EXCEPT_NC 169, eA9
EXCEPT_NC 170, eAA
EXCEPT_NC 171, eAB
EXCEPT_NC 172, eAC
EXCEPT_NC 173, eAD
EXCEPT_NC 174, eAE
EXCEPT_NC 175, eAF
EXCEPT_NC 176, eB0
EXCEPT_NC 177, eB1
EXCEPT_NC 178, eB2
EXCEPT_NC 179, eB3
EXCEPT_NC 180, eB4
EXCEPT_NC 181, eB5
EXCEPT_NC 182, eB6
EXCEPT_NC 183, eB7
EXCEPT_NC 184, eB8
EXCEPT_NC 185, eB9
EXCEPT_NC 186, eBA
EXCEPT_NC 187, eBB
EXCEPT_NC 188, eBC
EXCEPT_NC 189, eBD
EXCEPT_NC 190, eBE
EXCEPT_NC 191, eBF
EXCEPT_NC 192, eC0
EXCEPT_NC 193, eC1
EXCEPT_NC 194, eC2
EXCEPT_NC 195, eC3
EXCEPT_NC 196, eC4
EXCEPT_NC 197, eC5
EXCEPT_NC 198, eC6
EXCEPT_NC 199, eC7
EXCEPT_NC 200, eC8
EXCEPT_NC 201, eC9
EXCEPT_NC 202, eCA
EXCEPT_NC 203, eCB
EXCEPT_NC 204, eCC
EXCEPT_NC 205, eCD
EXCEPT_NC 206, eCE
EXCEPT_NC 207, eCF
EXCEPT_NC 208, eD0
EXCEPT_NC 209, eD1
EXCEPT_NC 210, eD2
EXCEPT_NC 211, eD3
EXCEPT_NC 212, eD4
EXCEPT_NC 213, eD5
EXCEPT_NC 214, eD6
EXCEPT_NC 215, eD7
EXCEPT_NC 216, eD8
EXCEPT_NC 217, eD9
EXCEPT_NC 218, eDA
EXCEPT_NC 219, eDB
EXCEPT_NC 220, eDC
EXCEPT_NC 221, eDD
EXCEPT_NC 222, eDE
EXCEPT_NC 223, eDF
EXCEPT_NC 224, eE0
EXCEPT_NC 225, eE1
EXCEPT_NC 226, eE2
EXCEPT_NC 227, eE3
EXCEPT_NC 228, eE4
EXCEPT_NC 229, eE5
EXCEPT_NC 230, eE6
EXCEPT_NC 231, eE7
EXCEPT_NC 232, eE8
EXCEPT_NC 233, eE9
EXCEPT_NC 234, eEA
EXCEPT_NC 235, eEB
EXCEPT_NC 236, eEC
EXCEPT_NC 237, eED
EXCEPT_NC 238, eEE
EXCEPT_NC 239, eEF
EXCEPT_NC 240, eF0
EXCEPT_NC 241, eF1
EXCEPT_NC 242, eF2
EXCEPT_NC 243, eF3
EXCEPT_NC 244, eF4
EXCEPT_NC 245, eF5
EXCEPT_NC 246, eF6
EXCEPT_NC 247, eF7
EXCEPT_NC 248, eF8
EXCEPT_NC 249, eF9
EXCEPT_NC 250, eFA
EXCEPT_NC 251, eFB
EXCEPT_NC 252, eFC
EXCEPT_NC 253, eFD
EXCEPT_NC 254, eFE
EXCEPT_NC 255, eFF
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
e20: db "#ISR20",0x0
e21: db "#ISR21",0x0
e22: db "#ISR22",0x0
e23: db "#ISR23",0x0
e24: db "#ISR24",0x0
e25: db "#ISR25",0x0
e26: db "#ISR26",0x0
e27: db "#ISR27",0x0
e28: db "#ISR28",0x0
e29: db "#ISR29",0x0
e2A: db "#ISR2A",0x0
e2B: db "#ISR2B",0x0
e2C: db "#ISR2C",0x0
e2D: db "#ISR2D",0x0
e2E: db "#ISR2E",0x0
e2F: db "#ISR2F",0x0
e30: db "#ISR30",0x0
e31: db "#ISR31",0x0
e32: db "#ISR32",0x0
e33: db "#ISR33",0x0
e34: db "#ISR34",0x0
e35: db "#ISR35",0x0
e36: db "#ISR36",0x0
e37: db "#ISR37",0x0
e38: db "#ISR38",0x0
e39: db "#ISR39",0x0
e3A: db "#ISR3A",0x0
e3B: db "#ISR3B",0x0
e3C: db "#ISR3C",0x0
e3D: db "#ISR3D",0x0
e3E: db "#ISR3E",0x0
e3F: db "#ISR3F",0x0
e40: db "#ISR40",0x0
e41: db "#ISR41",0x0
e42: db "#ISR42",0x0
e43: db "#ISR43",0x0
e44: db "#ISR44",0x0
e45: db "#ISR45",0x0
e46: db "#ISR46",0x0
e47: db "#ISR47",0x0
e48: db "#ISR48",0x0
e49: db "#ISR49",0x0
e4A: db "#ISR4A",0x0
e4B: db "#ISR4B",0x0
e4C: db "#ISR4C",0x0
e4D: db "#ISR4D",0x0
e4E: db "#ISR4E",0x0
e4F: db "#ISR4F",0x0
e50: db "#ISR50",0x0
e51: db "#ISR51",0x0
e52: db "#ISR52",0x0
e53: db "#ISR53",0x0
e54: db "#ISR54",0x0
e55: db "#ISR55",0x0
e56: db "#ISR56",0x0
e57: db "#ISR57",0x0
e58: db "#ISR58",0x0
e59: db "#ISR59",0x0
e5A: db "#ISR5A",0x0
e5B: db "#ISR5B",0x0
e5C: db "#ISR5C",0x0
e5D: db "#ISR5D",0x0
e5E: db "#ISR5E",0x0
e5F: db "#ISR5F",0x0
e60: db "#ISR60",0x0
e61: db "#ISR61",0x0
e62: db "#ISR62",0x0
e63: db "#ISR63",0x0
e64: db "#ISR64",0x0
e65: db "#ISR65",0x0
e66: db "#ISR66",0x0
e67: db "#ISR67",0x0
e68: db "#ISR68",0x0
e69: db "#ISR69",0x0
e6A: db "#ISR6A",0x0
e6B: db "#ISR6B",0x0
e6C: db "#ISR6C",0x0
e6D: db "#ISR6D",0x0
e6E: db "#ISR6E",0x0
e6F: db "#ISR6F",0x0
e70: db "#ISR70",0x0
e71: db "#ISR71",0x0
e72: db "#ISR72",0x0
e73: db "#ISR73",0x0
e74: db "#ISR74",0x0
e75: db "#ISR75",0x0
e76: db "#ISR76",0x0
e77: db "#ISR77",0x0
e78: db "#ISR78",0x0
e79: db "#ISR79",0x0
e7A: db "#ISR7A",0x0
e7B: db "#ISR7B",0x0
e7C: db "#ISR7C",0x0
e7D: db "#ISR7D",0x0
e7E: db "#ISR7E",0x0
e7F: db "#ISR7F",0x0
e80: db "#ISR80",0x0
e81: db "#ISR81",0x0
e82: db "#ISR82",0x0
e83: db "#ISR83",0x0
e84: db "#ISR84",0x0
e85: db "#ISR85",0x0
e86: db "#ISR86",0x0
e87: db "#ISR87",0x0
e88: db "#ISR88",0x0
e89: db "#ISR89",0x0
e8A: db "#ISR8A",0x0
e8B: db "#ISR8B",0x0
e8C: db "#ISR8C",0x0
e8D: db "#ISR8D",0x0
e8E: db "#ISR8E",0x0
e8F: db "#ISR8F",0x0
e90: db "#ISR90",0x0
e91: db "#ISR91",0x0
e92: db "#ISR92",0x0
e93: db "#ISR93",0x0
e94: db "#ISR94",0x0
e95: db "#ISR95",0x0
e96: db "#ISR96",0x0
e97: db "#ISR97",0x0
e98: db "#ISR98",0x0
e99: db "#ISR99",0x0
e9A: db "#ISR9A",0x0
e9B: db "#ISR9B",0x0
e9C: db "#ISR9C",0x0
e9D: db "#ISR9D",0x0
e9E: db "#ISR9E",0x0
e9F: db "#ISR9F",0x0
eA0: db "#ISRA0",0x0
eA1: db "#ISRA1",0x0
eA2: db "#ISRA2",0x0
eA3: db "#ISRA3",0x0
eA4: db "#ISRA4",0x0
eA5: db "#ISRA5",0x0
eA6: db "#ISRA6",0x0
eA7: db "#ISRA7",0x0
eA8: db "#ISRA8",0x0
eA9: db "#ISRA9",0x0
eAA: db "#ISRAA",0x0
eAB: db "#ISRAB",0x0
eAC: db "#ISRAC",0x0
eAD: db "#ISRAD",0x0
eAE: db "#ISRAE",0x0
eAF: db "#ISRAF",0x0
eB0: db "#ISRB0",0x0
eB1: db "#ISRB1",0x0
eB2: db "#ISRB2",0x0
eB3: db "#ISRB3",0x0
eB4: db "#ISRB4",0x0
eB5: db "#ISRB5",0x0
eB6: db "#ISRB6",0x0
eB7: db "#ISRB7",0x0
eB8: db "#ISRB8",0x0
eB9: db "#ISRB9",0x0
eBA: db "#ISRBA",0x0
eBB: db "#ISRBB",0x0
eBC: db "#ISRBC",0x0
eBD: db "#ISRBD",0x0
eBE: db "#ISRBE",0x0
eBF: db "#ISRBF",0x0
eC0: db "#YIELD",0x0
eC1: db "#ISRC1",0x0
eC2: db "#ISRC2",0x0
eC3: db "#ISRC3",0x0
eC4: db "#ISRC4",0x0
eC5: db "#ISRC5",0x0
eC6: db "#ISRC6",0x0
eC7: db "#ISRC7",0x0
eC8: db "#ISRC8",0x0
eC9: db "#ISRC9",0x0
eCA: db "#ISRCA",0x0
eCB: db "#ISRCB",0x0
eCC: db "#ISRCC",0x0
eCD: db "#ISRCD",0x0
eCE: db "#ISRCE",0x0
eCF: db "#ISRCF",0x0
eD0: db "#ISRD0",0x0
eD1: db "#ISRD1",0x0
eD2: db "#ISRD2",0x0
eD3: db "#ISRD3",0x0
eD4: db "#ISRD4",0x0
eD5: db "#ISRD5",0x0
eD6: db "#ISRD6",0x0
eD7: db "#ISRD7",0x0
eD8: db "#ISRD8",0x0
eD9: db "#ISRD9",0x0
eDA: db "#ISRDA",0x0
eDB: db "#ISRDB",0x0
eDC: db "#ISRDC",0x0
eDD: db "#ISRDD",0x0
eDE: db "#ISRDE",0x0
eDF: db "#ISRDF",0x0
eE0: db "#ISRE0",0x0
eE1: db "#ISRE1",0x0
eE2: db "#ISRE2",0x0
eE3: db "#ISRE3",0x0
eE4: db "#ISRE4",0x0
eE5: db "#ISRE5",0x0
eE6: db "#ISRE6",0x0
eE7: db "#ISRE7",0x0
eE8: db "#ISRE8",0x0
eE9: db "#ISRE9",0x0
eEA: db "#ISREA",0x0
eEB: db "#ISREB",0x0
eEC: db "#ISREC",0x0
eED: db "#ISRED",0x0
eEE: db "#ISREE",0x0
eEF: db "#ISREF",0x0
eF0: db "#ISRF0",0x0
eF1: db "#ISRF1",0x0
eF2: db "#ISRF2",0x0
eF3: db "#ISRF3",0x0
eF4: db "#ISRF4",0x0
eF5: db "#ISRF5",0x0
eF6: db "#ISRF6",0x0
eF7: db "#ISRF7",0x0
eF8: db "#ISRF8",0x0
eF9: db "#ISRF9",0x0
eFA: db "#ISRFA",0x0
eFB: db "#ISRFB",0x0
eFC: db "#ISRFC",0x0
eFD: db "#ISRFD",0x0
eFE: db "#ISRFE",0x0
eFF: db "#ISRFF",0x0
