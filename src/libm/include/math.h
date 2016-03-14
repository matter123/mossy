/*
	Copyright 2016 Matthew Fosdick

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
//good chance implementation of most of these functions is going to be under a different copyright
//i am not by any stretch of the imagination a mathmatician
int abs(int value);
long labs(long value);
long long llabs(long long value);
float fabs(float value);
double dabs(double value);

double cos(double value); //NO-IMP
double sin(double value); //NO-IMP
#ifdef __cplusplus
}
#endif
