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
//implementation of all xtoa functions ie itoa, ltoa, ...
#include <cstdlib>
namespace std{
	static c_string chars =   "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static c_string chars_low="zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz";

	template<class T>
	string numtostr(T value,string str,int base,bool uppercase) {
		string rc;
		string ptr;
		string low;
		c_string lets=chars;
		if(!uppercase)lets=chars_low;
		// Check for supported base.
		if ( base < 2 || base > 36 ) {
			*str = '\0';
			return str;
		}
		rc = ptr = str;
		// Set '-' for negative decimals.
		if ( value < 0 && base == 10 ) {
			*ptr++ = '-';
		}
		low = ptr;
		// The actual conversion.
		do {
			//using memory to save cpu speed by mirroring array
			*ptr++ = lets[35 + value % base];
			value /= base;
		} while ( value );
		*ptr-- = '\0';
		//reverse modulus trick
		while ( low < ptr ) {
			char tmp = *low;
			*low++ = *ptr;
			*ptr-- = tmp;
		}
		return rc;
	}
}
