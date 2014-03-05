/*
 * Copyright 2013 Matthew Fosdick

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

#ifndef STRING_H
#define	STRING_H
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stddef.h>
namespace std {
	cstring strcat(cstring s1,c_cstring s2);
	cstring strcpy(cstring s1,c_cstring s2);
	int    strlen(c_cstring s1);
	bool   strcmp(c_cstring s1,c_cstring s2);
	void  *memmove(void *dest,const void *src,size_t num);
	void  *memset(void *addr,uint8_t value,size_t bytes);
	void  *memset16(uint16_t *addr,uint16_t value,size_t ops);
	void  *memset32(uint32_t *addr,uint32_t value,size_t ops);
}
#endif

