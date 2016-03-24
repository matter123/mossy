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
#include <math.h>
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
		bool startmatch=false;
		bool neg=false;
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
			} else {
				if(c=='+'&&!startmatch) {
					continue;
				} else if(c=='-'&&!startmatch) {
					neg=true;
				} else break;
			}
		}
		if(neg) {
			accum*=-1;
		}
		return (startmatch?accum:def);
	}
	//doesnt use e notation
	//not perfectly accurate, does not use big number so cant be perfectly accurate
	template<class T>
	char *floattostr(T value,char *str,int base,bool uppercase,int min_digits = 0) {
		PANIC("math functions needed for foattostr dont exist yet");
		char *retc=str;
		int firstExp=0;
		bool havePrinted=false;
		T digit=0;
		if(value < 0) {
			*str++='-';
			value*=-1;
		}
		firstExp=(int)floor(log10f(value));
		//value/=pow(10,firstExp);
		while(value>0) {
			if(firstExp==0) {
				if(!havePrinted) {
					*str++='0';
				}
				*str++='.';
			}
			havePrinted=true;
			firstExp--;
			//digit=floor(value);
			*str++='0'+(char)digit;
			value-=digit;
			value*=10;
		}
		return retc;
	}
	//doesn't handle e notation
	template<class T>
	T strtofloat(const char *str,T def) {
		T integerPart=0;
		T fractionPart=0;
		int divisor=0;
		bool startmatch=false;
		bool neg=false;
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
				if(divisor==0) {
					integerPart*=10;
					integerPart+=(c-'0');
				} else {
					fractionPart*=10;
					fractionPart+=(c-'0');
					divisor *=10;
				}
			} else {
				if(c=='+'&&!startmatch) {
					continue;
				} else if(c=='-'&&!startmatch) {
					neg=true;
				} else if(c=='.'&&divisor==0) {
					divisor=1;
				} else break;
			}
		}
		if(divisor==0)divisor=1;
		T accum=integerPart + fractionPart / divisor;
		if(neg) {
			accum*=-1;
		}
		return (startmatch?accum:def);
	}
}
