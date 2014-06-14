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
#include "utf8.h"

int get_char_len(const char *str) {
	unsigned char c=*str;
	//if not a start of char return -1
	if((c&0xC0)==0x80) {
		return -1;
	}
	//number of high one's is total byte count
	uint8_t count=0;
	while((c&0x80)==0x80) {
		count++;
		c=c<<1;
	}
	if(!count) {
		count=1;
	}
	return (count>4?-1:count);
}

char *next_char(const char *str) {
	int skip=get_char_len(str);
	if(skip>0) {
		return (char *)(str+skip);
	}
	return NULL;
}

char *get_next_char(const char *str, size_t count) {
	char *s=(char *)str;
	while(count--) {
		s=next_char(s);
		if(s==NULL) {
			return NULL;
		}
		if(*s=='\0') {
			return s;
		}
	}
	return s;
}

bool is_char_equal(const char *c1, const char *c2) {
	int len=get_char_len(c1);
	if(len!=get_char_len(c2)) {
		return false;
	}
	while(len--) {
		if(*(c1+len)!=*(c2+len)) {
			return false;
		}
	}
	return true;
}

void char_copy(char *str, const char *c) {
	int len=get_char_len(c);
	while(len--) {
		*(str++)=*(c++);
	}
}