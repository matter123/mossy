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
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
extern "C" {
int sprintf(char *str,const char *fmt, ...) {
	va_list list;
	va_start(list,fmt);
	int len=vsprintf(str,fmt,list);
	va_end(list);
	return len;
}
//copied from mossy1 may be kinda shitty
int vsprintf(char *s, const char *format, va_list arg) {
	volatile bool special=false;
	char cn;
	int i;
	double d;

	unsigned int u;
	char buf[32] = {0};
	while (*format) {
		char *bufcp=buf;
		char c=*format++;
		if (special) {
			switch (c) {
				case 'a':
					d=va_arg(arg,double); //not to break compatiblity
					break;
				case 'A':
					d=va_arg(arg,double); //same as above
					break;
				case 'c':
					cn=(char)va_arg(arg,int);
					*s++=cn;
					break;
				case 'd':
				case 'i':
					i=va_arg(arg,int);
					itoa(i,buf,10);
					while(*bufcp)*s++=*bufcp++;
					break;
				case 'e':
				case 'E':
				case 'f':
				case 'F':
				case 'g':
				case 'G':
					d=va_arg(arg,double); //see a
					break;
				case 'n':
					*s='\0';
					*(va_arg(arg,int *))=strlen(s);
					break;
				case 'o':
					u=va_arg(arg,unsigned int);
					itoa(u,buf,8);
					while(*bufcp)*s++=*bufcp++;
					break;
				case 'p':
					itoa(va_arg(arg,uintptr_t),buf,16);
					while(*bufcp)*s++=*bufcp++;
					break;
				case 's':
					bufcp=va_arg(arg,char *);
					while(*bufcp)*s++=*bufcp++;
					break;
				case 'u':
					u=va_arg(arg,unsigned int);
					break;
				case 'x':
				case 'X':
					u=va_arg(arg,unsigned int);
					itoa(u,buf,16);
					*s++='0';
					*s++='x';
					while(*bufcp)*s++=*bufcp++;
					break;
				case '%':
					*s++='%';
			}
			special=false;
		} else {
			if (c=='%') {
				special=true;
			} else {
				*s++=c;
			}
		}
	}
	*s='\0';
	return strlen(s);
}
}
