/*
 * Copyright 2013 Matthew Fosdick

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

#ifndef PIT_H
#define PIT_H

namespace x86 {

	enum pit_freq{
		PIT_FREQ_05MS, PIT_FREQ_10MS, PIT_FREQ_15MS, PIT_FREQ_20MS
	};

	void init_pit(int freq);
}
#endif
