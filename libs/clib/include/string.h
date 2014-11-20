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
#include <stddef.h>
#include <stdint.h>
/**
    @file
    @brief C style string functions
    @details most string functions in <string.h>
    along with UTF-8 variants and safe copy functions
*/
#ifdef __cplusplus
extern "C" {
#endif

/**
    @brief copies memory
    @details using a left-to-right copy,
    copy \a num bytes from \a src to \a
    dest

    @param dest pointer to a block of memory at least
    \a num bytes big, MUST NOT overlap with \a src
    @param src pointer to a block of memory at least
    \a num bytes big, MUST NOT overlap with \a dest
    @param num number of bytes to copy from \a src to \a dest

    @return a pointer equal to \a dest
    @date created on 2014-06-13
    @sa memmove
*/
void *memcpy(void *dest, const void *src, size_t num);

/**
    @brief copies potentially overlapping memory
    @details using a left-to-right or right-to-left copy,
    copy \a num bytes from \a src to \a
    dest safely

    @param dest pointer to a block of memory at least
    \a num bytes big, MAY overlap with \a src
    @param src pointer to a block of memory at least
    \a num bytes big, MAY overlap with \a dest
    @param num number of bytes to copy from \a src to \a dest

    @return pointer equal to \a dest
    @date created on 2014-06-13
    @sa memcpy
*/
void *memmove(void *dest, const void *src, size_t num);

/**
    @brief copies chars from \a src to \a dest
    @details copies chars from \a src to \a dest
    stopping the copy when a NULL byte is found

    @param dest pointer to buffer of memory
    @param src pointer to a string of UTF-8 chars

    @return pointer equal to \a dest
    @date created on 2014-06-13
    @sa strncpy
    @sa strlcpy
    @sa mbsncpy
*/
char *strcpy(char *dest, const char *src);

/**
    @brief copies \a num bytes from \a src to \a dest
    @details copies \a num bytes from \a src to \a dest
    if a NULL byte is found when copying, then the rest
    of \dest is filled with NULL bytes

    @note this function DOES NOT guarantee that \a dest
    will end with a NULL byte

    @param dest pointer to a buf of memory at least \a num bytes big
    @param src pointer to a string of UTF-8 chars
    @param num number of bytes to fill \a dest with

    @return pointer equal to \a dest
    @date created on 2014-06-13
    @sa strcpy
    @sa strlcpy
    @sa mbsncpy
*/
char *strncpy(char *dest, const char *src, size_t num);

/**
    @brief copies upto \a num bytes from \a src to \a dest
    @details copies upto \a num bytes from \a src to \dest
    if a NULL byte is found when coping, then the rest of
    \a dest will be filled with NULL bytes. If the number
    of bytes remaining to copy is less then the length of
    the current char plus 1, then the current char is not
    copied and the rest of \a dest is filled with NULL bytes

    @note this function DOES guarantee that \a dest
    will end with a NULL byte

    @param dest pointer to a block of memory at least \a num
    bytes big
    @param src pointer to a string of UTF-8 chars
    @param num number of bytes to fill \a dest with

    @return pointer equal to \a dest
    @date created on 2014-06-13
    @sa strcpy
    @sa strncpy
    @sa mbsncpy
*/
char *strlcpy(char *dest, const char *src, size_t num);

/**
    @brief copies upto \a num bytes from \a src to \a dest
    @details copies upto \a num bytes from \a src to \dest
    if a NULL byte is found when coping, then the rest of
    \a dest will be filled with NULL bytes. If the number
    of bytes remaining to copy is less then the length of
    the current char, then the current char is not copied
    and the rest of \a dest is filled with NULL bytes

    @note this function DOES NOT guarantee that \a dest
    will end with a NULL byte

    @param dest pointer to a block of memory at least \a num
    bytes big
    @param src pointer to a string of UTF-8 chars
    @param num number of bytes to fill \a dest with

    @return pointer equal to \a dest
    @date created on 2014-06-13
    @sa strcpy
    @sa strncpy
    @sa strlcpy
*/
char *mbsncpy(char *dest, const char *src, size_t num);

/**
    @brief appends \a src onto \a dest
    @details appends the entire string of chars in \a src
    onto the end of \a dest

    @param dest pointer to a string of UTF-8 chars
    @param src pointer to a string of UTF-8 chars

    @return pointer equal to \a dest
    @date created on 2014-05-30
    @sa strncat
    @sa strlcat
*/
char *strcat(char *dest, const char *src);

/**
    @brief appends \a src onto \a dest
    @details appends the first \a num bytes of \a src onto \a dest

    @note this function DOES guarantee that \a dest
    will end with a NULL byte

    @param dest pointer to a string of UTF-8 chars
    @param src pointer to a string of UTF-8 chars

    @return pointer equal to \a dest
    @date created on 2014-05-30
    @sa strcat
    @sa strlcat
*/
char *strncat(char *dest, const char *src, size_t num);

/**
    @brief appends \a src onto \a dest
    @details appends the first \a num bytes of \a src onto \a dest
    if the number of bytes left to copy is less then the current char's length + 1
    then the current char is not copied, and the rest of \a dest is filled with
    NULL bytes

    @param dest pointer to a string of UTF-8 chars
    @param src pointer to a string of UTF-8 chars
    @param num number of bytes to copy

    @return pointer equal to \a dest
    @date created on 2014-06-13
    @sa strcat
    @sa strncat
*/
char *strlcat(char *dest, const char *src, size_t num);

/**
    @brief compares the bytes of \a ptr1 and \a ptr2
    @details compares the first \a num bytes of \a ptr and \a ptr
    if all bytes are equal, this returns a 0. If a byte is different
    it returns *\a ptr1 - *\a ptr2

    @param ptr1 pointer to a block of memory
    @param ptr2 pointer to a block of memory
    @param num number of bytes to compare

    @return 0 if the bytes of of \a ptr1 and \a ptr2 are the same, else *\a ptr1 - *\a ptr2
    @date created on 2014-05-30
    @sa strcmp
    @sa mbscmp
*/
int memcmp(const void *ptr1, const void *ptr2, size_t num);

/**
    @brief compares \a str1 and \a str2
    @details compares byte by byte of each string
    \a str1 and \a str2 stopping at the first NULL byte

    @param str1 pointer to a string of UTF-8 chars
    @param str2 pointer to another string of UTF-8 char

    @return 0 if both strings are equal, the difference
    of the first different byte if not
    @date created on 2014-05-30
    @sa memcmp
    @sa mbscmp
*/
int strcmp(const char *str1, const char *str2);

/**
    @brief compares \a str1 and \a str2
    @details compares each char of \a str1 and \a str2
    two chars are considered equal if after decoding they
    result in the same codepoint

    @note this method counts combining char pairs as separate
    chars from there precombinded versions

    @param str1 pointer to a UTF-8 string
    @param str2 pointer to another UTF-8 string

    @return 0 if the strings are equal, the difference
    in the first differing codepoint otherwise
    @date created on 2014-07-06
    @sa memcmp
    @sa strcmp
*/
int mbscmp(const char *str1, const char *str2);

/**
    @brief compares 2 strings
    @details compares up to num bytes of \a str1 and
    \a str2.

    @param str1 pointer to a UTF-8 string
    @param str2 pointer to a UTF-8 string
    @param num max number of bytes to compare

    @return 0 if strings are equal the diffrence in the first byte otherwise
    @date created on 2014-05-30
    @sa mbscmp
    @sa strcmp
    @sa mbsncmp
*/
int strncmp(const char *str1, const char *str2, size_t num);

/**
    @brief compares 2 strings
    @details compares up to num bytes of \a str1 and
    \a str2. the two strings are compared codepoint to codepoint
    combining chars are ignored

    @param str1 pointer to a UTF-8 string
    @param str2 pointer to a UTF-8 string
    @param num max number of bytes to compare

    @return 0 if strings are equal the diffrence in the first codepoint otherwise
    @date created on 2014-05-30
    @sa mbscmp
    @sa strcmp
    @sa strncmp
*/
int mbsncmp(const char *str1, const char *str2, size_t num); //WORKS

/**
    @brief finds a byte
    @details searches the first num bytes pointed to by
    \a ptr and looks for the bytes with the value of
    \a value

    @param ptr pointer to a block of bytes
    @param value value of byte to look for capped at 0xFF
    @param num number of bytes to search

    @return a pointer to the first matching byte, NULL
    if it not found
    @date created on 2014-05-30
    @sa strchr
    @sa mbschr
*/
void *memchr(const void *ptr, int value, size_t num);

////////////////////////////////////////
// Searches in 'str' for a byte with  //
//   the value of 'value'             //
// Returns a pointer to the first     //
//   byte that matches or NULL        //
// Created on 2014-05-30              //
////////////////////////////////////////
char *strchr(const char *str, int value);

////////////////////////////////////////
// Searches in 'str' for a char with  //
//   the value of 'value'             //
// Returns a pointer to the first     //
//   char that matches or NULL        //
// Created on 2014-07-06              //
////////////////////////////////////////
char *mbschr(const char *str, const char *value);

////////////////////////////////////////
// Searches 'str' for any bytes in    //
//   'bytes'                          //
// Returns the number of bytes before //
//   the first match, or the length   //
//   in bytes of the string in 'str'  //
// Created on 2014-05-30              //
////////////////////////////////////////
size_t strcspn(const char *str, const char *bytes);

////////////////////////////////////////
// Searches 'str' for any chars in    //
//   'chars'                          //
// Returns the number of chars before //
//   the first match, or the number   //
//   of chars in the string in 'str'  //
// Created on 2014-07-06              //
////////////////////////////////////////
size_t mbscspn(const char *str, const char *chars);

////////////////////////////////////////
// Searches 'str' for any bytes in    //
//   'bytes'                          //
// Returns a pointer to the first     //
//   byte that matches, or NULL       //
// Created on 2014-05-30              //
////////////////////////////////////////
char *strpbrk(const char *str1, const char *bytes);

////////////////////////////////////////
// Searches 'str' for any chars in    //
//   'chars'                          //
// Returns a pointer to the first     //
//   char that matches, or NULL       //
// Created on 2014-07-06              //
////////////////////////////////////////
char *mbspbrk(const char *str1, const char *chars);

////////////////////////////////////////
// Searches in 'str' for a byte with  //
//   the value of 'value'             //
// Returns a pointer to the last      //
//   byte that matches or NULL        //
// Created on 2014-05-30              //
////////////////////////////////////////
char *strrchr(const char *str1, int value);

////////////////////////////////////////
// Searches in 'str' for a char with  //
//   the value of 'value'             //
// Returns a pointer to the last      //
//   char that matches or NULL        //
// Created on 2014-07-06              //
////////////////////////////////////////
char *mbsrchr(const char *str1, const char *value);

////////////////////////////////////////
// Searches 'str' for any bytes in    //
//   'bytes'                          //
// Returns length of initial portion  //
//   that contains all the bytes in   //
//   'bytes'                          //
// Created on 2014-05-30              //
////////////////////////////////////////
size_t strspn(const char *str, const char *bytes);

////////////////////////////////////////
// Searches 'str' for any chars in    //
//   'chars'                          //
// Returns number of chars in initial //
//   portion that contains all the    //
//   chars in 'chars'                 //
// Created on 2014-05-30              //
////////////////////////////////////////
size_t mbsspn(const char *str, const char *chars);

////////////////////////////////////////
// Searches 'str1' for all bytes in   //
//   'str2'                           //
// Returns pointer to the first       //
//   portion of 'str1' that matches   //
//   all the bytes in 'str2' except   //
//   the null byte                    //
// Created on 2014-05-30              //
////////////////////////////////////////
char *strstr(const char *str1, const char *str2);

////////////////////////////////////////
// Searches 'str1' for all chars in   //
//   'str2'                           //
// Returns pointer to the first       //
//   portion of 'str1' that matches   //
//   all the chars in 'str2'          //
// Created on 2014-07-06              //
////////////////////////////////////////
char *mbsstr(char *str1, const char *str2);

////////////////////////////////////////
// TODO                               //
// Returns TODO                       //
// Created on 2014-05-31              //
////////////////////////////////////////
char *strtok(char *str, const char *delim);

////////////////////////////////////////
// TODO                               //
// Returns TODO                       //
// Created on 2014-07-06              //
////////////////////////////////////////
char *mbstok(char *str, const char *delim);

////////////////////////////////////////
// Sets 'num' bytes, starting from    //
//   'ptr', to 'value'                //
// Returns 'ptr'                      //
// Created on 2014-05-31              //
////////////////////////////////////////
void *memset(void *ptr,int value,size_t num);

////////////////////////////////////////
// Sets 'num' words, starting from    //
//   'ptr', to 'value'                //
// Returns 'ptr'                      //
// Created on 2014-05-31              //
////////////////////////////////////////
void *memset16(uint16_t *ptr,uint16_t value,size_t num);

////////////////////////////////////////
// Sets 'num' dwords, starting from   //
//   'ptr', to 'value'                //
// Returns 'ptr'                      //
// Created on 2014-05-31              //
////////////////////////////////////////
void *memset32(uint32_t *ptr,uint32_t value,size_t num);

////////////////////////////////////////
// Sets 'num' qwords, starting from   //
//   'ptr', to 'value'                //
// Returns 'ptr'                      //
// Created on 2014-05-31              //
////////////////////////////////////////
void *memset64(uint64_t *ptr,uint64_t value,size_t num);


////////////////////////////////////////
// Counts the number of bytes in      //
//   'str'                            //
// Returns the count                  //
// Created on 2014-05-31              //
////////////////////////////////////////
size_t strlen(const char *str);

////////////////////////////////////////
// Counts the number of chars in      //
//   'str'                            //
// Returns the count                  //
// Created on 2014-05-31              //
////////////////////////////////////////
size_t mbslen(const char *str);
#ifdef __cplusplus
}
#endif
