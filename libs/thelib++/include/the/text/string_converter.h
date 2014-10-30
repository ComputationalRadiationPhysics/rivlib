/*
 * the/text/string_converter.h
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

#ifndef THE_TEXT_STRINGCONVERTER_H_INCLUDED
#define THE_TEXT_STRINGCONVERTER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_converter"
#endif /* THE_STRING_H_INCLUDED */

#include "the/config.h"

#include "the/force_inline.h"
#include "the/not_copyable.h"
#include "the/not_instantiable.h"
#include "the/stack_trace.h"

#include "the/text/string_utf8_converter.h"


#ifndef THE_A2W
#define THE_A2W(str) static_cast<const wchar_t *>(\
    ::the::text::string_converter::to_wstr_ptr(str))
#endif /* !THE_A2W */
#ifndef THE_W2A
#define THE_W2A(str) static_cast<const char *>(\
    ::the::text::string_converter::to_astr_ptr(str))
#endif /* !THE_W2A */

#if defined(UNICODE) || defined(_UNICODE)
#ifndef THE_A2T
#define THE_A2T(str) static_cast<const wchar_t *>(\
    ::the::text::string_converter::to_wstr_ptr(str))
#endif /* !THE_A2T */
#ifndef THE_T2A
#define THE_T2A(str) static_cast<const char *>(\
    ::the::text::string_converter::to_astr_ptr(str))
#endif /* !THE_T2A */
#ifndef THE_W2T
#define THE_W2T(str) static_cast<const wchar_t *>(\
    ::the::text::string_converter::to_wstr_ptr(str))
#endif /* !THE_W2T */
#ifndef THE_T2W
#define THE_T2W(str) static_cast<const wchar_t *>(\
    ::the::text::string_converter::to_wstr_ptr(str))
#endif /* !THE_T2W */
#else /* defined(UNICODE) || defined(_UNICODE) */
#ifndef THE_A2T
#define THE_A2T(str) static_cast<const char *>(\
    ::the::text::string_converter::to_astr_ptr(str))
#endif /* !THE_A2T */
#ifndef THE_T2A
#define THE_T2A(str) static_cast<const char *>(\
    ::the::text::string_converter::to_astr_ptr(str))
#endif /* !THE_T2A */
#ifndef THE_W2T
#define THE_W2T(str) static_cast<const char *>(\
    ::the::text::string_converter::to_astr_ptr(str))
#endif /* !THE_W2T */
#ifndef THE_T2W
#define THE_T2W(str) static_cast<const wchar_t *>(\
    ::the::text::string_converter::to_wstr_ptr(str))
#endif /* !T2W */
#endif /* defined(UNICODE) || defined(_UNICODE) */


namespace the {
namespace text {

    /**
     * Static utility class converting between ANSI, Unicode, and Utf8 strings
     */
    class string_converter : public string_utf8_converter,
            public not_instantiable {

    public:

        /**
         * Utility string class, which is directly castable to the value_type
         *
         * @remarks: Do not use directly!
         *           Used by the W2A ... macros
         *
         * @tparam T The type of the target string, e.g. std::string or
         *           std::wstring.
         */
        template<class T>
        class castable_utility_string : public the::not_copyable {

        public:

            /** The type of character that the class was instantiated with. */
            typedef typename T::value_type char_type;

            /** The type of string that the class was instantiated with. */
            typedef T string_type;

            /**
             * Ctor
             *
             * @param t The instantiation value
             */
            inline castable_utility_string(const string_type& t) : val(t) {
                THE_STACK_TRACE;
                // intentionally empty
            }

            /**
             * move ctor
             *
             * @param s The object to be moved
             */
            inline castable_utility_string(castable_utility_string&& s)
                    : val(s.val) {
                THE_STACK_TRACE;
                // intentionally empty
            }

            /**
             * Cast operator to the value_type
             *
             * @return Pointer to the array of value_type
             */
            inline operator const char_type *(void) const {
                THE_STACK_TRACE;
                return this->val.c_str();
            }

        private:

            /** The stored value */
            const string_type val;
        };

        /**
         * Performs a "non-conversion" of string 'src' to 'dst'.
         *
         * This method performs basically an assignment and is required for the
         * conversion macros.
         *
         * @param dst The string to receive the text from 'src'.
         * @param src The source string.
         * 
         * @return 'dst'.
         */
        template<class T>
        static inline T& convert(T& dst, const T& src) {
            THE_STACK_TRACE;
            dst = src;
            return dst;
        }

        /**
         * Performs a "non-conversion" of string 'src' to 'dst'.
         *
         * This method performs basically an assignment and is required for the
         * conversion macros.
         *
         * @param dst The string to receive the text from 'src'.
         * @param src The source string.
         * 
         * @return 'dst'.
         */
        template<class T>
        static inline T& convert(T& dst,
                const typename T::value_type *src) {
            THE_STACK_TRACE;
            dst = src;
            return dst;
        }

        /**
         * Performs a "non-conversion" of the first 'len' characters of string
         * 'src' to 'dst'.
         *
         * This method performs basically an assignment and is required for the
         * conversion macros.
         *
         * @param dst The string to receive the text from 'src'.
         * @param src The source string.
         * @param len The length of 'src'.
         * 
         * @return 'dst'.
         */
        template<class T>
        static inline T& convert(T& dst, const typename T::value_type *src,
                const size_t len) {
            THE_STACK_TRACE;
            dst.assign(src, len);
            return dst;
        }

        /**
         * Converts 'src' to the ANSI String 'dst'
         *
         * @param dst The ANSI String object to receive the string data
         * @param src The source of the string data
         * @param defChar The default character to be used for characters
         *                which cannot be represented by an ANSI string with
         *                the current system locale
         *
         * @return A reference to 'dst'
         */
        static inline the::astring& convert(the::astring& dst,
                const the::wstring& src, const char defChar = '?') {
            THE_STACK_TRACE;
            return convert(dst, src.c_str(), src.length(), defChar);
        }

        /**
         * Converts 'src' to the ANSI String 'dst'
         *
         * @param dst The ANSI String object to receive the string data
         * @param src The source of the string data
         * @param defChar The default character to be used for characters
         *                which cannot be represented by an ANSI string with
         *                the current system locale
         *
         * @return A reference to 'dst'
         */
        static inline the::astring& convert(the::astring& dst,
                const wchar_t *src, const char defChar = '?') {
            THE_STACK_TRACE;
            return convert(dst, src, ::wcslen(src), defChar);
        }

        /**
         * Converts 'src' to the ANSI String 'dst'
         *
         * @param dst The ANSI String object to receive the string data
         * @param src The source of the string data
         * @param len The length of the source string in character
         * @param defChar The default character to be used for characters
         *                which cannot be represented by an ANSI string with
         *                the current system locale
         *
         * @return A reference to 'dst'
         */
        static the::astring& convert(the::astring& dst, const wchar_t *src,
            size_t len, const char defChar = '?');

        /**
         * Converts 'src' to the unicode string 'dst'
         *
         * @param dst The unicode string object to receive the string data
         * @param src The source of the string data
         *
         * @return A reference to 'dst'
         */
        static inline the::wstring& convert(the::wstring& dst,
                const the::astring& src) {
            THE_STACK_TRACE;
            return convert(dst, src.c_str(), src.length());
        }

        /**
         * Converts 'src' to the unicode string 'dst'
         *
         * @param dst The unicode string object to receive the string data
         * @param src The source of the string data
         *
         * @return A reference to 'dst'
         */
        static inline the::wstring& convert(the::wstring& dst,
                const char *src) {
            THE_STACK_TRACE;
            return convert(dst, src, ::strlen(src));
        }

        /**
         * Converts 'src' to the unicode string 'dst'
         *
         * @param dst The unicode string object to receive the string data.
         * @param src The source of the string data.
         * @param len The length of the source string in character.
         *
         * @return A reference to 'dst'.
         */
        static the::wstring& convert(the::wstring& dst, const char *src,
            const size_t len);

        /**
         * Converts 'src' to a string of the specified type
         *
         * @param src The source of the string data.
         *
         * @return The newly generated string of the specified type
         */
        template<class Tr, class Tp>
        static inline Tr convert(const Tp& src) {
            Tr r;
            convert(r, src);
            return r;
        }

        /**
         * Converts 'src' to a string of the specified type
         *
         * @param src The source of the string data.
         * @param len The length of the source string in character.
         *
         * @return The newly generated string of the specified type
         */
        template<class Tr, class Tp>
        static inline Tr convert(const Tp *src, const size_t len) {
            Tr r;
            convert(r, src, len);
            return r;
        }

        /**
         * Returns a character as 'char'
         *
         * Remarks: Char-wise operations are broken under Linux when a UTF-8
         *          locale is used (which is used by most distributions).
         *
         * @param c a character
         *
         * @return 'c' as 'char' or 0 if there is any conversion error
         */
        static THE_FORCE_INLINE char to_a(char c) {
            THE_STACK_TRACE;
            return c;
        }

        /**
         * Returns a character as 'char'
         *
         * Remarks: Char-wise operations are broken under Linux when a UTF-8
         *          locale is used (which is used by most distributions).
         *
         * @param c a character
         *
         * @return 'c' as 'char' or 0 if there is any conversion error
         */
        static char to_a(wchar_t c);

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static THE_FORCE_INLINE the::astring& to_a(the::astring& str) {
            THE_STACK_TRACE;
            return str;
        }

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static THE_FORCE_INLINE const the::astring& to_a(
                const the::astring& str) {
            THE_STACK_TRACE;
            return str;
        }

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static inline the::astring to_a(const char *str) {
            THE_STACK_TRACE;
            return str;
        }

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         * @param len The length of 'str' in characters
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static inline the::astring to_a(const char *str, size_t len) {
            THE_STACK_TRACE;
            return the::astring(str, len);
        }

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static inline the::astring to_a(const the::wstring& str) {
            THE_STACK_TRACE;
            the::astring retval;
            convert(retval, str);
            return retval;
        }

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static inline the::astring to_a(const wchar_t *str) {
            THE_STACK_TRACE;
            the::astring retval;
            convert(retval, str);
            return retval;
        }

        /**
         * Returns the string of 'str' as 'the::astring' (either as value or
         * as reference)
         *
         * @param str The original string
         * @param len The length of 'str' in characters
         *
         * @return 'str' as 'the::astring' (either as value or as reference)
         */
        static inline the::astring to_a(const wchar_t *str, size_t len) {
            THE_STACK_TRACE;
            the::astring retval;
            convert(retval, str, len);
            return retval;
        }

        /**
         * Converts a string into an ANSI string.
         *
         * @remarks: Do not call this method directly!
         *           Use the W2A ... macros instead.
         *
         * @param s The source string
         *
         * @return The converted string
         */
        static THE_FORCE_INLINE const char *to_astr_ptr(const char *s) {
            THE_STACK_TRACE;
            return s;
        }

        /**
         * Converts a string into an ANSI string.
         *
         * @remarks: Do not call this method directly!
         *           Use the W2A ... macros instead.
         *
         * @param s The source string
         *
         * @return The converted string
         */
        template<class Tp>
        static THE_FORCE_INLINE const castable_utility_string<astring>
        to_astr_ptr(const Tp& s) {
            THE_STACK_TRACE;
            astring d;
            return castable_utility_string<astring>(
                string_converter::convert(d, s));
        }

        /**
         * Returns a character as 'wchar_t'
         *
         * Remarks: Char-wise operations are broken under Linux when a UTF-8
         *          locale is used (which is used by most distributions).
         *
         * @param c a character
         *
         * @return 'c' as 'wchar_t' or 0 if there is any conversion error
         */
        static wchar_t to_w(char c);

        /**
         * Returns a character as 'wchar_t'
         *
         * Remarks: Char-wise operations are broken under Linux when a UTF-8
         *          locale is used (which is used by most distributions).
         *
         * @param c a character
         *
         * @return 'c' as 'wchar_t' or 0 if there is any conversion error
         */
        static THE_FORCE_INLINE wchar_t to_w(wchar_t c) {
            THE_STACK_TRACE;
            return c;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static inline the::wstring to_w(the::astring& str) {
            THE_STACK_TRACE;
            the::wstring retval;
            convert(retval, str);
            return retval;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static inline the::wstring to_w(const char *str) {
            THE_STACK_TRACE;
            the::wstring retval;
            convert(retval, str);
            return retval;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         * @param len The length of 'str' in characters
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static inline the::wstring to_w(const char *str, size_t len) {
            THE_STACK_TRACE;
            the::wstring retval;
            convert(retval, str, len);
            return retval;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static THE_FORCE_INLINE the::wstring& to_w(the::wstring& str) {
            THE_STACK_TRACE;
            return str;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static THE_FORCE_INLINE const the::wstring& to_w(
                const the::wstring& str) {
            THE_STACK_TRACE;
            return str;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static inline the::wstring to_w(const wchar_t *str) {
            THE_STACK_TRACE;
            return str;
        }

        /**
         * Returns the string of 'str' as 'the::wstring' (either as value or
         * as reference)
         *
         * @param str The original string
         * @param len The length of 'str' in characters
         *
         * @return 'str' as 'the::wstring' (either as value or as reference)
         */
        static inline the::wstring to_w(const wchar_t *str, size_t len) {
            THE_STACK_TRACE;
            return the::wstring(str, len);
        }

        /**
         * Converts a string into an wide string.
         *
         * @remarks: Do not call this method directly!
         *           Use the W2A ... macros instead.
         *
         * @param s The source string
         *
         * @return The converted string
         */
        static THE_FORCE_INLINE const wchar_t *to_wstr_ptr(const wchar_t *s) {
            THE_STACK_TRACE;
            return s;
        }

        /**
         * Converts a string into an wide string.
         *
         * @remarks: Do not call this method directly!
         *           Use the W2A ... macros instead.
         *
         * @param s The source string
         *
         * @return The converted string
         */
        template<class Tp>
        static THE_FORCE_INLINE const castable_utility_string<wstring>
        to_wstr_ptr(const Tp& s) {
            THE_STACK_TRACE;
            wstring d;
            return castable_utility_string<wstring>(
                string_converter::convert(d, s));
        }

    };

} /* end namespace Text */
} /* end namespace The */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRINGCONVERTER_H_INCLUDED */
