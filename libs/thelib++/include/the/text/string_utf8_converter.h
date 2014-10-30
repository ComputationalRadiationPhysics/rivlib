/*
 * the/text/string_utf8_converter.h
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of TheLib, TheLib Team, nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef THE_TEXT_STRING_UTF8_CONVERTER_H_INCLUDED
#define THE_TEXT_STRING_UTF8_CONVERTER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_converter"
#endif /* THE_STRING_H_INCLUDED */


namespace the {
namespace text {

    /**
     * Static utility class converting between ANSI, Unicode, and Utf8 strings
     */
    class string_utf8_converter {
    public:

        /**
         * Converts the UTF-8 encoded string 'src' to the ANSI string 'dst'
         *
         * @param dst The ANSI string object to receive the string data
         * @param src The source of the string data
         * @param defChar The default character to be used for characters
         *                which cannot be represented by an ANSI string with
         *                the current system locale
         *
         * @return True on success, false on any error
         */
        static inline bool convert_from_utf8(the::astring& dst,
                const the::astring& src, const char defChar = '?') {
            return convert_from_utf8(dst, src.c_str(), src.length(), defChar);
        }

        /**
         * Converts the UTF-8 encoded string 'src' to the ANSI string 'dst'
         *
         * @param dst The ANSI string object to receive the string data
         * @param src The source of the string data
         * @param defChar The default character to be used for characters
         *                which cannot be represented by an ANSI string with
         *                the current system locale
         *
         * @return True on success, false on any error
         */
        static inline bool convert_from_utf8(the::astring& dst,
                const char *src, const char defChar = '?') {
            return convert_from_utf8(dst, src, ::strlen(src), defChar);
        }

        /**
         * Converts the UTF-8 encoded string 'src' to the ANSI string 'dst'
         *
         * @param dst The ANSI string object to receive the string data
         * @param src The source of the string data
         * @param len The length of the source string in bytes
         * @param defChar The default character to be used for characters
         *                which cannot be represented by an ANSI string with
         *                the current system locale
         *
         * @return True on success, false on any error
         */
        static bool convert_from_utf8(the::astring& dst, const char *src,
            size_t len, const char defChar = '?');

        /**
         * Converts the UTF-8 encoded string 'src' to the unicode string 'dst'
         *
         * @param dst The unicode string object to receive the string data
         * @param src The source of the string data
         *
         * @return True on success, false on any error
         */
        static inline bool convert_from_utf8(the::wstring& dst,
                const the::astring& src) {
            return convert_from_utf8(dst, src.c_str(), src.length());
        }

        /**
         * Converts the UTF-8 encoded string 'src' to the unicode string 'dst'
         *
         * @param dst The unicode string object to receive the string data
         * @param src The source of the string data
         *
         * @return True on success, false on any error
         */
        static inline bool convert_from_utf8(the::wstring& dst,
                const char *src) {
            return convert_from_utf8(dst, src, ::strlen(src));
        }

        /**
         * Converts the UTF-8 encoded string 'src' to the unicode string 'dst'
         *
         * @param dst The unicode string object to receive the string data
         * @param src The source of the string data
         * @param len The length of the source string in bytes
         *
         * @return True on success, false on any error
         */
        static bool convert_from_utf8(the::wstring& dst, const char *src,
            size_t len);

        /**
         * Converts 'src' to the UTF-8 string 'dst'
         *
         * @param dst The UTF-8 string object to receive the string data
         * @param src The source of the string data
         *
         * @return True on success, false on any error
         */
        static inline bool convert_to_utf8(the::astring& dst,
                const the::astring& src) {
            return convert_to_utf8(dst, src.c_str(), src.length());
        }

        /**
         * Converts 'src' to the UTF-8 string 'dst'
         *
         * @param dst The UTF-8 string object to receive the string data
         * @param src The source of the string data
         *
         * @return True on success, false on any error
         */
        static inline bool convert_to_utf8(the::astring& dst, const char *src) {
            return convert_to_utf8(dst, src, ::strlen(src));
        }

        /**
         * Converts 'src' to the UTF-8 string 'dst'
         *
         * @param dst The UTF-8 string object to receive the string data
         * @param src The source of the string data
         * @param len The length of the source string in characters
         *
         * @return True on success, false on any error
         */
        static bool convert_to_utf8(the::astring& dst, const char *src,
            size_t len);

        /**
         * Converts 'src' to the UTF-8 string 'dst'
         *
         * @param dst The UTF-8 string object to receive the string data
         * @param src The source of the string data
         *
         * @return True on success, false on any error
         */
        static inline bool convert_to_utf8(the::astring& dst,
                const the::wstring& src) {
            return convert_to_utf8(dst, src.c_str(), src.length());
        }

        /**
         * Converts 'src' to the UTF-8 string 'dst'
         *
         * @param dst The UTF-8 string object to receive the string data
         * @param src The source of the string data
         *
         * @return True on success, false on any error
         */
        static inline bool convert_to_utf8(the::astring& dst,
                const wchar_t *src) {
            return convert_to_utf8(dst, src, ::wcslen(src));
        }

        /**
         * Converts 'src' to the UTF-8 string 'dst'
         *
         * @param dst The UTF-8 string object to receive the string data
         * @param src The source of the string data
         * @param len The length of the source string in characters
         *
         * @return True on success, false on any error
         */
        static bool convert_to_utf8(the::astring& dst, const wchar_t *src,
            size_t len);

    protected:

        /** internally used 'Size' type */
        typedef long Size;

        /** forbidden Ctor */
        string_utf8_converter(void);

        /** forbidden Ctor */
        string_utf8_converter(const string_utf8_converter& srv);

        /** forbidden Dtor */
        ~string_utf8_converter(void);

        /**
         * Calculates the needed size in Bytes to store the given ANSI string
         * in UTF8 coding, including the terminating zero.
         *
         * Note: This implementation should only be used if you are using 7-bit
         * ANSI strings. If you are using any 8-Bit ANSI characters, the whole
         * string must be converted to a Unicode wide string!
         *
         * @param str The input string.
         * @param len The length of 'str' in characters
         *
         * @return The size in Bytes needed to store the given string, or 
         *         negative if the string contains characters not 
         *         representable in UTF8.
         */
        static Size calc_utf8_size(const char *str, size_t len);

        /**
         * Calculates the needed size in Bytes to store the given UNICODE 
         * string in UTF8 coding, including the terminating zero.
         *
         * Note: This implementation should only be used if you are using 7-bit
         * ANSI strings. If you are using any 8-Bit ANSI characters, the whole
         * string must be converted to a Unicode wide string!
         *
         * @param str The input string.
         * @param len The length of 'str' in characters
         *
         * @return The size in Bytes needed to store the given string, or 
         *         negative if the string contains characters not 
         *         representable in UTF8.
         */
        static Size calc_utf8_size(const wchar_t *str, size_t len);

        /**
         * Answer the string length in character of the given UTF8-String, 
         * excluding the terminating zero. If the string is not a valid
         * UTF8-String, teh return value is negative.
         *
         * @param str The UTF8-String.
         * @param len The length of 'str' in bytes
         *
         * @return The length of the string, or negative on error.
         */
        static Size string_length(const char *str, size_t len);

    };

} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_UTF8_CONVERTER_H_INCLUDED */
