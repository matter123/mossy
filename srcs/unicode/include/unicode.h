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
#include <stdbool.h>
namespace unicode {
	/**
	 * @file
	 * @brief utility functions and decelerations for Unicode
	 */

	/**
	 * @typedef
	 * @brief a single Unicode codepoint, representing 1 char
	 */
	typedef uint32_t codepoint;

	/**
	 * @brief the accepted encodings
	 */
	enum encoding {
		/** basic ascii 0-127*/
		ASCII   = 0x01,
		/** UTF8 */
		UTF8    = 0x01,
		/** UTF16 little endian*/
		UTF16LE = 0x02,
		/** UTF16 big endian*/
		UTF16BE = 0x04,
		/** UTF16 unknown endian*/
		UTF16   = 0x06,
		/** UTF32 little endian*/
		UTF32LE = 0x08,
		/** UTF32 big endian*/
		UTF32BE = 0x10,
		/** UTF16 unknown endian*/
		UTF32   = 0x18,
		/** Unknown encoding*/
		UNKNOWN = 0xFF,
	};
	/**
	 * @brief detects encoding of \a stream
	 * @details using various properties of each format
	 * guess the encoding of the stream
	 *
	 * @param stream pointer to a stream of bytes
	 * @param len length of stream
	 *
	 * @return the detected encoding, or UNKOWN if unable
	 * to detect encoding
	 * @date created on 2014-10-18
	 */
	enum encoding detect_encoding(char *stream, size_t len);

	/**
	 * @note currently not implemented
	 * @brief general purpose encoding conversion
	 * @details converts a stream between two different encodings
	 *
	 * @param stream input stream of bytes
	 * @param in_len input stream length
	 * @param from best guess of input stream encoding
	 * @param to desired output encoding
	 * @param out_stream pointer to output stream of bytes
	 * @param out_len max length of output stream
	 *
	 * @note if both out_stream and out_len are NULL and 0, respectively
	 * \a convert will allocate a stream of the required length, the caller
	 * is responsible to \a free the stream when done
	 *
	 * @return number of bytes copied to \a out_stream if the values
	 * is larger than \a out_len, the output stream was not big enough
	 * to contain the entire input stream, and the value returned is how
	 * big the output stream needs to be
	 *
	 * @date created on 2014-10-19
	 */
	int convert(char *stream, size_t in_len, enum encoding from,
	            enum encoding to, char **out_stream, size_t out_len);

	void copy_codeunit(char *dest, char *src);
}
