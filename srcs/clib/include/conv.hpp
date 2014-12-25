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
//implementation of all xtoa functions ie itoa, ltoa, ...
#include <ctype.h>
namespace std {
	static const char *chars =
	    "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static const char *chars_low=
	    "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

	////////////////////////////////////////
	// Converts 'value' into a string and //
	//   copies the string onto 'str'     //
	// 'Base' what base (2,8,10,26,etc)   //
	//   to use when converting           //
	// 'Uppercase' if the base used has   //
	//   letters, should uppercase?       //
	// 'Min_digits' minimum number of     //
	//   digits to have in the string     //
	// Returns 'str'                      //
	// Created on 2014-05-30              //
	////////////////////////////////////////
	template<class T>
	char *numtostr(T value,char *str,int base,bool uppercase,int min_digits = 0) {
		char *rc;
		char *ptr;
		char *low;
		const char *lets=chars;
		if(!uppercase) {
			lets=chars_low;
		}
		if(base < 2 || base > 36) {
			*str = '\0';
			return str;
		}
		rc=ptr=str;
		if(base==10&&(value<0)) {
			*ptr++ = '-';
		}
		low = ptr;
		do {
			//using memory to save cpu speed by mirroring array
			*ptr++ = lets[35 + value % base];
			value /= base;
		} while(value);
		if(min_digits) {
			min_digits-=(ptr-rc);
			if(min_digits>0)
				while(min_digits--) {
					*ptr++='0';
				}
		}
		*ptr--='\0';
		//reverse the string
		while(low < ptr) {
			char tmp = *low;
			*low++ = *ptr;
			*ptr-- = tmp;
		}
		return rc;
	}
	////////////////////////////////////////
	// Converts 'str' into a base 10      //
	//   number, or 'def' if can't find   //
	//   a digit in the 'str'             //
	// 'Def' number used when no match    //
	//   was found                        //
	// Returns the converted number or    //
	//   'def' if no match was found      //
	// Created on 2014-05-30              //
	////////////////////////////////////////
	template<class T>
	T strtonum(const char *str, T def) {
		T accum=0;
		bool ac;
		bool neg;
		while(*str) {
			char c=*str++;
			if(isspace(c)) {
				if(ac) {
					break;
				}
				continue;
			}
			if(isdigit(c)) {
				ac=true;
				accum*=10;
				accum+=(c-'0');
				if(neg) {
					accum*=-1;
					neg=false;
				}
			} else {
				if(c=='+'&&!ac) {
					continue;
				}
				if(c=='-'&&!ac) {
					neg=true;
				}
				break;
			}
		}
		return (ac?accum:def);
	}
}