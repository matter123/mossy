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
#include <unicode.h>
#include <utf8.h>
#include <ctype.h>
namespace unicode {
	/**
	 * @brief checks if an encoding is a single encoding
	 * @details determines if encoding \a e represents a
	 * single encoding format
	 *
	 * @param e the encoding format to check
	 * @return true IFF e represents a single valid format
	 * otherwise false
	 * @date created on 2014-10-18
	 */
	bool is_single_encoding(encoding e) {
		uint32_t iencode=static_cast<uint32_t>(e);
		return !(iencode&(iencode-1)); //is iencode a POT, only real encoding are POT
	}

	void copy_codeunit(char *dest, char *src) {
		size_t len=utf8::get_char_len(src);
		while(len--) {
			*(dest++)=*(src++);
		}
	}

	bool isascii(char c) {
		return c>32&&c<127;
	}
	/**
	 * @brief detects encoding of the byte stream \a s
	 * @details uses heuristics to try to detect
	 *
	 */
	enum encoding detect_encoding(char *s, size_t len) {
		uint8_t *stream=reinterpret_cast<uint8_t *>(s);
		bool big_endian;
		int u8_weight=0,u16_weight=0,u32_weight=0,swap_weight=0;
		if(len<4) {
			u32_weight=-1;//u32 is out of running
		}
		if(*stream!=0xFF) {
			big_endian=true;
		}
		//check the string alignment
		if((uintptr_t)stream%2) {
			u8_weight+=10;
		} else if((uintptr_t)stream%4) {
			u16_weight+=10;
		}
		//bom check
		if(*stream==0x00&&u32_weight!=-1) {
			u32_weight+=100;//if the first byte is zero it is most likely u32 big endian
		} else if(*stream==0xEF) {
			u8_weight+=100;
		} else if(big_endian&&*stream==0xFE) {
			u16_weight+=100;
		} else if(*stream==0xFF) {
			u16_weight+=50;
			if(u32_weight!=-1) {
				u32_weight+=50;
			}
		} else {
			//no bom was found
			//scan half of the stream and do NULL byte checks
			//we start at 1 to simplify checks to the left
			for(int i=1; i<(len>200?len/2:len); i++) {
				if(!i%2) {
					if(!i%4&&stream[i]==0x00) {
						swap_weight++;
					} else if(u16_weight>u32_weight&&stream[i]==0x00) {
						swap_weight++;
					}
				} else if(stream[i]==0x00) {
					swap_weight--;
				}
				if(swap_weight>25) {
					big_endian=false;
				} else if(swap_weight<-25) {
					big_endian=true;
				}
				if(stream[i]==0x00) {
					//zeros are probaly UTF-16/UTF-32
					u8_weight--;
					if(stream[i+1]==0x00) {
						u32_weight++;
					} else {
						u16_weight++;
					}
				} else if(stream[i]>=0x80&&stream[i]<=0xBF) {
					//try detecting special UT8 chars
					u8_weight++;
					if(stream[i-1]>=0xC0&&stream[i-1]<=0xF7) {
						u8_weight++;
					}
				} else if(stream[i]==0x20) {
					//a space
					if(stream[i-1]==0x00||stream[i+1]==0x00) {
						u16_weight+=1;
						u32_weight+=1;
					} else {
						u8_weight+=2;
					}
				} else if(isascii(stream[i])&&isascii(stream[i-1])&&isascii(stream[i+1])) {
					u8_weight+=5;
				}
			}
		}
		//done with heuristics
		if(u8_weight-u16_weight>(len>200?15:0)) {
			if(u8_weight-u32_weight>(len>200?15:0)) {
				return UTF8;
			}
		}
		if(u16_weight-u8_weight>(len>200?15:0)) {
			if(u16_weight-u32_weight>(len>200?15:0)) {
				return big_endian?UTF16BE:UTF16LE;
			}
		}
		if(u32_weight-u8_weight>(len>200?15:0)) {
			if(u32_weight-u16_weight>(len>200?15:0)) {
				return big_endian?UTF32BE:UTF32LE;
			}
		}
		return UNKNOWN;
	}
}
