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

size_t strlen(const char *string) {
	size_t len=0;
	while(*string++)len++;
	return len;
}

size_t strnlen(const char *string, int maxlen) {
	size_t len=0;
	while(*string++&&maxlen--)len++;
	return len;
}
