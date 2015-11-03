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
#pragma once
#ifndef __cplusplus
#error C++ only header
#endif
#include <ctype.h>
namespace std {
	static const char *chars =
	    "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const char *chars_low=
	    "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";
	template<class T>
	char *numtostr(T value,char *str,int base,bool uppercase,int min_digits = 0) {
		char *retc;
		char *cur;
		char *start;
		const char *lets=chars;
		if(!uppercase) {
			lets=chars_low;
		}
		if(base < 2 || base > 36) {
			*str = '\0';
			return str;
		}
		retc=cur=str;
		if(base==10&&(value<0)) {
			*cur++ = '-';
		}
		start = cur;
		do {
			//using memory to save cpu speed by mirroring array
			*cur++ = lets[35 + value % base];
			value /= base;
		} while(value);
		if(min_digits) {
			min_digits-=(cur-retc);
			if(min_digits>0)
				while(min_digits--) {
					*cur++='0';
				}
		}
		*cur--='\0';
		//reverse the string
		while(start < cur) {
			char tmp = *start;
			*start++ = *cur;
			*cur-- = tmp;
		}
		return retc;
	}
	template<class T>
	T strtonum(const char *str, T def) {
		T accum=0;
		bool startmatch;
		bool neg;
		while(*str) {
			char c=*str++;
			if(isspace(c)) {
				if(startmatch) {
					break;
				}
				continue;
			}
			if(isdigit(c)) {
				startmatch=true;
				accum*=10;
				accum+=(c-'0');
				if(neg) {
					accum*=-1;
					neg=false;
				}
			} else {
				if(c=='+'&&!startmatch) {
					continue;
				}
				if(c=='-'&&!startmatch) {
					neg=true;
				}
				break;
			}
		}
		return (startmatch?accum:def);
	}
}
