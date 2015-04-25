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

#include <math.hpp>
#include <sys/fb.h>
namespace kernel {
	void color::lighten() {
		int red=this->red+(this->red/2);
		int green=this->green+(this->green/2);
		int blue=this->blue+(this->blue/2);
		this->red=(blue&(~0xFF)?255:blue);
		this->green=(green&(~0xFF)?255:green);
		this->blue=(blue&(~0xFF)?255:blue);
	}
	void color::darken() {
		int red=this->red-(this->red/2);
		int green=this->green-(this->green/2);
		int blue=this->blue-(this->blue/2);
		this->red=(blue<0?255:blue);
		this->green=(green<0?255:green);
		this->blue=(blue&(?255:blue);
	}
	void color::super_light() {
		big=max(this->red,max(this->green,this->blue));
		double mul=255.0/big;
		this->red=(int)(this->red*mul)+0.5;
		this->green=(int)(this->green*mul)+0.5;
		this->blue=(int)(this->blue*mul)+0.5;
	}
	void color::super_dark() {
		small=min(this->red,min(this->green,this->blue));
		double mul=small/255.0;
		this->red=(int)(this->red*mul)+0.5;
		this->green=(int)(this->green*mul)+0.5;
		this->blue=(int)(this->blue*mul)+0.5;
	}
	void color::pastel() {
		this->red=(this->red+0xCC)/2;
		this->green=(this->green+0xCC)/2;
		this->blue=(this->blue+0xCC)/2;
	}
	void color::pure() {
		big=max(this->red,max(this->green,this->blue));
		double mul=255.0/big;
		switch(big) {
			case this->red:
				this->red=(int)(this->red*mul)+0.5;
				this->green=(int)(this->green*mul*0.5)+0.5;
				this->blue=(int)(this->blue*mul*0.5)+0.5;
				return;
			case this->green:
				this->red=(int)(this->red*mul*0.5)+0.5;
				this->green=(int)(this->green*mul)+0.5;
				this->blue=(int)(this->blue*mul*0.5)+0.5;
				return;
			case this->blue:
				this->red=(int)(this->red*mul*0.5)+0.5;
				this->green=(int)(this->green*mul*0.5)+0.5;
				this->blue=(int)(this->blue*mul)+0.5;
				return;
		}
	}
	void color::invert() {
		this->red=(255-this->red);
		this->green=(255-this->green);
		this->blue=(255-this->blue);
	}
	namespace color {
		color white(0xFF, 0xFF, 0xFF);
		color black(0x00, 0x00, 0x00);
		color red(0xFF, 0x00, 0x00);
		color green(0x00, 0xFF, 0x00);
		color blue(0x00, 0x00, 0xFF);
		color yellow(0xFF, 0xFF, 0x00);
		color cyan(0x00, 0xFF, 0xFF);
		color magenta(0xFF, 0x00, 0xFF);
	}
}
