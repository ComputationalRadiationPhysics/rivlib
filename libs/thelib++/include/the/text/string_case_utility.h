/*
 * the/text/string_case_utility.h
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
#ifndef THE_TEXT_STRING_CASE_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_CASE_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_utility"
#endif /* THE_STRING_H_INCLUDED */
#include "the/config.h"
#include "the/force_inline.h"
#include "the/types.h"
#include "the/text/char_utility.h"


namespace the {
namespace text {

    /**
     * Utility class to convert character casing of strings
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_case_utility {
    public:

#ifndef THE_WINDOWS

        /**
         * Converts all characters of a string to lower case 
         *
         * @param str The string to be converted
         *
         * @return A reference to 'str'
         */
        static the::astring& to_lower_case(the::astring& str);

        /**
         * Converts all characters of a string to upper case 
         *
         * @param str The string to be converted
         *
         * @return A reference to 'str'
         */
        static the::astring& to_upper_case(the::astring& str);

        /**
         * Converts 'cnt' character in 'srt' to lower case and copies them to
         * 'dst'
         *
         * @param dst The memory to receive the converted characters
         * @param cnt The number of characters to be converted and copied from
         *            'src' to 'dst'
         * @param src The source of the original characters
         */
        static void to_lower_case(char *dst, size_t cnt, const char *src);

        /**
         * Converts 'cnt' character in 'srt' to upper case and copies them to
         * 'dst'
         *
         * @param dst The memory to receive the converted characters
         * @param cnt The number of characters to be converted and copied from
         *            'src' to 'dst'
         * @param src The source of the original characters
         */
        static void to_upper_case(char *dst, size_t cnt, const char *src);

#endif

        /**
         * Converts all characters of a string to lower case 
         *
         * @param str The string to be converted
         *
         * @return A reference to 'str'
         */
        template<class T>
        static THE_FORCE_INLINE T& to_lower_case(T& str) {
            to_lower_case(const_cast<typename T::value_type*>(str.c_str()),
                str.length(), str.c_str());
            return str;
        }

        /**
         * Converts all characters of a string to upper case 
         *
         * @param str The string to be converted
         *
         * @return A reference to 'str'
         */
        template<class T>
        static THE_FORCE_INLINE T& to_upper_case(T& str) {
            to_upper_case(const_cast<typename T::value_type*>(str.c_str()),
                str.length(), str.c_str());
            return str;
        }

        /**
         * Converts 'cnt' character in 'srt' to lower case and copies them to
         * 'dst'
         *
         * @param dst The memory to receive the converted characters
         * @param cnt The number of characters to be converted and copied from
         *            'src' to 'dst'
         * @param src The source of the original characters
         */
        template<class T>
        static void to_lower_case(T *dst, size_t cnt, const T *src);

        /**
         * Converts 'cnt' character in 'srt' to upper case and copies them to
         * 'dst'
         *
         * @param dst The memory to receive the converted characters
         * @param cnt The number of characters to be converted and copied from
         *            'src' to 'dst'
         * @param src The source of the original characters
         */
        template<class T>
        static void to_upper_case(T *dst, size_t cnt, const T *src);

    protected:

        /** forbidden Ctor */
        string_case_utility(void);

        /** forbidden Copy Ctor */
        string_case_utility(const string_case_utility& src);

        /** forbidden Dtor */
        ~string_case_utility(void);

    private:

    };


    /*
     * string_case_utility::to_lower_case
     */
    template<class T>
    void string_case_utility::to_lower_case(T *dst, size_t cnt,
            const T *src) {
        for (size_t i = 0U; i < cnt; i++) {
            if (src[i] == 0) {
                dst[i] = 0;
                break;
            }
            dst[i] = char_utility::to_lower_case(src[i]);
        }
    }


    /*
     * string_case_utility::to_upper_case
     */
    template<class T>
    void string_case_utility::to_upper_case(T *dst, size_t cnt,
            const T *src) {
        for (size_t i = 0U; i < cnt; i++) {
            if (src[i] == 0) {
                dst[i] = 0;
                break;
            }
            dst[i] = char_utility::to_upper_case(src[i]);
        }
    }



} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_CASE_UTILITY_H_INCLUDED */
