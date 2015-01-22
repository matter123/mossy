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
#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h>
#include <stdint.h>
double atof(const char *str);
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);

char *itoa(int value,char *str, int base);
char *utoa(unsigned int value,char *str, int base);
char *ltoa(long value,char *str, int base);
char *ultoa(unsigned long value,char *str, int base);
char *lltoa(long long value,char *str, int base);
char *ulltoa(unsigned long long value,char *str, int base);

double strod(const char *str, char **end_ptr); //NO-IMP
float strof(const char *str, char **end_ptr); //NO-IMP
long strol(const char *str, char **end_ptr); //NO-IMP
long double strold(const char *str, char **end_ptr); //NO-IMP
long long stroll(const char *str, char **end_ptr); //NO-IMP
unsigned long stroul(const char *str, char **end_ptr); //NO-IMP
unsigned long long stroull(const char *str, char **end_ptr); //NO-IMP

#define RAND_MAX 1073741824
int rand();
void srand(unsigned int seed);

void *calloc(size_t num, size_t size);
void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr,size_t size);
#ifdef MOSSY
#define BAD_FOR_KERN __attribute((unavailable("you can't use this in kernel space")))
#define BAD_FOR_USER
#else
#define BAD_FOR_KERN
#define BAD_FOR_USER __attribute((unavailable("you can't use this in user space")))
#endif
#ifdef __cplusplus
}
namespace std {
	typedef decltype(nullptr) nullptr_t;
}
#endif
