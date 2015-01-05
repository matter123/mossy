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
#define _PC 1
//example setup
#define _a 0
#if VENDOR == _PC
#define PC
#elif VENDOR == _a
#define a
#endif

#ifndef asm
#include <stdint.h>
#ifdef PC

#elif defined a

#endif
#endif

#ifndef DEF_VENDOR
#undef _PC
#endif

