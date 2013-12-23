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

#ifndef MONITOR_H
#define MONITOR_H
#include <stdint.h>
namespace std {
	class ios_base {
	public:
		uint8_t base;
		//showbase and uppercase are tri-state bools 0=undef 1=false 2=true
		//if the tri-states are at 0 at time of evaluation treat as false
		uint8_t showbase;
		uint8_t uppercase;
		ios_base();
		ios_base(int base,int uppercase,int showbase);
		ios_base(int base);
		void combine(ios_base b);
	};
	class ConsoleColor {
	private:
		uint8_t backcolor;
		uint8_t color;
	public:
		uint16_t getColor();
		ConsoleColor(int foreground, int background);
		ConsoleColor();
	};

	class ostream {
	private:
		ios_base b;
		ConsoleColor c;
		uint16_t *mon=(uint16_t *)0xC00B8000;
		int px,py;
	public:
		ostream();
		ostream(ConsoleColor c);
		ostream& printc(char c);
		ostream& cls();
		ostream& print(const char* s);
		ostream& operator<<(bool b);
		ostream& operator<<(const char *s);
		ostream& operator<<(int i);
		ostream& operator<<(unsigned u);
		ostream& operator<<(long l);
		ostream& operator<<(unsigned long ul);
		ostream& operator<<(ios_base b);
		ostream& operator<<(ConsoleColor c);
	};

	extern ostream cout;

	extern ios_base hex;
	extern ios_base dec;
	extern ios_base oct;
	extern ios_base bin;

	extern ConsoleColor ColorDef;

	extern const char * endl;
}
#endif /* MONITOR_H_ */
