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
[GLOBAL IDT_flush]
[EXTERN idtp]
IDT_flush:
    lidt [idtp]
    ret

;place stub handlers here

%macro E_NOERRCODE 1
	[GLOBAL exception%1]
	exception%1:
		push dword 0
		push dword %1
		jmp execpt_common_stub
%endmacro

%macro E_ERRCODE 1
	[GLOBAL exception%1]
	exception%1:
		push dword %1
		jmp execpt_common_stub
%endmacro

%macro E_RESV 1
	[GLOBAL exception%1]
	exception%1:
		push dword %1
		jmp resv_common_stub
%endmacro

%macro PIC 1
	[GLOBAL pic%1]
	pic%1:
		push dword 0 ;keep a same stack frame for both exceptions and pic
		push dword %1
		jmp pic_common_stub
%endmacro

E_NOERRCODE 0
E_NOERRCODE 1
E_NOERRCODE 2
E_NOERRCODE 3
E_NOERRCODE 4
E_NOERRCODE 5
E_NOERRCODE 6
E_NOERRCODE 7
E_ERRCODE   8
E_NOERRCODE 9
E_ERRCODE   10
E_ERRCODE   11
E_ERRCODE   12
E_ERRCODE   13
E_ERRCODE   14
E_RESV      15
E_NOERRCODE 16
E_ERRCODE   17
E_NOERRCODE 18
E_NOERRCODE 19
E_NOERRCODE 20
E_RESV      21
E_RESV      22
E_RESV      23
E_RESV      24
E_RESV      25
E_RESV      26
E_RESV      27
E_RESV      28
E_RESV      29
E_NOERRCODE 30
E_RESV      31
PIC         0
PIC         1
PIC         2
PIC         3
PIC         4
PIC         5
PIC         6
PIC         7
PIC         8
PIC         9
PIC         10
PIC         11
PIC         12
PIC         13
PIC         14
PIC         15

[EXTERN except_handler]
execpt_common_stub:
	pushad
	call except_handler
	popad
	add esp,8
	iret

[EXTERN resv_except_handler]
resv_common_stub:
	pushad
	call resv_except_handler
	popad
	add esp,8
	iret

[EXTERN pic_handle]
pic_common_stub:
	pushad
	call pic_handle
	popad
	add esp,8
	iret
#endif