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
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
////////////////////////////////////////
// Counts the number of bytes in the  //
//   char 'str'                       //
// Returns the count                  //
// Created on 2014-06-13              //
////////////////////////////////////////
size_t get_char_len(const char *str);

////////////////////////////////////////
// Gets the next char in the string   //
//   'str'                            //
// Returns the next char              //
// Created on 2014-06-13              //
////////////////////////////////////////
char *next_char(const char *str);

////////////////////////////////////////
// Gets the next 'count' char in the  //
//   string 'str'                     //
// Returns the next char              //
// Created on 2014-06-13              //
////////////////////////////////////////
char *get_next_char(const char *str, size_t count);

////////////////////////////////////////
// Determines if two chars are equal  //
// Returns true if equal, false       //
//   otherwise                        //
// Created on 2014-06-13              //
////////////////////////////////////////
bool is_char_equal(const char *c1, const char *c2);

////////////////////////////////////////
// Copies char 'c' onto string 'str'  //
// Created on 2014-06-14              //
////////////////////////////////////////
void char_copy(char *str, const char *c);

////////////////////////////////////////
// Decodes the char 'c' into a        //
//   codepoint                        //
// Returns the decoded codepoint      //
// Created on 2014-06-14              //
////////////////////////////////////////
uint32_t decode_char(const char *c);

////////////////////////////////////////
// Encodes the codepoint 'code_point' //
//   and copies the char onto         //
//   'buffer'                         //
// Returns the length of the encoded  //
//   char                             //
// Created on 2014-07-05              //
////////////////////////////////////////
size_t encode_char(uint32_t code_point,char *buffer);


////////////////////////////////////////
// Encodes the codepoint 'code_point' //
//   as a 5 byte UTF8 overlong char   //
//   and copies the char onto         //
//   'buffer'                         //
//   char                             //
// Created on 2014-07-05              //
////////////////////////////////////////
void encode_five(uint32_t code_point,char *buffer);

////////////////////////////////////////
// Copies the string 'src' into the   //
//   string 'dest' and normalizes     //
//   overlong chars, except special   //
//   5 byte chars                     //
// Returns the number of bytes copied //
//   onto 'dest'                      //
// Created on 2014-07-06              //
////////////////////////////////////////
size_t normalize(char *dest, const char *src);

////////////////////////////////////////
// Copies the string 'src' into the   //
//   string 'dest' and normalizes     //
//   overlong chars, except special   //
//   5 byte chars, copies up to 'num' //
//   bytes onto 'dest'                //
// Returns the number of bytes copied //
//   onto 'dest'                      //
// Created on 2014-07-06              //
////////////////////////////////////////
size_t normalizen(char *dest, const char *src, size_t num);

#ifdef __cplusplus
}
#endif