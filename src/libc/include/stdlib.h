/*
	Copyright 2015 Matthew Fosdick

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
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#if IN_LIBC
NORETURN void abort();
#endif
#if IN_KERNEL
#define __alloca(size) __builtin_alloca (size)
#endif
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);

char *itoa(int value,char *str, int base);
char *utoa(unsigned int value,char *str, int base);
char *ltoa(long value,char *str, int base);
char *ultoa(unsigned long value,char *str, int base);
char *lltoa(long long value,char *str, int base);
char *ulltoa(unsigned long long value,char *str, int base);
#ifdef __cplusplus
}
#endif
