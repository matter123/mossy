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
#include <utf8.h>
#if TEST
#include <test.h>
void test_utf8()__attribute__((used));
static kernel::test_module test_module=kernel::test_module("UTF-8   ",&test_utf8);
void test_utf8() {
	bool abrt = false;
	unit_test("a",0,0);
	test_module.set_passed();
}

#endif
size_t get_char_len(const char *str) {
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
	return (count>5?-1:count);
}

char *next_char(const char *str) {
	size_t skip=get_char_len(str);
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
	do {
		int len=get_char_len(c1);
		if(len!=get_char_len(c2)) {
			break;
		}
		while(len--) {
			if(*(c1+len)!=*(c2+len)) {
				break;
			}
		}
		return true;
	} while(0);
	//handle overlong
	uint32_t cp1=decode_char(c1);
	uint32_t cp2=decode_char(c2);
	return cp1==cp2;
}

void char_copy(char *str, const char *c) {
	size_t len=get_char_len(c);
	while(len--) {
		*(str++)=*(c++);
	}
}
uint32_t decode_char(const char *c) {
	size_t len=get_char_len(c);
	switch(len) {
		case 1:
			return *c;
		case 2:
			return ((*(c+0)&0x1F)<<6)|((*(c+1)&0x3F)<<00);
		case 3:
			return ((*(c+0)&0x0F)<<12)|((*(c+1)&0x3F)<<6)|((*(c+2)&0x3F)<<0);
		case 4:
			return ((*(c+0)&0x07)<<18)|((*(c+1)&0x3F)<<12)|((*(c+2)&0x3F)<<6)
			       |((*(c+3)&0x3F)<<0);
		case 5:
			return ((*(c+0)&0x03)<<24)|((*(c+1)&0x3F)<<18)|((*(c+2)&0x3F)<<12)
			       |((*(c+3)&0x3F)<<6)|((*(c+4)&0x3F)<<0);
		default:
			return 0;
	}
}

size_t encode_char(uint32_t cp,char *b) {
	if(cp>0x3FFFFFF) {
		return 0;
	}
	size_t len=0;
	int cp_store=0;
	if(cp>0x1FFFFF) {
		len=5;
		cp_store=26;
	} else if(cp>0xFFFF) {
		len=4;
		cp_store=21;
	} else if(cp>0x7FF) {
		len=3;
		cp_store=16;
	} else if(cp>0x7F) {
		len=2;
		cp_store=11;
	} else {
		len=1;
		cp_store=7;
	}

	//set non coding portion of lead byte
	uint8_t lead=(len==1?0x00:0x80);
	for(int i=2; i<=len; i++) {
		lead|=(1<<(8-i));
	}

	uint8_t lead_cp[5]= {7,5,4,3,2};
	uint8_t masks[6]= {0x7F,0x1F,0xF,0x7,0x3,0x3F};
	//create lead byte
	*b++=lead|((cp>>(cp_store-=lead_cp[len-1]))&masks[len-1]);

	//fill in following bytes
	while(cp_store>0) {
		*b++=0x80|((cp>>(cp_store-=6))&masks[5]);
	}
	return len;
}

void encode_five(uint32_t cp,char *b) {
	if(cp>0x3FFFFFF) {
		return;
	}
	int cp_store=26;

	//set non coding portion of lead byte
	uint8_t lead=0x80;
	for(int i=2; i<=5; i++) {
		lead|=(1<<(8-i));
	}

	//create lead byte
	*b++=lead|((cp>>(cp_store-=2))&0x3);

	//fill in following bytes
	while(cp_store>0) {
		*b++=0x80|((cp>>(cp_store-=6))&0x3F);
	}
}

size_t normalize(char *dest, const char *src) {
	size_t tlen=0;
	while(*src) {
		size_t len=get_char_len(src);
		//5 is a special overlong, and is to stay that way
		if(len==5) {
			char_copy(dest,src);
			tlen+=5;
			dest+=5;
			src+=5;
			continue;
		}
		uint32_t decode=decode_char(src);
		size_t slen=0;
		if(decode>0xFFFF) {
			slen=4;
		} else if(decode>0x7FF) {
			slen=3;
		} else if(decode>0x7F) {
			slen=2;
		} else {
			slen=1;
		}
		if(slen!=len) {
			encode_char(decode,dest);
		} else {
			char_copy(dest,src);
		}
		tlen+=slen;
		dest+=slen;
		src+=len;
	}
	*dest='\0';
	return tlen;
}

size_t normalizen(char *dest, const char *src, size_t num) {
	size_t tlen=0;
	while(*src && num>0) {
		size_t len=get_char_len(src);
		//5 is a special overlong, and is to stay that way
		if(len==5) {
			char_copy(dest,src);
			tlen+=5;
			dest+=5;
			src+=5;
			num-=5;
			continue;
		}
		uint32_t decode=decode_char(src);
		size_t slen=0;
		if(decode>0xFFFF) {
			slen=4;
		} else if(decode>0x7FF) {
			slen=3;
		} else if(decode>0x7F) {
			slen=2;
		} else {
			slen=1;
		}
		if((slen-num)<=0) {
			break;
		}
		if(slen!=len) {
			encode_char(decode,dest);
		} else {
			char_copy(dest,src);
		}
		tlen+=slen;
		dest+=slen;
		src+=len;
	}
	*dest='\0';
	return tlen;
}