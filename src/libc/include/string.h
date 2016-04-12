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
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
size_t strlen(const char *);
size_t strnlen(const char *, int maxlen);
void *memset(void *dest,int value,size_t size);
int   memcmp(const void *mem1,const void *mem2,size_t num);
void *memcpy(void *dest,const void *src,size_t size);
void *memmove(void *dest,const void *src,size_t size);
char *strcat(char *dest,const char *src);
char *strcpy(char *dest,const char *src);
int   strcmp(const char *dest,const char *src);
char *strncpy(char *dest,const char *src, size_t n);
int   strncmp(const char *dest,const char *src, size_t n);
char *strpbrk(char *str1, const char *str2);
#ifdef __cplusplus
}
#endif
