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
#include <stdlib.h>
//LCG (Linear Congruential Generator)
static unsigned int a=1103515245;
static unsigned int last;
static unsigned int c=12345;
static unsigned int modulus=2147483648;

int rand() {
	unsigned long temp=(a*last+c)%modulus;
	last=temp;
	return temp&0xFFFFFFFF;
}

void srand(unsigned int seed) {
	last=seed;
}
