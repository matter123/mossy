/*
    Copyright 2014 Matthew Fosdick

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#pragma once
#include <arch.h>
namespace kernel {
#ifdef X86
#define syscall_0_0(num) asm volatile("int %0"::"N"(num))
#define syscall_1_0(num,arg1) asm volatile("int %0"::"N"(num),"a"(arg1))
#define syscall_2_0(num,arg1,arg2) asm volatile("int %0"::"N"(num),"a"(arg1),"b"(arg2))
#elif defined X64
#define syscall_0_0(num) asm volatile("int %0"::"N"(num))
#define syscall_1_0(num,arg1) asm volatile("int %0"::"N"(num),"a"(arg1))
#define syscall_2_0(num,arg1,arg2) asm volatile("int %0"::"N"(num),"a"(arg1),"b"(arg2))
#endif
}
