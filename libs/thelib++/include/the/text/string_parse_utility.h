/*
 * the/text/string_parse_utility.h
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
/*
 * CharTraits.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 - 2006 by Christoph Mueller. All rights reserved.
 */
#ifndef THE_TEXT_STRING_PARSE_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_PARSE_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#include "the/argument_exception.h"
#include "the/format_exception.h"
#include "the/types.h"


namespace the {
namespace text {

    /**
     * Utility class to parse variable values from strings
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_parse_utility {
    public:

        /**
         * Converts the string str to the returned boolean value.
         *
         * @param str The input string.
         *
         * @return The parsed boolean value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        boolean value.
         */
        template <class T>
        static inline bool parse_bool(const T& str) {
            return parse_bool(str.c_str());
        }

        /**
         * Converts the string str to the returned boolean value.
         *
         * @param str The input string.
         *
         * @return The parsed boolean value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        boolean value.
         */
        static bool parse_bool(const char *str);

        /**
         * Converts the string str to the returned boolean value.
         *
         * @param str The input string.
         *
         * @return The parsed boolean value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        boolean value.
         */
        static bool parse_bool(const wchar_t *str);

        /**
         * Converts the string str to the returned floating point value.
         *
         * @param str The input string.
         * @param localeInvariant Uses the invariant 'C' locale for parsing
         *
         * @return The parsed floating point value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        floating point value.
         */
        template <class T>
        static inline double parse_double(const T& str) {
            return parse_double(str.c_str());
        }

        /**
         * Converts the string str to the returned floating point value.
         *
         * @param str The input string.
         *
         * @return The parsed floating point value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        floating point value.
         */
        static double parse_double(const char *str);

        /**
         * Converts the string str to the returned floating point value.
         *
         * @param str The input string.
         * @param localeInvariant Uses the invariant 'C' locale for parsing
         *
         * @return The parsed floating point value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        floating point value.
         */
        static double parse_double(const wchar_t *str);

        /**
         * Converts the string str to the returned integer value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        template <class T>
        static inline int parse_int(const T& str) {
            return parse_int(str.c_str());
        }

        /**
         * Converts the string str to the returned integer value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        static int parse_int(const char *str);

        /**
         * Converts the string str to the returned integer value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        static int parse_int(const wchar_t *str);

        /**
         * Converts the string str to the returned 64 bit integer value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        template <class T>
        static inline int64_t parse_int64(const T& str) {
            return parse_int64(str.c_str());
        }

        /**
         * Converts the string str to the returned 64 bit integer value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        template <class T>
        static inline int64_t parse_int64(const T *str) {
            int64_t sig = 1;
            if (str == NULL) {
                throw argument_exception("str", __FILE__, __LINE__);
            }
            if (*str == static_cast<T>('-')) {
                sig = -1;
                str++;
            }
            int64_t value = static_cast<int64_t>(parse_uint64(str));
            if (value < 0) {
                throw format_exception("Overflow parsing Int64", 
                    __FILE__, __LINE__);
            }
            return sig;
        }

        /**
         * Converts the string str to the returned 64 bit unsigned integer 
         * value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        template <class T>
        static uint64_t inline parse_uint64(const T& str) {
            return parse_uint64(str.c_str());
        }

        /**
         * Converts the string str to the returned 64 bit unsigned integer 
         * value.
         *
         * @param str The input string.
         *
         * @return The parsed integer value.
         *
         * @throw IllegalParamException if str is NULL.
         * @throw format_exception if the string could not be parsed to an
         *        integer value.
         */
        template <class T>
        static uint64_t inline parse_uint64(const T *str) {
            uint64_t value = 0;
            uint64_t v;
            if (str == NULL) {
                throw argument_exception("str", __FILE__, __LINE__);
            }
            while ((*str != 0) && (*str >= static_cast<T>('0')) 
                    && (*str <= static_cast<T>('9'))) {
                v = value * 10 + (*str - static_cast<T>('0'));
                str++;
                if (v < value) {
                    throw format_exception("Overflow parsing Int64", 
                        __FILE__, __LINE__);
                }
                value = v;
            }
            return value;
        }

    protected:

        /** forbidden Ctor */
        string_parse_utility(void);

        /** forbidden Copy Ctor */
        string_parse_utility(const string_parse_utility& src);

        /** forbidden Dtor */
        ~string_parse_utility(void);

    private:

    };


} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRINGPARSEUTILITY_H_INCLUDED */
