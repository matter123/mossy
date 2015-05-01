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
namespace kernel {
	enum packing {
		BGRX,
		XRGB,
		BGR,
		RGB,
		BGRX_16,
		BGR_16,
		XRGB_16,
		RGB_16,
		SCREEN,
	};
	enum write_policy {
		READ_ONLY,
		READ_WRITE,
		WRITE_ONLY,
	};
	enum copy_mode {
		PASSTHROUGH,
		TILE,
		STRETCH,
		IGNORE,
		FAST,
	};
	enum bit_blit_op {
		AND,
		OR,
		XOR,
		ADD,
		SUB,
		MUL,
		DIV,
		MOD,
	};
	struct color {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
		bool transparent;

		void lighten();
		void super_light();
		void darken();
		void super_dark();
		void pastel();
		void pure();
		void invert();
		color(int _red, int _green, int _blue, bool _transparent = false)
			: red(_red)
			, green(_green)
			, blue(_blue)
			, transparent(_transparent)
			{}
	};

	namespace colors {
		extern color white;
		extern color black;
		extern color red;
		extern color green;
		extern color blue;
		extern color yellow;
		extern color cyan;
		extern color magenta;
	}

	class buffer {
	protected:
		int stride;
		pointer addr;
		write_policy wp;
		packing pack;
		buffer *cow;
		buffer(int width, int height, packing pack=SCREEN, write_policy write=READ_WRITE);
	public:
		int width;
		int height;
		buffer(buffer &b);
		~buffer();
		virtual void fill(color c)=0;
		virtual void copy_from(buffer &b, int x=0, int y=0, copy_mode=PASSTHROUGH)=0;
		virtual void bit_blit(buffer &b, bit_blit_op operation)=0;
	};

	buffer *get_screen();
	buffer *get_front_buffer();
	buffer *get_background();
	buffer *get_background_orig();

	void init_system_fb();
}
