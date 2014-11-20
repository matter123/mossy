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
#include <test.h>
#if TEST
#include <hal/hal.h>
void test_utf8() {
}
static volatile kernel::test_module tm=kernel::test_module("UTF-8       ",&test_utf8);
#endif