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
#include <stdlib.h>
namespace std {
	namespace TC {
		extern char RED[];
		extern char BLUE[];
		extern char GREEN[];
		extern char YELLOW[];
		extern char MAGENTA[];
		extern char CYAN[];
		extern char WHITE[];
		extern char BLACK[];

		extern char DRED[];
		extern char DBLUE[];
		extern char DGREEN[];
		extern char DYELLOW[];
		extern char DMAGENTA[];
		extern char DCYAN[];
		extern char DGRAY[];
		extern char LGRAY[];
		extern char BACKCOLOR[];
		extern char TRANSPARENT[];
		extern char RESET[];

		char *getColor(uint8_t red, uint8_t green, uint8_t blue);
	}
}
