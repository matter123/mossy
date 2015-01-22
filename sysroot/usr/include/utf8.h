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

/**
    @file
    @brief utility functions for handling UTF-8 chars
    @details collection of functions to
    - \link get_char_len get char length \endlink
    - \link next_char get next char in a string \endlink
    - \link is_char_equal determine equality of 2 chars \endlink
    - \link decode_char decode UTF-8 \endlink
    - \link encode_char encode UTF-8 \endlink
    - \link normalize normalize a UTF-8 string \endlink
    - and a few others
*/

namespace unicode {
	namespace utf8 {
		/**
		    @brief counts number of bytes in char
		    @details counts the number of bytes in
		    the first char of \a str

		    @param str pointer to a UTF-8 char

		    @return a number between 1 and 5
		    @date created on 2014-06-13
		*/
		size_t get_char_len(const char *str);

		/**
		    @brief gets the next char in a string
		    @details advances \a str by the length in
		    bytes of the char \a str currently points at

		    @param str pointer to a string of UTF-8 chars

		    @return pointer to a string of UTF-8 chars
		    @date created on 2014-06-13
		    @sa get_next_char
		*/
		char *next_char(const char *str);

		/**
		    @brief skips chars in string
		    @details advances \a str by the number of
		    bytes equal to \a count chars

		    @param str pointer to string of UTF-8 chars
		    @param count number of chars to skip

		    @return pointer to a string of UTF-8 chars
		    @date created on 2014-06-13
		    @sa next_char
		*/
		char *get_next_char(const char *str, size_t count);

		/**
		    @brief determines if two chars are the same
		    @details checks if \a c1 and \a c2 have the same
		    length and byte pattern, if they don't it also
		    checks their code points for equality

		    @param c1 pointer to a UTF-8 char
		    @param c2 pointer to a UTF-8 char

		    @return true if \a c1 and \a c2 are equal
		    @date created on 2014-06-13
		*/
		bool is_char_equal(const char *c1, const char *c2);

		/**
		    @brief copies char from \a c to \a str
		    @details copies the number of bytes equal
		    to the length of \a c to \a str

		    @param str pointer to a string of UTF-8 chars
		    @param c pointer to a UTF-8 char

		    @date created on 2014-06-14
		*/
		void char_copy(char *str, const char *c);

		/**
		    @brief turns \a c into codepoint
		    @details uses the method outlined in
		    <a href="http://tools.ietf.org/html/rfc3629">RFC 3629</a>
		    to turn a UTF-8 char into
		    a codepoint

		    @param c pointer to a UTF-8 char

		    @return the codepoint of the passed char
		    or zero if the encoding fails
		    @date created on 2014-06-14
		    @sa encode_char
		*/
		uint32_t decode_char(const char *c);

		/**
		    @brief turns \a code_point into a char
		    @details uses the method outlined in
		    <a href="http://tools.ietf.org/html/rfc3629">RFC 3629</a>
		    to turn a codepoint into
		    a UTF-8 char, and copy onto \a buffer

		    @param code_point codepoint to encode
		    @param buffer pointer to a block of memory
		    at least 5 bytes big

		    @return the number of bytes copied onto the buffer
		    @date created on 2014-07-05
		    @sa encode_five
		    @sa decode_char
		*/
		size_t encode_char(uint32_t code_point,char *buffer);

		/**
		    @brief turns \a code_point into a five
		    byte UTF-8 char
		    @details using <a href="http://tools.ietf.org/html/rfc2279">RFC 2279</a>,
		    turn \a code_point to an overlong five
		    byte UTF-8 char and copy onto \a buffer

		    @param code_point codepoint to encode
		    @param buffer pointer to a block of memory
		    at least 5 bytes big

		    @date created on 2014-07-05
		    @sa encode_char
		*/
		void encode_five(uint32_t code_point,char *buffer);

		/**
		    @brief normalizes overlong chars
		    @details copies \a src into \a dest,
		    while normalizes overlong chars, with
		    the exception of 5 byte control chars

		    @param dest pointer to a buffer at least the size of src
		    @param src pointer to the source buffer

		    @return the new size of the UTF-8 string
		    @date created on 2014-07-06
		    @sa normalizen
		*/
		size_t normalize(char *dest, const char *src);

		/**
		    @brief normalizes overlong chars
		    @details copies \a src into \a dest,
		    while normalizing overlong chars

		    @param dest pointer to a buffer at least the size of src
		    @param src pointer to the source buffer

		    @return the new size of the UTF-8 string
		    @date created on 2014-07-06
		    @sa normalize
		*/
		size_t normalizen(char *dest, const char *src, size_t num);
	}
}
