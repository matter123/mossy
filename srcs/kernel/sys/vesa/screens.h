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
#include <sys/vesa.h>
#include <hal/multiboot.h>
namespace kernel {
	class screen_32BPPXRGB: public graphics::screen {
	public:
		virtual void flush();
		screen_32BPPXRGB(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_32BPPRGBX: public graphics::screen {
	public:
		virtual void flush();
		screen_32BPPRGBX(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_32BPPXBGR: public graphics::screen {
	public:
		virtual void flush();
		screen_32BPPXBGR(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_32BPPBGRX: public graphics::screen {
	public:
		virtual void flush();
		screen_32BPPBGRX(hal::multiboot_fb *fb_info, uintptr_t addr);
	};

	class screen_24BPPRGB: public graphics::screen {
	public:
		virtual void flush();
		screen_24BPPRGB(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_24BPPBGR: public graphics::screen {
	public:
		virtual void flush();
		screen_24BPPBGR(hal::multiboot_fb *fb_info, uintptr_t addr);
	};

	class screen_16BPPRGB565: public graphics::screen {
	public:
		virtual void flush();
		screen_16BPPRGB565(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_16BPPBGR565: public graphics::screen {
	public:
		virtual void flush();
		screen_16BPPBGR565(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_16BPPXRGB1555: public graphics::screen {
	public:
		virtual void flush();
		screen_16BPPXRGB1555(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_16BPPRGBX5551: public graphics::screen {
	public:
		virtual void flush();
		screen_16BPPRGBX5551(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_16BPPXBGR1555: public graphics::screen {
	public:
		virtual void flush();
		screen_16BPPXBGR1555(hal::multiboot_fb *fb_info, uintptr_t addr);
	};
	class screen_16BPPBGRX5551: public graphics::screen {
	public:
		virtual void flush();
		screen_16BPPBGRX5551(hal::multiboot_fb *fb_info, uintptr_t addr);
	};

	class screen_default: public graphics::screen {
		vesa_mode_info *mi;
		hal::multiboot_fb *fb_info;
	public:
		virtual void flush();
		screen_default(hal::multiboot_fb *fb_info, vesa_mode_info *mode_info, uintptr_t addr);
	};
}
