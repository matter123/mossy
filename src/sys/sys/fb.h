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
namespace kernel {
	void init_fb();

	void copy_rect(int dest_x,int dest_y,
	               int w,int h,
	               void *rect_buf);
	void copy_rect(int src_x,int src_y,
	               int w,int h,
	               int dest_x, int dest_y);
	enum bit_blit_op {
		AND,
		OR,
		NOT,
		XOR,
	};
	void put_pixel(int x, int y, uint32_t color);

	void bit_blit(int dest_x,int dest_y,
	              int w,int h,
	              void *src_buf, bit_blit_op op);
	void bit_blit(int w,int h,
	              void *dest_buf,void *src_buf,
	              bit_blit_op op);

	uint32_t pack_rgb(uint8_t r,uint8_t g,uint8_t b);

	void fill_rect(int x,int y,int w,int h, uint32_t packed_color);
	void fill_hline(int x,int y,int w,uint32_t packed_color);
	void fill_vline(int x,int y,int h,uint32_t packed_color);

	void reset_fb();
	void reset_fb(int x, int y, int w, int h);
	void reset_fb(int x, int y, int w, int h, int img_x, int img_y);

	int get_w();
	int get_h();
	int get_bpp();
}