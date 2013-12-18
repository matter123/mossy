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

#include "monitor.h"
#include <conv.hpp>
namespace std {
	ios_base::ios_base() {
		base=10;
		showbase=1;
		uppercase=1;
	}

	ios_base::ios_base(int b) {
		base=b;
		showbase=0;
		uppercase=0;
	}

	ios_base::ios_base(int b,int u,int s) {
		base=b;
		showbase=s;
		uppercase=u;
	}
	console_color::console_color(int foreground, int background) {
		color=foreground;
		backcolor=background;
	}
	console_color::console_color() {
		color=0xF;
		backcolor=0x0;
	}
	uint16_t console_color::getColor() {
		return ((backcolor&0xF)<<4)|(color&0xF);
	}
	void ios_base::combine(ios_base b) {
		if(b.base>0)this->base=b.base;
		if(b.showbase>0)this->showbase=b.showbase;
		if(b.uppercase>0)this->uppercase=b.uppercase;
	}

	ostream & ostream::print(const char *s) {
		while(*s)printc(*(s++));
		return *this;
	}

	ostream & ostream::printc(char let) {
		if(let=='\n') {
			px=0;
			py++;
			return *this;
		}
		if(let=='\t') {
			px=(px+4)&(~3);
			return *this;
		}
		mon[py*80+px++]=let|this->c.getColor()<<8;
		if(px>=80) {
			px=0;
			py++;
		}
		return *this;
	}

	ostream::ostream(){
		px=0;
		py=0;
		c=console_color(0xF,0x0);
	}

	ostream::ostream(console_color color){
		px=0;
		py=0;
		c=color;
	}

	ostream &ostream::operator <<(const char *s) {
		print(s);
		return *this;
	}
	ostream &ostream::operator <<(int i) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16)print("0x");
			if(b.base==8)print("0");
			if(b.base==2)print("0b");
		}
		print(numtostr(i,buf,b.base,b.uppercase));
		return *this;
	}
	ostream &ostream::operator <<(unsigned int u) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16)print("0x");
			if(b.base==8)print("0");
			if(b.base==2)print("0b");
		}
		print(numtostr(u,buf,b.base,b.uppercase));
		return *this;
	}
	ostream &ostream::operator <<(long l) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16)print("0x");
			if(b.base==8)print("0");
			if(b.base==2)print("0b");
		}
		print(numtostr(l,buf,b.base,b.uppercase));
		return *this;
	}
	ostream &ostream::operator <<(unsigned long ul) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16)print("0x");
			if(b.base==8)print("0");
			if(b.base==2)print("0b");
		}
		print(numtostr(ul,buf,b.base,b.uppercase));
		return *this;
	}

	ostream &ostream::operator <<(ios_base base) {
		b.combine(base);
		return *this;
	}
	ostream& ostream::operator<<(console_color color) {
		c=color;
		return *this;
	}

	ostream & ostream::cls() {
		px=0;
		py=0;
		for(int x=0;x<80;x++) {
			for(int y=0;y<25;y++) {
				mon[y*80+x]=' '|this->c.getColor()<<8;
			}
		}
		return *this;
	}
	console_color def(0xF,0x0);

	ostream cout(def);

	ios_base dec;
	ios_base hex(16,2,2);
	ios_base oct(8,2,2);
	ios_base bin(2,2,2);

	char const *endl="\n";
}
