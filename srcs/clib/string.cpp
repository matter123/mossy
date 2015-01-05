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
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "utf8.h"
extern "C" {
	/*
	##     ## ######## ##     ##  ######  ##     ## ########
	###   ### ##       ###   ### ##    ## ##     ## ##     ##
	#### #### ##       #### #### ##       ##     ## ##     ##
	## ### ## ######   ## ### ## ##       ######### ########
	##     ## ##       ##     ## ##       ##     ## ##   ##
	##     ## ##       ##     ## ##    ## ##     ## ##    ##
	##     ## ######## ##     ##  ######  ##     ## ##     ##
	*/
	void *memchr(const void *ptr, int value, size_t num) {
		uint8_t *p=(uint8_t *)ptr;
		while(num--) {
			if(*p==(uint8_t)value) {
				return p;
			}
			p++;
		}
		return NULL;
	}
	/*
	 ######  ######## ########   ######  ##     ## ########
	##    ##    ##    ##     ## ##    ## ##     ## ##     ##
	##          ##    ##     ## ##       ##     ## ##     ##
	 ######     ##    ########  ##       ######### ########
	      ##    ##    ##   ##   ##       ##     ## ##   ##
	##    ##    ##    ##    ##  ##    ## ##     ## ##    ##
	 ######     ##    ##     ##  ######  ##     ## ##     ##
	*/
	char *strchr(const char *str, int value) {
		while(*str) {
			if(*str==(char)value) {
				return (char *)str;
			}
			str++;
		}
		return NULL;
	}
	/*
	##     ## ########   ######   ######  ##     ## ########
	###   ### ##     ## ##    ## ##    ## ##     ## ##     ##
	#### #### ##     ## ##       ##       ##     ## ##     ##
	## ### ## ########   ######  ##       ######### ########
	##     ## ##     ##       ## ##       ##     ## ##   ##
	##     ## ##     ## ##    ## ##    ## ##     ## ##    ##
	##     ## ########   ######   ######  ##     ## ##     ##
	*/
	char *mbschr(const char *str, const char *value) {
		while(*str) {
			if(unicode::utf8::is_char_equal(str,value)) {
				return (char *)str;
			}
			str=unicode::utf8::next_char(str);
		}
		return NULL;
	}
	/*
	 ######  ######## ########   ######   ######  ########  ##    ##
	##    ##    ##    ##     ## ##    ## ##    ## ##     ## ###   ##
	##          ##    ##     ## ##       ##       ##     ## ####  ##
	 ######     ##    ########  ##        ######  ########  ## ## ##
	      ##    ##    ##   ##   ##             ## ##        ##  ####
	##    ##    ##    ##    ##  ##    ## ##    ## ##        ##   ###
	 ######     ##    ##     ##  ######   ######  ##        ##    ##
	*/
	size_t strcspn(const char *str1, const char *str2) {
		const char *s2=str2;
		const char *s1=str1;
		while(*str1) {
			s2=str2;
			while(*s2) {
				if(*s2++==*str1) {
					return str1-s1;
				}
			}
			str1++;
		}
		return str1-s1;
	}
	/*
	##     ## ########   ######   ######   ######  ########  ##    ##
	###   ### ##     ## ##    ## ##    ## ##    ## ##     ## ###   ##
	#### #### ##     ## ##       ##       ##       ##     ## ####  ##
	## ### ## ########   ######  ##        ######  ########  ## ## ##
	##     ## ##     ##       ## ##             ## ##        ##  ####
	##     ## ##     ## ##    ## ##    ## ##    ## ##        ##   ###
	##     ## ########   ######   ######   ######  ##        ##    ##
	*/
	size_t mbscspn(const char *str1, const char *str2) {
		const char *s=str2;
		size_t len=0;
		while(*str1) {
			s=str2;
			while(*s) {
				if(unicode::utf8::is_char_equal(str1,s)) {
					return len;
				}
				s=unicode::utf8::next_char(s);
			}
			str1=unicode::utf8::next_char(str1);
			len++;
		}
		return len;
	}
	/*
	 ######  ######## ########  ########  ########  ########  ##    ##
	##    ##    ##    ##     ## ##     ## ##     ## ##     ## ##   ##
	##          ##    ##     ## ##     ## ##     ## ##     ## ##  ##
	 ######     ##    ########  ########  ########  ########  #####
	      ##    ##    ##   ##   ##        ##     ## ##   ##   ##  ##
	##    ##    ##    ##    ##  ##        ##     ## ##    ##  ##   ##
	 ######     ##    ##     ## ##        ########  ##     ## ##    ##
	*/
	char *strpbrk(const char *str1, const char *str2) {
		const char *s2=str2;
		while(*str1) {
			s2=str2;
			while(*s2) {
				if(*s2++==*str1) {
					return (char *)str1;
				}
			}
			str1++;
		}
		return NULL;
	}
	/*
	##     ## ########   ######  ########  ########  ########  ##    ##
	###   ### ##     ## ##    ## ##     ## ##     ## ##     ## ##   ##
	#### #### ##     ## ##       ##     ## ##     ## ##     ## ##  ##
	## ### ## ########   ######  ########  ########  ########  #####
	##     ## ##     ##       ## ##        ##     ## ##   ##   ##  ##
	##     ## ##     ## ##    ## ##        ##     ## ##    ##  ##   ##
	##     ## ########   ######  ##        ########  ##     ## ##    ##
	*/
	char *mbspbrk(const char *str1, const char *chars) {
		const char *s=chars;
		while(*str1) {
			s=chars;
			while(*s) {
				if(unicode::utf8::is_char_equal(str1,s)) {
					return (char *)str1;
				}
				s=unicode::utf8::next_char(s);
			}
			str1=unicode::utf8::next_char(str1);
		}
		return NULL;
	}
	/*
	 ######  ######## ########  ########   ######  ##     ## ########
	##    ##    ##    ##     ## ##     ## ##    ## ##     ## ##     ##
	##          ##    ##     ## ##     ## ##       ##     ## ##     ##
	 ######     ##    ########  ########  ##       ######### ########
	      ##    ##    ##   ##   ##   ##   ##       ##     ## ##   ##
	##    ##    ##    ##    ##  ##    ##  ##    ## ##     ## ##    ##
	 ######     ##    ##     ## ##     ##  ######  ##     ## ##     ##
	*/
	char *strrchr(const char *str1, int value) {
		char *last=NULL;
		if(value=='\0') {
			return (char *)str1+strlen(str1);
		}
		while(*str1) {
			if(*str1==(char)value) {
				last=(char *)str1;
			}
			str1++;
		}
		return last;
	}
	/*
	##     ## ########   ######  ########   ######  ##     ## ########
	###   ### ##     ## ##    ## ##     ## ##    ## ##     ## ##     ##
	#### #### ##     ## ##       ##     ## ##       ##     ## ##     ##
	## ### ## ########   ######  ########  ##       ######### ########
	##     ## ##     ##       ## ##   ##   ##       ##     ## ##   ##
	##     ## ##     ## ##    ## ##    ##  ##    ## ##     ## ##    ##
	##     ## ########   ######  ##     ##  ######  ##     ## ##     ##
	*/
	char *mbsrchr(const char *str1, const char *value) {
		char *last=NULL;
		if(*value=='\0') {
			return (char *)str1+strlen(str1);
		}
		while(*str1) {
			if(unicode::utf8::is_char_equal(str1,value)) {
				last=(char *)str1;
			}
			str1=unicode::utf8::next_char(str1);
		}
		return last;
	}
	/*
	 ######  ######## ########   ######  ########  ##    ##
	##    ##    ##    ##     ## ##    ## ##     ## ###   ##
	##          ##    ##     ## ##       ##     ## ####  ##
	 ######     ##    ########   ######  ########  ## ## ##
	      ##    ##    ##   ##         ## ##        ##  ####
	##    ##    ##    ##    ##  ##    ## ##        ##   ###
	 ######     ##    ##     ##  ######  ##        ##    ##
	*/
	size_t strspn(const char *str1, const char *bytes) {
		size_t count=0;
		const char *s=bytes;
		while(*str1) {
			s=bytes;
			bool match=false;
			while(*s) {
				if(*str1==*s) {
					match=true;
					count++;
					break;
				}
				s++;
			}
			if(!match) {
				break;
			}
			str1++;
		}
		return count;
	}
	/*
	##     ## ########   ######   ######  ########  ##    ##
	###   ### ##     ## ##    ## ##    ## ##     ## ###   ##
	#### #### ##     ## ##       ##       ##     ## ####  ##
	## ### ## ########   ######   ######  ########  ## ## ##
	##     ## ##     ##       ##       ## ##        ##  ####
	##     ## ##     ## ##    ## ##    ## ##        ##   ###
	##     ## ########   ######   ######  ##        ##    ##
	*/
	size_t mbsspn(const char *str, const char *chars) {
		size_t count=0;
		const char *s=chars;
		while(*str) {
			s=chars;
			bool match=false;
			while(*s) {
				if(unicode::utf8::is_char_equal(str,s)) {
					match=true;
					count++;
					break;
				}
				s=unicode::utf8::next_char(s);
			}
			if(!match) {
				break;
			}
			str=unicode::utf8::next_char(str);
		}
		return count;
	}
	/*
	 ######  ######## ########   ######  ######## ########
	##    ##    ##    ##     ## ##    ##    ##    ##     ##
	##          ##    ##     ## ##          ##    ##     ##
	 ######     ##    ########   ######     ##    ########
	      ##    ##    ##   ##         ##    ##    ##   ##
	##    ##    ##    ##    ##  ##    ##    ##    ##    ##
	 ######     ##    ##     ##  ######     ##    ##     ##
	*/
	char *strstr(const char *str1, const char *str2) {
		const char *s1=str1;
		const char *s2=str2;
		while(*str1) {
			s2=str2;
			s1=str1;
			bool found=true;
			while(*s2) {
				if(*s1==*s2) {
					s1++;
					s2++;
				} else {
					found=false;
					break;
				}
			}
			if(found) {
				return (char *)str1;
			}
			str1++;
		}
		return NULL;
	}
	/*
	##     ## ########   ######   ######  ######## ########
	###   ### ##     ## ##    ## ##    ##    ##    ##     ##
	#### #### ##     ## ##       ##          ##    ##     ##
	## ### ## ########   ######   ######     ##    ########
	##     ## ##     ##       ##       ##    ##    ##   ##
	##     ## ##     ## ##    ## ##    ##    ##    ##    ##
	##     ## ########   ######   ######     ##    ##     ##
	*/
	char *mbsstr(char *str1, const char *str2) {
		char *s1=str1;
		const char *s2=str2;
		while(*str1) {
			s2=str2;
			s1=str1;
			bool found=true;
			while(*s2) {
				if(unicode::utf8::is_char_equal(s1,s2)) {
					s1=unicode::utf8::next_char(s1);
					s2=unicode::utf8::next_char(s2);
				} else {
					found=false;
					break;
				}
			}
			if(found) {
				return str1;
			}
			str1=unicode::utf8::next_char(str1);
		}
		return NULL;
	}
	/*
	 ######  ######## ########  ########  #######  ##    ##
	##    ##    ##    ##     ##    ##    ##     ## ##   ##
	##          ##    ##     ##    ##    ##     ## ##  ##
	 ######     ##    ########     ##    ##     ## #####
	      ##    ##    ##   ##      ##    ##     ## ##  ##
	##    ##    ##    ##    ##     ##    ##     ## ##   ##
	 ######     ##    ##     ##    ##     #######  ##    ##
	*/
	char *strtok(char *str, const char *delim) {
		static char *last=NULL;
		if(str==NULL) {
			if(last==NULL||*last=='\0') {
				return NULL;
			}
			str=last;
		}
		if(!*str) {
			return NULL;
		}
		char *s_tok=NULL;
		while(*str) {
			const char *d=delim;
			bool match=false;
			while(*d) {
				if(*d==*str) {
					match=true;
					break;
				}
				d++;
			}
			if(!s_tok&&!match) {
				s_tok=str;
				str++;
				continue;
			}
			if(s_tok&&match) {
				last=str+1;
				*str='\0';
				return s_tok;
			}
			str++;
		}
		last=NULL;
		return s_tok;
	}
	/*
	##     ## ########   ######  ########  #######  ##    ##
	###   ### ##     ## ##    ##    ##    ##     ## ##   ##
	#### #### ##     ## ##          ##    ##     ## ##  ##
	## ### ## ########   ######     ##    ##     ## #####
	##     ## ##     ##       ##    ##    ##     ## ##  ##
	##     ## ##     ## ##    ##    ##    ##     ## ##   ##
	##     ## ########   ######     ##     #######  ##    ##
	*/
	char *mbstok(char *str, const char *delim) {
		static char *last=NULL;
		if(str==NULL) {
			if(last==NULL||*last=='\0') {
				return NULL;
			}
			str=last;
		}
		if(!*str) {
			return NULL;
		}
		char *s_tok=NULL;
		while(*str) {
			const char *d=delim;
			bool match=false;
			while(*d) {
				if(unicode::utf8::is_char_equal(d,str)) {
					match=true;
					break;
				}
				d=unicode::utf8::next_char(d);
			}
			if(!s_tok&&!match) {
				s_tok=str;
				str=unicode::utf8::next_char(str);
				continue;
			}
			if(s_tok&&match) {
				last=unicode::utf8::next_char(str);
				*str='\0';
				return s_tok;
			}
			str=unicode::utf8::next_char(str);
		}
		last=NULL;
		return s_tok;
	}
	/*
	##     ## ######## ##     ##  ######  ######## ########
	###   ### ##       ###   ### ##    ## ##          ##
	#### #### ##       #### #### ##       ##          ##
	## ### ## ######   ## ### ##  ######  ######      ##
	##     ## ##       ##     ##       ## ##          ##
	##     ## ##       ##     ## ##    ## ##          ##
	##     ## ######## ##     ##  ######  ########    ##
	*/
	void *memset(void *ptr,int value,size_t num) {
		uint8_t *p=(uint8_t *)ptr;
		while(num--) {
			*p++=(uint8_t)value;
		}
		return ptr;
	}
	/*
	##     ## ######## ##     ##  ######  ######## ########    ##    #######
	###   ### ##       ###   ### ##    ## ##          ##     ####   ##     ##
	#### #### ##       #### #### ##       ##          ##       ##   ##
	## ### ## ######   ## ### ##  ######  ######      ##       ##   ########
	##     ## ##       ##     ##       ## ##          ##       ##   ##     ##
	##     ## ##       ##     ## ##    ## ##          ##       ##   ##     ##
	##     ## ######## ##     ##  ######  ########    ##     ######  #######
	*/
	void *memset16(uint16_t *ptr,uint16_t value,size_t num) {
		uint16_t *ret=ptr;
		while(num--) {
			*ptr++=value;
		}
		return ret;
	}
	/*
	##     ## ######## ##     ##  ######  ######## ########  #######   #######
	###   ### ##       ###   ### ##    ## ##          ##    ##     ## ##     ##
	#### #### ##       #### #### ##       ##          ##           ##        ##
	## ### ## ######   ## ### ##  ######  ######      ##     #######   #######
	##     ## ##       ##     ##       ## ##          ##           ## ##
	##     ## ##       ##     ## ##    ## ##          ##    ##     ## ##
	##     ## ######## ##     ##  ######  ########    ##     #######  #########
	*/
	void *memset32(uint32_t *ptr,uint32_t value,size_t num) {
		uint32_t *ret=ptr;
		while(num--) {
			*ptr++=value;
		}
		return ret;
	}
	/*
	##     ## ######## ##     ##  ######  ######## ########  #######  ##
	###   ### ##       ###   ### ##    ## ##          ##    ##     ## ##    ##
	#### #### ##       #### #### ##       ##          ##    ##        ##    ##
	## ### ## ######   ## ### ##  ######  ######      ##    ########  ##    ##
	##     ## ##       ##     ##       ## ##          ##    ##     ## #########
	##     ## ##       ##     ## ##    ## ##          ##    ##     ##       ##
	##     ## ######## ##     ##  ######  ########    ##     #######        ##
	*/
	void *memset64(uint64_t *ptr,uint64_t value,size_t num) {
		uint64_t *ret=ptr;
		while(num--) {
			*ptr++=value;
		}
		return ret;
	}
	/*
	 ######  ######## ########  ##       ######## ##    ##
	##    ##    ##    ##     ## ##       ##       ###   ##
	##          ##    ##     ## ##       ##       ####  ##
	 ######     ##    ########  ##       ######   ## ## ##
	      ##    ##    ##   ##   ##       ##       ##  ####
	##    ##    ##    ##    ##  ##       ##       ##   ###
	 ######     ##    ##     ## ######## ######## ##    ##
	*/
	size_t strlen(const char *str) {
		size_t count=0;
		while(*str++) {
			count++;
		}
		return count;
	}
	/*
	##     ## ########   ######  ##       ######## ##    ##
	###   ### ##     ## ##    ## ##       ##       ###   ##
	#### #### ##     ## ##       ##       ##       ####  ##
	## ### ## ########   ######  ##       ######   ## ## ##
	##     ## ##     ##       ## ##       ##       ##  ####
	##     ## ##     ## ##    ## ##       ##       ##   ###
	##     ## ########   ######  ######## ######## ##    ##
	*/
	size_t mbslen(const char *str) {
		size_t count=0;
		while(str&&*str) {
			count++;
			str=unicode::utf8::next_char(str);
		}
		return count;
	}
}
