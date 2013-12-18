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


#ifndef __IDT_H
#define __IDT_H
#include "../../define.h"

#define IDT_TYPE_INTRPT 0x0E
#define IDT_TYPE_TRAP   0x0F
#define IDT_TYPE_TASK   0x05
void install_idt(void *handler, int number, int type, int min_ring);

void enable_idt();

#endif
