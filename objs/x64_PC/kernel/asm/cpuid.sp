





; Copyright 2013 Matthew Fosdick

; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at

; http:

; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.


[GLOBAL cpuid]
cpuid:
 push rbp
 mov rbp, rsp
 push rbx
 xor rax, rax
 mov rax, rdi
 cpuid
 cmp qword rsi, 0 ;eax
 je .ret
 cmp qword rsi, 1 ;ebx
 je .ret_rbx
 cmp qword rsi, 2 ;ecx
 je .ret_rcx
 mov eax, edx ;edx
 jmp .ret
 .ret_rbx:
  mov eax, ebx
  jmp .ret
 .ret_rcx:
  mov eax, ecx
 .ret:
  pop rbx
  pop rbp
  ret
