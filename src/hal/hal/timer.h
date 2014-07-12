/*
    Copyright 2013 Matthew Fosdick

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
#include <stdint.h>

namespace hal {

	bool timer_init(uint32_t freq);

	typedef void (*timer_tick)(uint64_t tick);

	bool install_timer_callback(timer_tick callback, int tick_mod);

	bool reset_counter();

	uint64_t get_counter();

	uint32_t get_timer_freq();
}