/*
 * Copyright 2014 Matthew Fosdick

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

#include <debug.h>
#include <conv.hpp>

namespace std {
	debug_level dl=dl_warning;

	//for now assume mode 7 color text mode, later use stdio for this
	int x,y;
	void write_c(int attrib,char c) {
		if(c=='\n') {
			x=0;
			y++;
			return;
		}
		if(c=='\t') {
			x=(x+4)&(~3);
			return;
		}
		(reinterpret_cast<uint16_t *>(0xC00B8000))[y*80+x++]=c|attrib<<8;
		if(x>=80) {
			x=0;
			y++;
		}
	}
	void write(int attrib,c_cstring s) {
		while(*s)write_c(attrib,*(s++));
	}
	void debug(debug_level level,c_cstring s) {
		if(level<=dl)write((level==dl_error?0xD:0xF),s);
	}


	void debug_report_level(debug_level level) {
		dl=level;
	}

	void error(c_cstring s) {
		debug(dl_error,s);
	}

	void cls(cstring s) {
		std::memset16(reinterpret_cast<uint16_t *>(0xC00B8000),' '|0xF,80*25);
	}

	void strace(uint32_t depth) {
		uint32_t *ebp=&depth-2;
		debug(dl_critical,"Stack trace:    \n");
		for(uint32_t frame=0;frame<depth;++frame) {
			uint32_t eip=ebp[1];
			if(eip==0) {
				//bottem of stack
				goto hlt;
			}
			ebp=reinterpret_cast<uint32_t *>(ebp[0]);
			char bufa[9];
			char *buf=bufa;
			buf=numtostr(eip,buf,16,true,8);
			debug(dl_critical,"  0x");
			debug(dl_critical,buf);
			debug(dl_critical,"    \n");
		}
		hlt:
		debug(dl_critical,"end trace.      \n");
		asm volatile ("cli\nhlt");
		while(1);//no reach
	}

	void assert(bool condition) {
		if(!condition) {
			asm volatile ("cli\nhlt");
		}
		while(1);//no reach
	}
}