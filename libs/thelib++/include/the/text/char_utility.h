/*
 * the/text/char_utility.h
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

#ifndef THE_TEXT_CHAR_UTILITY_H_INCLUDED
#define THE_TEXT_CHAR_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/text/locale_utility.h"
#include "the/assert.h"
#include "the/memory.h"
#include "the/types.h"


namespace the {
namespace text {

    /**
     * Static class providing utility functions for operations on single
     * characters.
     *
     * Depends on the LocaleUtility class.
     *
     * @remarks On most Linux systems the defaulte Locale is nowadays *.UTF8
     *  This means that a single 'char' can only represent 7-bit ANSI by
     *  itself and that all other characters in a 'char*' string will be
     *  stored by more than one 'char'. This BREAKS the possibility of doing
     *  conversion per-character. If operations on a whole string are to be
     *  performed DO NOT USE methods from CharUtility. USE methods from
     *  StringUtility instead.
     */
    class char_utility {
    public:

        /**
         * Answer if c is an alpha character
         *
         * @param c The character to test
         *
         * @return True if c is an alpha character
         */
        template<class T>
        static inline bool is_alpha(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).is(std::ctype_base::alpha, c);
        }

        /**
         * Answer if c is a digit character
         *
         * @param c The character to test
         *
         * @return True if c is a digit character
         */
        template<class T>
        static inline bool is_digit(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).is(std::ctype_base::digit, c);
        }

        /**
         * Answer if c is a lower case character
         *
         * @param c The character to test
         *
         * @return True if c is a lower case character
         */
        template<class T>
        static inline bool is_lower_case(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).is(std::ctype_base::lower, c);
        }

        /**
         * Answer if c is a white space character
         *
         * @param c The character to test
         *
         * @return True if c is a white space character
         */
        template<class T>
        static inline bool is_space(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).is(std::ctype_base::space, c);
        }

        /**
         * Answer if c is a upper case character
         *
         * @param c The character to test
         *
         * @return True if c is a upper case character
         */
        template<class T>
        static inline bool is_upper_case(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).is(std::ctype_base::upper, c);
        }

        /**
         * Converts a character to ANSI
         *
         * @param c The character to be converted
         * @param defChar The default character to be used if 'c' could not
         *                be converted
         *
         * @return The converted character
         */
        static inline char to_ansi(char c, char defChar = '?') {
            return c;
        }

        /**
         * Converts a character to ANSI
         *
         * @param c The character to be converted
         * @param defChar The default character to be used if 'c' could not
         *                be converted
         *
         * @return The converted character
         */
        static inline char to_ansi(wchar_t c, char defChar = '?') {
            return std::use_facet<std::ctype<wchar_t> >
                (locale_utility::locale()).narrow(c, defChar);
        }

        /**
         * Converts a character to lower case 
         *
         * @param c The character to be converted
         *
         * @return The converted character
         */
        template<class T>
        static inline T to_lower_case(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).tolower(c);
        }

        /**
         * Converts a character to Unicode
         *
         * @param c The character to be converted
         *
         * @return The converted character
         */
        static inline wchar_t to_unicode(char c) {
            return std::use_facet<std::ctype<wchar_t> >
                (locale_utility::locale()).widen(c);
        }

        /**
         * Converts a character to Unicode
         *
         * @param c The character to be converted
         *
         * @return The converted character
         */
        static inline wchar_t to_unicode(wchar_t c) {
            return c;
        }

        /**
         * Converts a character to upper case 
         *
         * @param c The character to be converted
         *
         * @return The converted character
         */
        template<class T>
        static inline T to_upper_case(T c) {
            return std::use_facet<std::ctype<T> >(
                locale_utility::locale()).toupper(c);
        }

    private:

        /** forbidden ctor */
        char_utility(void);

        /** forbidden copy ctor */
        char_utility(const char_utility& src);

        /** forbidden dtor */
        ~char_utility(void);

    };


} /* end namesapce text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_CHAR_UTILITY_H_INCLUDED */
