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
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char *str,const char *fmt, ...);
int vsprintf(char *str,const char *fmt, va_list args);
int vsnprintf(char *str,size_t max,const char *fmt, va_list args);
typedef size_t (*cb_callback)(void *pass,const char *buf,size_t len);
int cbprintf(void *pass, cb_callback cb, const char *fmt, ...);
int vcbprintf(void *pass, cb_callback cb, const char *fmt, va_list arg);
#ifdef __cplusplus
}
#endif
