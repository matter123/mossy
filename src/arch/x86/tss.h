/*
 * Copyright 2014 Matthew Fosdick

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

#include <stdint.h>
#include <struct.h>

struct TSS {
	uint16_t LINK;
	uint16_t resv_0;
	uint32_t ESP0;
	uint16_t SS0;
	uint16_t resv_1;
	uint32_t ESP1;
	uint16_t SS1;
	uint16_t resv_2;
	uint32_t ESP2;
	uint16_t SS2;
	uint16_t resv_3;
	uint32_t CR3;
	uint32_t EIP;
	uint32_t EFLAGS;
	uint32_t EAX;
	uint32_t ECX;
	uint32_t EDX;
	uint32_t EBX;
	uint32_t ESP;
	uint32_t EBP;
	uint32_t ESI;
	uint32_t EDI;
	uint16_t ES;
	uint16_t resv_4;
	uint16_t CS;
	uint16_t resv_5;
	uint16_t DS;
	uint16_t resv_6;
	uint16_t FS;
	uint16_t resv_4;
	uint16_t GS;
	uint16_t resv_5;
	uint16_t LDTR;
	uint16_t resv_6;
	uint16_t resv_7;
	uint16_t IOBP;
};