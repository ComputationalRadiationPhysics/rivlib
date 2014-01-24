/*
 * the/text/string_double_invariant_utility.h
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
#ifndef THE_TEXT_STRING_DOUBLE_INVARIANT_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_DOUBLE_INVARIANT_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <cstdlib>
#include <cmath>
#include "the/text/string_converter.h"
#include "the/argument_null_exception.h"
#include "the/format_exception.h"
#include "the/types.h"
#include "the/math/types.h"


namespace the {
namespace text {

    /**
     * static utility class to convert between 'double' and 'string'
     * invariant from the current locale, emulating results of the 'C' locale.
     *
     * @remark Do not use this class directly. Use string_utility instead.
     */
    class string_double_invariant_utility {
    public:

        /** Possible format styles */
        enum class format_style {
            /** Automatically select 'STYLE_NORMAL' or 'format_style::scientific' */
            automatic,
            /** Uses floating point representation */
            normal,
            /** Uses scientific representation with exponent */
            scientific
        };

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The string
         *
         * @return The parsed double
         *
         * @throws format_exception If 'str' could not be successfully parsed
         */
        template<class T>
        static inline double parse_double_invariant(const T& str) {
            return parse(str, static_cast<int>(str.length()));
        }

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The zero-terminated string
         *
         * @return The parsed double
         *
         * @throws argument_null_exception if 'str' is NULL
         * @throws format_exception If 'str' could not be successfully parsed
         */
        template<class T>
        static inline double parse_double_invariant(
                const T *str) {
            return parse(str, -1);
        }

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The string
         * @param defVal The default value to be returned if 'str' could not
         *               be parsed
         *
         * @return The parsed double
         */
        template<class T>
        static inline double parse_double_invariant(const T& str,
                double defVal) {
            try {
                return parse(str, static_cast<int>(str.length()));
            } catch(...) {
            }
            return defVal;
        }

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The zero-terminated string
         * @param defVal The default value to be returned if 'str' could not
         *               be parsed
         *
         * @return The parsed double
         */
        template<class T>
        static inline double parse_double_invariant(
                const T *str, double defVal) {
            try {
                return parse(str, -1);
            } catch(...) {
            }
            return defVal;
        }

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The string
         * @param defVal The default value to be returned if 'str' could not
         *               be parsed
         * @param error Will be set to false if 'str' was parsed successfully
         *              or will be set to true if the parsing failed.
         *
         * @return The parsed double
         */
        template<class T>
        static inline double parse_double_invariant(const T& str,
                double defVal, bool& error) {
            try {
                error = false;
                return parse(str, static_cast<int>(str.length()));
            } catch(...) {
                error = true;
            }
            return defVal;
        }

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The zero-terminated string
         * @param defVal The default value to be returned if 'str' could not
         *               be parsed
         * @param error Will be set to false if 'str' was parsed successfully
         *              or will be set to true if the parsing failed.
         *
         * @return The parsed double
         */
        template<class T>
        static inline double parse_double_invariant(
                const T *str, double defVal,
                bool& error) {
            try {
                error = false;
                return parse(str, -1);
            } catch(...) {
                error = true;
            }
            return defVal;
        }

        /**
         * Converts a double value into a string representation (base 10 with
         * the specified number of digits).
         *
         * @param str The string receiving the result
         * @param value The input double value
         * @param style The representation style to be used
         * @param minLeadingDigits The minimum number of leading digits. The
         *                         resulting string will be padded with
         *                         leading zeros. A negative value results in
         *                         automatic adjustment.
         * @param maxTrailingDigits The maximum number of trailing digits. A
         *                          negative value results in automatic
         *                          adjustment.
         * @param significantDigits The overall number of significant digits.
         *                          A negative value results in automatic
         *                          adjustment.
         *
         * @remark If the scientific exponential representation style is used,
         *         the values 'minLeadingDigits' and 'maxTrailingDigits' are
         *         ignored.
         *
         * @remark The values 'minLeadingDigits', 'significantDigits', and
         *         'maxTrailingDigits' are priorized in this order
         *
         * @return 'str'
         */
        template<class T>
        static T& double_to_string_invariant(T& str, double value,
            format_style style = format_style::automatic,
            int minLeadingDigits = 1, int significantDigits = -1,
            int maxTrailingDigits = -1);

    protected:

        /** forbidden Ctor */
        string_double_invariant_utility(void);

        /** forbidden Copy Ctor */
        string_double_invariant_utility(
            const string_double_invariant_utility& src);

        /** forbidden Dtor */
        ~string_double_invariant_utility(void);

    private:

        /** Test constant for NaN */
        const static double testNaN;

        /** Test constant for positive Inf */
        const static double testInf;

        /** Test constant for negative Inf */
        const static double testNInf;

        /**
         * Parses a string as double value emulating the 'C' locale
         *
         * @param str The string
         * @param len The length of 'str' in characters excluding a terminating
         *            zero, or negative if 'str' is zero-terminated
         *
         * @return The parsed double
         *
         * @throws argument_null_exception if 'str' is NULL
         * @throws format_exception If 'str' could not be successfully parsed
         */
        template<class T>
        static double parse(const T *str, int len);

    };


    /*
     * string_double_invariant_utility::double_to_string_invariant
     */
    template<class T>
    T& string_double_invariant_utility::double_to_string_invariant(T& str,
            double value, format_style style, int minLeadingDigits,
            int significantDigits, int maxTrailingDigits) {
        const static double epsilon = static_cast<double>(the::math::double_epsilon
            * 1000.0);

        // special values
        if (::fabs(value) < epsilon) {
            str = static_cast<typename T::value_type>('0');
            return str;
        }

        if (::memcmp(&value, &testNaN, sizeof(double)) == 0) {
            string_converter::convert(str, "#NAN");
            return str;
        }
        if (::memcmp(&value, &testInf, sizeof(double)) == 0) {
            string_converter::convert(str, "#INF");
            return str;
        }
        if (::memcmp(&value, &testNInf, sizeof(double)) == 0) {
            string_converter::convert(str, "#NINF");
            return str;
        }

        if (value < 0.0) {
            str = static_cast<typename T::value_type>('-');
            value *= -1.0;
        } else {
            str.clear();
        }
        double mask;
        int val;
        double iVal = 0.0;
        double e = ::log10(value);
        int ei = static_cast<int>(::ceil(e));

        if ((ei < -10) && (ei > 10) && (style == format_style::automatic)) {
                    style = format_style::scientific;
        }

        if (style == format_style::scientific) {
            ei = static_cast<int>(::floor(e));
            int iei = ei;
            mask = ::pow(10.0, ei);
            val = static_cast<int>(value / mask) % 10;
            if ((significantDigits == 0) || (significantDigits == 1)) {
                mask /= 10;
                int vv = static_cast<int>(value / mask) % 10;
                if (vv >= 5) val++;
            }
            str += static_cast<typename T::value_type>('0' + val);
            iVal = static_cast<double>(val) * mask;
            if (significantDigits > 0) significantDigits--;

            if ((significantDigits != 0) && (::fabs(value - iVal) > epsilon)) {
                str += static_cast<typename T::value_type>('.');
                while ((significantDigits != 0)
                        && (::fabs(value - iVal) > epsilon)) {
                    --ei;
                    mask = ::pow(10.0, ei);
                    val = static_cast<int>(value / mask) % 10;
                    if (significantDigits == 0) {
                        mask /= 10;
                        int vv = static_cast<int>(value / mask) % 10;
                        if (vv >= 5) val++;
                    }
                    str += static_cast<typename T::value_type>('0' + val);
                    iVal += static_cast<double>(val) * mask;
                }
            }

            if (iei != 0) {
                // because large E is ugly
                str += static_cast<typename T::value_type>('e');
                if (iei < 0) {
                    str += static_cast<typename T::value_type>('-');
                    iei *= -1;
                }
                T tmpStr;
                while (iei != 0) {
                    tmpStr = static_cast<typename T::value_type>('0' + (iei % 10))
                        + tmpStr;
                    iei /= 10;
                }
                str += tmpStr;

            }

        } else {
            // leading digits
            if ((minLeadingDigits > 0) && (minLeadingDigits > ei)) {
                if (significantDigits >= 0) {
                    significantDigits += minLeadingDigits - ei;
                }
                ei = minLeadingDigits; // number of digits before the dot
            }
            if ((minLeadingDigits < 0) && (ei < 0)) {
                // on autoadjust show at least one digit
                ei = 1;
            }
            if (ei < 0) {
                ei = 0;
                if (::fabs(value) < epsilon) ei = 1; // special for zero
            }

            while (ei > 0) {
                if (significantDigits == 0) {
                    str += static_cast<typename T::value_type>('0');
                    iVal *= 10.0;
                    --ei;
                    continue;
                }
                if (significantDigits > 0) --significantDigits;
                mask = ::pow(10.0, --ei);
                val = static_cast<int>(value / mask) % 10;
                if (significantDigits == 0) {
                    mask /= 10.0;
                    int vv = static_cast<int>(value / mask) % 10;
                    if (vv >= 5) ++val;
                }
                iVal = iVal * 10.0 + static_cast<double>(val);
                str += static_cast<typename T::value_type>('0' + val);
            }

            if (((value - iVal) > epsilon) && (significantDigits != 0)
                    && (maxTrailingDigits != 0)) {
                str += static_cast<typename T::value_type>('.');
                while (((value - iVal) > epsilon) && (significantDigits != 0)
                        && (maxTrailingDigits != 0)) {
                    //double dbgDiff = value - iVal;
                    if (significantDigits > 0) significantDigits--;
                    if (maxTrailingDigits > 0) maxTrailingDigits--;
                    mask = ::pow(10.0, --ei);
                    if (mask == 0.0) break; // sanity check
                    val = static_cast<int>(value / mask) % 10;
                    
                    iVal += static_cast<double>(val) * mask;
                    if ((significantDigits == 0) || (maxTrailingDigits == 0)) {
                        mask /= 10.0;
                        int vv = static_cast<int>(value / mask) % 10;
                        if (vv >= 5) ++val;
                    }
                    str += static_cast<typename T::value_type>('0' + val);

                }
            }

        }

        return str;
    }


    /*
     * string_double_invariant_utility::parse
     */
    template<class T>
    double string_double_invariant_utility::parse(
            const T *str, int len) {
        if (str == NULL) {
            throw argument_null_exception("str", __FILE__, __LINE__);
        }
        bool neg = false, eNeg = false;
        long val = 0, fracV = 0, fracS = 1, expV = 1;
        int state = 0;
        bool complete = false;
        // 1. skip trailing spaces
        // 2. sign
        // 3. digits (zero or more)
        // 4. dot (optional)
        // 5. digits (optional)
        // 6. 'e' or 'E' (optional)
        // 7. digits (exponent; optional)

        // 1.
        while ((*str != static_cast<T>(0)) && (len != 0)) {
            if (!the::text::char_utility::is_space(*str)) break;
            str++;
            if (len > 0) len--;
        }
        if ((*str == static_cast<T>(0)) || (len == 0)) {
            throw format_exception("Empty String", __FILE__, __LINE__);
        }

        if (*str == static_cast<T>('#')) {
            // special symbol!
            if (((len < 0) || (len > 4))
                    && (str[1] == static_cast<T>('N')
                        || str[1] == static_cast<T>('n'))
                    && (str[2] == static_cast<T>('A')
                        || str[2] == static_cast<T>('a'))
                    && (str[3] == static_cast<T>('N')
                        || str[3] == static_cast<T>('n'))) {
                return testNaN;
            }
            if (((len < 0) || (len > 4))
                    && (str[1] == static_cast<T>('I')
                        || str[1] == static_cast<T>('i'))
                    && (str[2] == static_cast<T>('N')
                        || str[2] == static_cast<T>('n'))
                    && (str[3] == static_cast<T>('F')
                        || str[3] == static_cast<T>('f'))) {
                return testInf;
            }
            if (((len < 0) || (len > 5))
                    && (str[1] == static_cast<T>('N')
                        || str[1] == static_cast<T>('n'))
                    && (str[2] == static_cast<T>('I')
                        || str[2] == static_cast<T>('i'))
                    && (str[3] == static_cast<T>('N')
                        || str[3] == static_cast<T>('n'))
                    && (str[4] == static_cast<T>('F')
                        || str[4] == static_cast<T>('f'))) {
                return testNInf;
            }
            throw format_exception("Invalid special symbol",
                __FILE__, __LINE__);
        }

        while (!complete && (*str != static_cast<T>(0)) && (len != 0)) {
            switch (state) {
            case 0: // first char
                if (*str == static_cast<T>('-')) {
                    neg = true;
                    state = 1;
                } else if (*str == static_cast<T>('+')) {
                    state = 1;
                } else if (*str == static_cast<T>('.')) {
                    state = 3;
                } else if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    val = (*str - static_cast<T>('0'));
                    state = 2;
                } else {
                    // first char was illegal
                    throw format_exception("Illegal first char",
                        __FILE__, __LINE__);
                }
                break;
            case 1: // sign was found, now we need a digit or the point
                if (*str == static_cast<T>('.')) {
                    state = 3;
                } else if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    val = (*str - static_cast<T>('0'));
                    state = 2;
                } else {
                    // no number!
                    throw format_exception("Not starting with digit",
                        __FILE__, __LINE__);
                }
                break;
            case 2: // at least one digit was found
                if (*str == static_cast<T>('.')) {
                    state = 3;
                } else if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    val = val * 10 + (*str - static_cast<T>('0'));
                } else if ((*str == static_cast<T>('e'))
                        || (*str == static_cast<T>('E'))) {
                    state = 5;
                } else {
                    // string depleted!
                    complete = true;
                }
                break;
            case 3: // decimal dot parsed, now read frac and search for E
                if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    fracV = (*str - static_cast<T>('0'));
                    fracS *= 10;
                    state = 4;
                } else if ((*str == static_cast<T>('e'))
                        || (*str == static_cast<T>('E'))) {
                    state = 5;
                } else {
                    // string depleted!
                    complete = true;
                }
                break;
            case 4: // continue parsing frac and searching for E
                if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    fracV = fracV * 10 + (*str - static_cast<T>('0'));
                    fracS *= 10;
                } else if ((*str == static_cast<T>('e'))
                        || (*str == static_cast<T>('E'))) {
                    state = 5;
                } else {
                    // string depleted!
                    complete = true;
                }
                break;
            case 5: // E found, need an optional sign and at least one digit 
                if (*str == static_cast<T>('-')) {
                    eNeg = true;
                    state = 6;
                } else if (*str == static_cast<T>('+')) {
                    state = 6;
                } else if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    expV = (*str - static_cast<T>('0'));
                    state = 7;
                } else {
                    // first exp char was illegal
                    throw format_exception("Illegal first exponent char",
                        __FILE__, __LINE__);
                }
                break;
            case 6: // E found, sign found, now need at least one digit 
                if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    expV = (*str - static_cast<T>('0'));
                    state = 7;
                } else {
                    // first exp char was illegal
                    throw format_exception("Illegal first exponent char",
                        __FILE__, __LINE__);
                }
                break;
            case 7: 
                if ((*str >= static_cast<T>('0'))
                        && (*str <= static_cast<T>('9'))) {
                    expV = expV * 10 + (*str - static_cast<T>('0'));
                } else {
                    // string depleted!
                    complete = true;
                }
                break;
            }

            str++;
            if (len > 0) len--;
        }

        if (neg) {
            val = -val;
            fracV = -fracV;
        }
        if (eNeg) expV = -expV;

        double d = static_cast<double>(val)
            + static_cast<double>(fracV) / static_cast<double>(fracS);

        if (expV != 1) d *= ::pow(10.0, expV);

        return d;
    }

} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_DOUBLE_INVARIANT_UTILITY_H_INCLUDED */
