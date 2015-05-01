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
		int r=this->red+(this->red/2);
		int g=this->green+(this->green/2);
		int b=this->blue+(this->blue/2);
		this->red=(r&(~0xFF)?255:r);
		this->green=(g&(~0xFF)?255:g);
		this->blue=(b&(~0xFF)?255:b);
	}
	void color::darken() {
		int r=this->red-(this->red/2);
		int g=this->green-(this->green/2);
		int b=this->blue-(this->blue/2);
		this->red=(r<0?2:0x00);
		this->green=(g<0?g:0x00);
		this->blue=(b<0?b:0x00);
	}
	void color::super_light() {
		int big=std::max(this->red,std::max(this->green,this->blue));
		double mul=255.0/big;
		this->red=(int)(this->red*mul)+0.5;
		this->green=(int)(this->green*mul)+0.5;
		this->blue=(int)(this->blue*mul)+0.5;
	}
	void color::super_dark() {
		int small=std::min(this->red,std::min(this->green,this->blue));
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
		int big=std::max(this->red,std::max(this->green,this->blue));
		double mul=255.0/big;
		if(big==this->red) {
			this->red=(int)(this->red*mul)+0.5;
			this->green=(int)(this->green*mul*0.5)+0.5;
			this->blue=(int)(this->blue*mul*0.5)+0.5;
			return;
		}
		if(big==this->green) {
			this->red=(int)(this->red*mul*0.5)+0.5;
			this->green=(int)(this->green*mul)+0.5;
			this->blue=(int)(this->blue*mul*0.5)+0.5;
			return;
		}
		if(big==this->blue) {
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
	namespace colors {
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
