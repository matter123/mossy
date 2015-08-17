/*
    Copyright 2015 Matthew Fosdick

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
#include <graphics/framebuffer.h>
namespace kernel {
	void init_vesa();
	struct vesa_mode_info {
		uint16_t attributes;
		uint8_t winA,winB;
		uint16_t granularity;
		uint16_t winsize;
		uint16_t segmentA, segmentB;
		uint32_t realFctPtr;
		uint16_t pitch;
		uint16_t Xres, Yres;
		uint8_t Wchar, Ychar, planes, bpp, banks;
		uint8_t memory_model, bank_size, image_pages;
		uint8_t reserved0;
		uint8_t red_mask, red_pos;
		uint8_t green_mask, green_pos;
		uint8_t blue_mask, blue_pos;
		uint8_t rsv_mask, rsv_pos;
		uint8_t directcolor_attributes;
		uint32_t physbase;
		uint32_t reserved1;
		uint16_t reserved2;
	}PACKED;
	extern graphics::screen *screen;
}
