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
#include <stdlib.h>
#include "conv.hpp"
extern "C" {
	//double atof(const char *str); //NO-IMP
	int atoi(const char *str) {
		return std::strtonum(str,0);
	}
	long atol(const char *str) {
		return std::strtonum(str,(long)0);
	}
	long long atoll(const char *str) {
		return std::strtonum(str,(long long)0);
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

	//double strod(const char *str, char **end_ptr); //NO-IMP
	//float strof(const char *str, char **end_ptr); //NO-IMP
	//long strol(const char *str, char **end_ptr); //NO-IMP
	//long double strold(const char *str, char **end_ptr); //NO-IMP
	//long long stroll(const char *str, char **end_ptr); //NO-IMP
	//unsigned long stroul(const char *str, char **end_ptr); //NO-IMP
	//unsigned long long stroull(const char *str, char **end_ptr); //NO-IMP

	//int rand(); //NO-IMP
	//void srand(unsigned int seed); //NO-IMP

	//void *calloc(size_t num, size_t size); //NO-IMP
	//void free(void *ptr); //NO-IMP
	//void *malloc(size_t size); //NO-IMP
	//void *realloc(void *ptr,size_t size); //NO-IMP
}