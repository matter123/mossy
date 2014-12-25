/*
    Copyright 2013 Matthew Fosdick

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
#include <ctype.h>
extern "C" {
	int isalnum(int c) {
		return isalpha(c)||isdigit(c);
	}
	int isalpha(int c) {
		return (c>='A'&&c<='Z')||(c>='a'&&c<='z');
	}
	int isblank(int c) {
		return (c==' ')||(c=='\t');
	}
	int iscntrl(int c) {
		return (c<20)||(c==127);
	}
	int isdigit(int c) {
		return c>='0'&&c<='9';
	}
	int isgraph(int c) {
		return (isprint(c))&&(c!=20);
	}
	int islower(int c) {
		return (c>='a'&&c<='z');
	}
	int isprint(int c) {
		return (c>=20)&&(c!=127);
	}
	int ispunct(int c) {
		return (c>=0x21&&c<=0x2F)||(c>=0x3A&&c<=0x40)||(c>=0x5B&&c<=0x60)
		       ||(c>=0x7B&&c<=0x7E);
	}
	int isspace(int c) {
		return (c==' '||c=='\t')||(c>=0xA&&c<=0xD);
	}
	int isupper(int c) {
		return (c>='A'&&c<='Z');
	}
	int isxdigit(int c) {
		return isdigit(c)||(c>='A'&&c<='F')||(c>='a'&&c<='f');
	}

	int tolower(int c) {
		if(!isupper(c)) {
			return c;
		}
		return c+('a'-'A');
	}
	int toupper(int c) {
		if(!islower(c)) {
			return c;
		}
		return c-('a'-'A');
	}
}