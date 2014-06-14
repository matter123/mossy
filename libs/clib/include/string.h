/*
    Copyright 2013 Matthew Fosdick

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
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
void *memcpy(void *dest, const void *src, size_t num); //WORKS,UTF8
void *memmove(void *dest, const void *src, size_t num); //WORKS,UTF8
char *strcpy(char *dest, const char *src); //WORKS,UTF8
char *strncpy(char *dest, const char *src, size_t num); //WORKS,UTF8
char *strlcpy(char *dest, const char *src, size_t num); //WORKS,UTF8
char *mbsncpy(char *dest, const char *src, size_t num); //WORKS,UTF8

char *strcat(char *dest, const char *src); //WORKS
char *strncat(char *dest, const char *src, size_t num); //WORKS
char *strlcat(char *dest, const char *src, size_t num);
char *mbsncat(char *dest, const char *src, size_t num);

int memcmp(const void *ptr1, const void *ptr2, size_t num); //WORKS
int strcmp(const char *str1, const char *ptr2); //WORKS
int strncmp(const char *str1, const char *str2, size_t num); //WORKS
int mbsncmp(const char *str1, const char *str2, size_t num); //WORKS

void *memchr(const void *ptr, int value, size_t num); //WORKS
char *strchr(const char *str, int value); //WORKS
size_t strcspn(const char *str1, const char *str2); //WORKS
char *strpbrk(const char *str1, const char *str2); //WORKS
char *strrchr(char *str1, int value); //WORKS
size_t strspn(const char *str1, const char *str2); //WORKS
char *strstr(char *str1, const char *str2); //WORKS
char *strtok(char *str, const char *delim); //WORKS

void *memset(void *ptr,int value,size_t num); //WORKS
void *memset16(uint16_t *ptr,uint16_t value,size_t num); //WORKS
void *memset32(uint32_t *ptr,uint32_t value,size_t num); //WORKS
void *memset64(uint64_t *ptr,uint64_t value,size_t num); //WORKS
size_t strlen(const char *str); //WORKS
size_t mbslen(const char *str); //WORKS
#ifdef __cplusplus
}
#endif