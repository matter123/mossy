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
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief checks if char is alphanumeric
 * @details determines if an ascii char is alphanumeric
 * i.e. '0' through '9' or 'A' through 'Z' or 'a' through 'z'
 *
 * @param c an ascii char
 *
 * @return 1 if alphanumeric, 0 otherwise
 * @sa isalpha
 * @sa isdigit
 * @sa isxdigit
 */
int isalnum(int c);
/**
 * @brief check is char is alphabetic
 * @details determines if an ascii char is alphabetic
 * i.e. 'A' through 'Z' or 'a' through 'z'
 *
 * @param c an ascii char
 *
 * @return 1 if alphabetic, 0 otherwise
 * @sa isalpha
 * @sa isxdigit
 */
int isalpha(int c);
/**
 * @brief checks if character is blank
 * @details determines if a chracter is
 * blank, as in ` ` or `\t`
 *
 * @param c an ascii character
 *
 * @return 1 if blank, 0 otherwise
 */
int isblank(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

int tolower(int c);
int toupper(int c);
#ifdef __cplusplus
}
#endif
