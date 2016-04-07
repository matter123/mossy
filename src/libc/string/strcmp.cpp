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
#include <stdlib.h>
#include <string.h>

int strcmp(const char *str1, const char *str2) {
	while(*str1&&*str1==*str2) {
		str1++;
		str2++;
	}
	if(*str1==*str2) {
		return 0;
	}
	return *str1-*str2;
}
int strncmp(const char *str1, const char *str2, size_t num) {
	while(num--) {
		if(*str1&&*str1==*str2) {
			if(!*str1) {
				return 0;
			}
			str1++;
			str2++;
		} else {
			return *str1-*str2;
		}
	}
	return 0;
}
