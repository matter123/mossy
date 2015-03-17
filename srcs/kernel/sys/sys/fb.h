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
	void init_fb();
	/**
	 * @brief copies rect_buf onto the screen at x, y
	 * copies the pixels from src_buf onto the screen at x, y
	 * src_buf must be packed pixels and in R1R1R1R1R2R2R2R2R3R3R3R3 form
	 * @param dest_x   destionation x location
	 * @param dest_y   destionation y location
	 * @param w        width of rectangle
	 * @param h        height of rectangle
	 * @param rect_buf location of start of source buffer x*y*bpp bytes big
	 */
	void copy_rect(int dest_x,int dest_y,
	               int w,int h,
	               void *rect_buf);
	/**
	 * @brief copies one part of the screen to another
	 * copies video memory from \a src_x, \a src_y to \a dest_x, \a dest_y
	 * in a rectangle the size of \a w x \a h
	 * @param src_x  x of the source rectangle
	 * @param src_y  y of the source rectangle
	 * @param w      width of the rectangle being copied
	 * @param h      height of the rectangle being copied
	 * @param dest_x x of the destionation rectangle
	 * @param dest_y y of the destionation rectangle
	 */
	void copy_rect(int src_x,int src_y,
	               int w,int h,
	               int dest_x, int dest_y);
	enum bit_blit_op {
		AND,
		OR,
		NOT,
		XOR,
	};
	/**
	 * put a single pixel onto the screen, not reccomended for normal operation
	 * @param x     x coordinate of pixel
	 * @param y     y coordinate of pixel
	 * @param color packed color for pixel
	 */
	void put_pixel(int x, int y, uint32_t color);
	/**
	 * performs a bit blit operation over a rectangle onto the screen
	 * src_buf must be packed pixels and in R1R1R1R1R2R2R2R2R3R3R3R3 form
	 * @param dest_x  x of the destionation rectangle
	 * @param dest_y  y of the destionation rectangle
	 * @param w       width of rectangle
	 * @param h       height of rectangle
	 * @param src_buf location of start of source buffer x*y*bpp bytes big
	 * @param op      bit_blit operation to perform
	 */
	void bit_blit(int dest_x,int dest_y,
	              int w,int h,
	              void *src_buf, bit_blit_op op);
	/**
	 * perorms a bit blit operation over a rectangle and onto another
	 * src_buf and dest_buf must be packed pixels and in R1R1R1R1R2R2R2R2R3R3R3R3 form
	 * @param w        width of rectangle
	 * @param h        height of rectangle
	 * @param dest_buf location of start of destionation buffer x*y*bpp bytes big
	 * @param src_buf  location of start of source buffer x*y*bpp bytes big
	 * @param op       bit blit operation to perform
	 */
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
