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
#include <numconv.h>
extern "C" {
	int atoi(const char *str) {
		return std::strtonum(str,(int)0);
	}
	long atol(const char *str) {
		return std::strtonum(str,(long)0);
	}
	long strtol(const char *str, char **endptr,int base) {
		while(isspace(*str))str++;
		long ret=std::strtonum(str,(long)0);
		if(*str=='+'||*str=='-')str++;
		while(isdigit(*str))str++;
		if(endptr)*endptr=(char *)str;
		return ret;
	}
	unsigned long strtoul(const char *str, char **endptr,int base) {
		while(isspace(*str))str++;
		unsigned long ret=std::strtonum(str,(unsigned long)0);
		if(*str=='+'||*str=='-')str++;
		while(isdigit(*str))str++;
		if(endptr)*endptr=(char *)str;
		return ret;
	}
	long long atoll(const char *str) {
		return std::strtonum(str,(long long)0);
	}
	float atof(const char *str) {
		return std::strtofloat(str,(float)0);
	}
	double atod(const char *str) {
		return std::strtofloat(str,(double)0);
	}

	char *itoa(int value,char *str, int base) {
		return std::numtostr(value,str,base,true);
	}
	char *utoa(unsigned int value,char *str, int base) {
		return std::numtostr(value,str,base,true);
	}
	char *ltoa(long value,char *str, int base) {
		return std::numtostr(value,str,base,true);
	}
	char *ultoa(unsigned long value,char *str, int base) {
		return std::numtostr(value,str,base,true);
	}
	char *lltoa(long long value,char *str, int base) {
		return std::numtostr(value,str,base,true);
	}
	char *ulltoa(unsigned long long value,char *str, int base) {
		return std::numtostr(value,str,base,true);
	}
	char *ftoa(float value,char *str, int base) {
		return std::floattostr(value,str,base,true);
	}
	char *dtoa(double value,char *str, int base) {
		return std::floattostr(value,str,base,true);
	}
}
