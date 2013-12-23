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

#include <cstring>

namespace std {
	string strcat(string s1,c_string s2) {
		string s=s1;
		if(*(s1)) {
			while(*(++s1));
		}
		while ( (*s1++ = *s2++) );
		return s;
	}
	string strcpy(string s1,c_string s2) {
		string s=s1;
		while(*(s2))*(s1++)=*(s2++);
		*(s1)='\0';
		return s;
	}
	int    strlen(string s1) {
		int i=0;
		while(*(s1++))i++;
		return i;
	}
	bool   strcmp(c_string s1,c_string s2) {
		while (*s1 && *s1 == *s2 ) {
			++s1;++s2;
		}
		return *(s1) == *(s2);
	}
}
