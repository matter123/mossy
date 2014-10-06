/*
    Copyright 2013-2014 Matthew Fosdick

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
#include <hal/console.h>
#include <conv.hpp>
#include <limits.h>
#include <hal/hal.h>
#include <hal/stackframe.h>
#include <stdlib.h>
#include <utf8.h>
namespace hal {
	void ostream::print(const char *s) {
		static bool back=false;
		if(s==NULL) {
			return print("--NULL POINTER--");
		}
		while(*s) {
			if(get_char_len(s)==5) {
				uint32_t cp=decode_char(s);
				if((cp&0x80000)==0x80000) {
					if(!back) {
						back=(cp&0x1000)==0x1000;
						if(back) {
							//magic_break();
							s+=get_char_len(s);
							continue;
						}
					}
					uint16_t color=cp&0xFFF;
					uint8_t red=(color>>8)&0xF;
					uint8_t green=(color>>4)&0xF;
					uint8_t blue=(color>>0)&0xF;
					uint8_t fc=0;
					if(red==0xF&&green==0x0&&blue==0) {
						fc=12;
					} else if(red==0x7&&green==0x0&&blue==0x0) {
						fc=4;
					} else if(red==0x0&&green==0xF&&blue==0x0) {
						fc=10;
					} else if(red==0x0&&green==0x7&&blue==0x0) {
						fc=2;
					} else if(red==0x0&&green==0x0&&blue==0xF) {
						fc=9;
					} else if(red==0x0&&green==0x0&&blue==0x7) {
						fc=1;
					} else if(red==0xF&&green==0xF&&blue==0x0) {
						fc=14;
					} else if(red==0x7&&green==0x7&&blue==0x0) {
						fc=6;
					} else if(red==0xF&&green==0x0&&blue==0xF) {
						fc=13;
					} else if(red==0x7&&green==0x0&&blue==0x7) {
						fc=5;
					} else if(red==0x0&&green==0xF&&blue==0xF) {
						fc=11;
					} else if(red==0x0&&green==0x7&&blue==0x7) {
						fc=3;
					} else if(red==0xF&&green==0xF&&blue==0xF) {
						fc=15;
					} else if(red==0x0&&green==0x0&&blue==0x0) {
						fc=0;
					} else if(red==0xA&&green==0xA&&blue==0xA) {
						fc=7;
					} else if(red==0x5&&green==0x5&&blue==0x5) {
						fc=8;
					}
					if(back) {
						this->c.backcolor=fc;
						back=false;
					} else {
						this->c.color=fc;
					}
				}
				s+=get_char_len(s);
				continue;
			}
			printc(this->c,s);
			s+=get_char_len(s);
		}
	}
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
	ios_base::ios_base(int b,int u,int s,int d) {
		base=b;
		showbase=s;
		uppercase=u;
		min_digits=d;
	}
	ConsoleColor::ConsoleColor(int foreground, int background) {
		color=foreground;
		backcolor=background;
	}
	ConsoleColor::ConsoleColor() {
		color=0xF;
		backcolor=0x0;
	}
	uint16_t ConsoleColor::getColor() {
		return ((backcolor&0xF)<<4)|(color&0xF);
	}
	void ios_base::combine(ios_base b) {
		if(b.base>0) {
			this->base=b.base;
		}
		if(b.showbase>0) {
			this->showbase=b.showbase;
		}
		if(b.uppercase>0) {
			this->uppercase=b.uppercase;
		}
		this->min_digits=b.min_digits;
	}
	ostream::ostream() {
		c=ColorDef;
	}

	ostream::ostream(ConsoleColor color) {
		c=color;
	}
	ostream &ostream::operator<<(char s) {
		static char buf[2]= {0,0};
		buf[0]=s;
		print(buf);
		return *this;
	}
	ostream &ostream::operator<<(const char *s) {
		print(s);
		return *this;
	}
	ostream &ostream::operator<<(int i) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16) {
				print("0x");
			}
			if(b.base==8) {
				print("0");
			}
			if(b.base==2) {
				print("0b");
			}
		}
		print(std::numtostr(i,buf,b.base,b.uppercase,b.min_digits));
		return *this;
	}
	ostream &ostream::operator<<(unsigned int u) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16) {
				print("0x");
			}
			if(b.base==8) {
				print("0");
			}
			if(b.base==2) {
				print("0b");
			}
		}
		print(std::numtostr(u,buf,b.base,b.uppercase,b.min_digits));
		return *this;
	}
	ostream &ostream::operator<<(long l) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16) {
				print("0x");
			}
			if(b.base==8) {
				print("0");
			}
			if(b.base==2) {
				print("0b");
			}
		}
		print(std::numtostr(l,buf,b.base,b.uppercase,b.min_digits));
		return *this;
	}
	ostream &ostream::operator<<(unsigned long ul) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16) {
				print("0x");
			}
			if(b.base==8) {
				print("0");
			}
			if(b.base==2) {
				print("0b");
			}
		}
		print(std::numtostr(ul,buf,b.base,b.uppercase,b.min_digits));
		return *this;
	}
	ostream &ostream::operator<<(long long ll) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16) {
				print("0x");
			}
			if(b.base==8) {
				print("0");
			}
			if(b.base==2) {
				print("0b");
			}
		}
		print(std::numtostr(ll,buf,b.base,b.uppercase,b.min_digits));
		return *this;
	}
	ostream &ostream::operator<<(unsigned long long ull) {
		char buf[50];
		if(b.showbase) {
			if(b.base==16) {
				print("0x");
			}
			if(b.base==8) {
				print("0");
			}
			if(b.base==2) {
				print("0b");
			}
		}
		print(std::numtostr(ull,buf,b.base,b.uppercase,b.min_digits));
		return *this;
	}
	ostream &ostream::operator<<(const void *p) {
		char buf[50];
#define S_UP sizeof(void *)
		print("0x");
		print(std::numtostr(reinterpret_cast<uintptr_t>(p),buf,16,true,
		                    (S_UP==8?16:8)));
		return *this;
	}
	ostream &ostream::operator<<(ios_base base) {
		b.combine(base);
		return *this;
	}
	ostream &ostream::operator<<(ConsoleColor color) {
		c=color;
		return *this;
	}
	void scroll() {
		scroll(1);
	}
	void print_boot_msg(const char *msg,bool ok, bool hlt) {
		if(get_x()!=0) {
			cout<<endl;
		}
		cout<<msg;
		while(width()-6>get_x()) {
			cout<<".";
		}
		cout<<" [";
		if(ok) {
			ConsoleColor gr(0xA,0x0);
			cout<<gr<<"OK"<<ColorDef;
		} else {
			ConsoleColor r(0x4,0x0);
			cout<<r<<"NO"<<ColorDef;
		}
		cout<<"]"<<endl;
		if(!ok&&hlt) {
			halt(true);
		}
	}
	void dump_regs(cpu_state *s) {
		cout<<endl;
		cout<<dec<<"Exception: "<<::get_int_num(s);
		if(::get_mnemonic(s)) {
			cout<<"("<<::get_mnemonic(s)<<")";
		}
		if(::get_err_code(s)) {
			cout<<" code: "<<::get_err_code(s)<<endl;
		} else {
			cout<<endl;
		}
		bool endol=false;
		int guess_stride=0;
		for(int i=0; i<::get_reg_count(); i++) {
			cout<<dec<<"reg "<<i<<": "<<address<<::get_reg(s,i)<<" ";
			if(guess_stride==0) {
				guess_stride=get_x();
			}
			if((get_x()+guess_stride)>width()) {
				cout<<endl;
				endol=true;
			} else {
				endol=false;
			}
		}
		if(!endol) {
			cout<<endl;
		}
		guess_stride=0;
		for(int i=0; i<::get_creg_count(); i++) {
			cout<<dec<<"control reg "<<i<<": "<<address<<::get_creg(s,i)<<" ";
			if(guess_stride==0) {
				guess_stride=get_x();
			}
			if((get_x()+guess_stride)>width()) {
				cout<<endl;
				endol=true;
			} else {
				endol=false;
			}
		}
		if(!endol) {
			cout<<endl;
		}
		cout<<"instruction pointer: "<<address<<::get_instruction_pointer(s)<<endl;
		cout<<"end cpu dump"<<endl;
		// cout<<"cs: "<<s->cs<<endl;
		// cout<<"ds: "<<s->ds<<endl;
		// cout<<"es: "<<s->es<<endl;
		// cout<<"ss: "<<s->ss<<endl;
		hal::magic_break();
		cout<<"Stack Trace:"<<endl;
		stack_frame *f=::get_frame(s);
		while(f&&get_function(f)) {
			cout<<"\t"<<address<<get_function(f)<<endl;
			f=get_next_frame(f);
		}
		cout<<"end stack trace"<<endl;
	}
	ConsoleColor ColorDef(0xF,0x0);
	ostream cout;
	ios_base dec;
	ios_base hex(16,2,2);
	ios_base oct(8,2,2);
	ios_base bin(2,2,2);
}
