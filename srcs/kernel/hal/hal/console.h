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

#pragma once

namespace hal {
	class ios_base {
		public:
			uint8_t base;
			uint8_t min_digits;
			//showbase and uppercase are tri-state bools 0=undef 1=false 2=true
			//if the tri-states are at 0 at time of evaluation treat as false
			uint8_t showbase;
			uint8_t uppercase;

			ios_base();
			ios_base(int base,int uppercase,int showbase);
			ios_base(int base,int uppercase,int showbase,int min_digits);
			ios_base(int base);
			void combine(ios_base b);
	};

	class ostream {
		private:
			uint32_t color;
			uint32_t back_color;
			uint32_t def_color;
			uint32_t def_back_color;
			ios_base b;
			void print(const char *s);
		public:
			ostream();
			ostream &operator<<(char c);
			ostream &operator<<(const char *s);
			ostream &operator<<(int i);
			ostream &operator<<(unsigned int u);
			ostream &operator<<(long l);
			ostream &operator<<(unsigned long ul);
			ostream &operator<<(long long ll);
			ostream &operator<<(unsigned long long ull);
			ostream &operator<<(const void *p);
			ostream &operator<<(ios_base b);
	};
	/**
	 * @brief scroll screen
	 * @details scrolls the screen 1 line upward, and clears
	 * the bottom most line
	 *
	 * @date created on 2014-04-20
	 */
	void scroll();

	////////////////////////////////////////
	// Scroll 'lines' lines               //
	// Created on 2014-04-20              //
	// Vendor defined                     //
	////////////////////////////////////////
	void scroll(int lines);

	////////////////////////////////////////
	// Print one UTF8 glyph               //
	// Created on 2014-04-20              //
	// Updated on 2014-06-13              //
	// Vendor defined                     //
	////////////////////////////////////////
	void printc(uint32_t back_color, uint32_t color, const char *glyph);

	////////////////////////////////////////
	// Clear the screen                   //
	// Created on 2014-04-20              //
	// Vendor defined                     //
	////////////////////////////////////////
	void cls();

	////////////////////////////////////////
	// Get console width                  //
	// Created on 2014-04-26              //
	// Vendor defined                     //
	////////////////////////////////////////
	int width();

	////////////////////////////////////////
	// Get console height                 //
	// Created on 2014-04-26              //
	// Vendor defined                     //
	////////////////////////////////////////
	int height();

	////////////////////////////////////////
	// Get console 'x' position           //
	// Created on 2014-04-26              //
	// Vendor defined                     //
	////////////////////////////////////////
	int get_x();

	////////////////////////////////////////
	// Get console 'y' position           //
	// Created on 2014-04-26              //
	// Vendor defined                     //
	////////////////////////////////////////
	int get_y();

	////////////////////////////////////////
	// print a boot time message with an  //
	//   [OK] or [NO] printed along side  //
	// msg: message to display            //
	// ok: whether to display [OK] or [NO]//
	// halt: halt the cpu if ok was false?//
	// Created on 2014-05-18              //
	////////////////////////////////////////
	void print_boot_msg(const char *msg,bool ok, bool halt);
	void print_boot_msg(const char *level,const char *app,const char *msg);
	extern ostream cout;

	extern ios_base hex;
	extern ios_base dec;
	extern ios_base oct;
	extern ios_base bin;
	extern ios_base address;

	extern const char *endl;
}
