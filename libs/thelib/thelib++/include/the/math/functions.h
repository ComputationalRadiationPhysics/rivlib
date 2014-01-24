/*
 * the/math/functions.h
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
 * mathfunctions.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_MATH_FUNCTIONS_H_INCLUDED
#define THE_MATH_FUNCTIONS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>
#include <utility>

#include "the/assert.h"
#include "the/utils.h"
#include "the/stack_trace.h"

#include "the/math/types.h"


namespace the {
namespace math {

    /**
     * Answer the absolute of 'n'.
     *
     * @return The absolute of 'n'.
     */
    template<class T> inline T abs(const T n) {
        THE_STACK_TRACE;
        return ::abs(n);
    }


    /**
     * Answer the absolute of 'n'.
     *
     * @return The absolute of 'n'.
     */
    template<> inline double abs(const double n) {
        THE_STACK_TRACE;
        return ::fabs(n);
    }


    /**
     * Answer the absolute of 'n'.
     *
     * @return The absolute of 'n'.
     */
    template<> inline float abs(const float n) {
        THE_STACK_TRACE;
        return ::fabsf(n);
    }


    /**
     * Answer the next power of two which is greater or equal to 'n'.
     *
     * This function has logarithmic runtime complexity.
     *
     * Notes: std::is_integral<T>::value must be true for the template parameter
     * T, i.e. The template must be instantiated with integral numbers.
     *
     * @param n A number.
     *
     * @return The smallest power of two with ('n' <= result).
     */
    template<class T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    next_power_of_two(const T n) {
        THE_STACK_TRACE;
        T retval = static_cast<T>(1);

        while (retval < n) {
            retval <<= 1;
        }

        return retval;
    }


    /**
     * Clamp 'n' to ['minVal', 'maxVal'].
     *
     * @param n      A number.
     * @param minVal The minimum valid number.
     * @param maxVal The maximum valid number, must be greater than 'minVal'.
     *
     * @return The clamped value of 'n'.
     */
    template<class T> inline T clamp(const T n, const T min_val, 
            const T max_val) {
        THE_STACK_TRACE;
        return ((n >= min_val) ? ((n <= max_val) ? n : max_val) : min_val);
    }


    /**
     * Answer whether 'm' and 'n' are equal.
     *
     * @param m        A number.
     * @param n        A number.
     *
     * @return true, if 'm' and 'n' are equal, false otherwise.
     */
    template<class T> inline bool is_equal(const T m, const T n) {
        THE_STACK_TRACE;
        return (m == n);
    }


    /**
     * Answer whether 'm' and 'n' are nearly equal.
     *
     * @param m       A floating point number.
     * @param n       A floating point number.
     * @param epsilon The epsilon value used for comparsion.
     *
     * @return true, if 'm' and 'n' are nearly equal, false otherwise.
     */
    inline bool is_equal(const float m, const float n, const float epsilon) {
        THE_STACK_TRACE;
        return (::fabsf(m - n) < epsilon);
    }


    /**
     * Answer whether 'm' and 'n' are nearly equal. This function uses the
     * the::math::float_epsilon constant.
     *
     * @param m       A floating point number.
     * @param n       A floating point number.
     *
     * @return true, if 'm' and 'n' are nearly equal, false otherwise.
     */
    template<> inline bool is_equal(const float m, const float n) {
        THE_STACK_TRACE;
        return is_equal(m, n, float_epsilon);
    }


    /**
     * Answer whether 'm' and 'n' are nearly equal.
     *
     * @param m       A floating point number.
     * @param n       A floating point number.
     * @param epsilon The epsilon value used for comparsion.
     *
     * @return true, if 'm' and 'n' are nearly equal, false otherwise.
     */
    inline bool is_equal(const double m, const double n, const double epsilon) {
        THE_STACK_TRACE;
        return (::fabs(m - n) < epsilon);
    }


    /**
     * Answer whether 'm' and 'n' are nearly equal. This function uses
     * the the::math::double_epsilon constant.
     *
     * @param m       A floating point number.
     * @param n       A floating point number.
     *
     * @return true, if 'm' and 'n' are nearly equal, false otherwise.
     */
    template<> inline bool is_equal(const double m, const double n) {
        THE_STACK_TRACE;
        return is_equal(m, n, double_epsilon);
    }


    /**
     * Answer whether 'n' is in the interval of 'minVal' to 'maxVal'.
     *
     * @param n          A number.
     * @param minVal     The lower border of the interval. The caller must 
     *                   ensure that 'minVal' <= 'maxVal'.
     * @param maxVal     The upper border of the interval. The caller must 
     *                   ensure that 'minVal' <= 'maxVal'.
     * @param includeMin If true, the lower border is considered part of 
     *                   the interval.
     * @param includeMax If true, the upper border is considered part of 
     *                   the interval.
     *
     * @return true if 'n' is within the interval, false otherwise.
     */
    template<class T> inline bool is_within(const T n, const T min_val,
            const T max_val, const bool include_min = false, 
            const bool include_max = false) {
        THE_STACK_TRACE;
        THE_ASSERT(min_val <= max_val);
        return (((min_val < n) || (include_min && (min_val == n)))
            && ((n < max_val) || (include_max && (max_val == n))));
    }


    /**
     * Answer the maximum of 'n' and 'm'.
     *
     * @param n An number.
     * @param m Another number.
     *
     * @return The maximum of 'n' and 'm'.
     */
    template<class T> inline T maximum(const T n, const T m) {
        THE_STACK_TRACE;
        return (n > m) ? n : m;
    }


    /**
     * Answer the minimum of 'n' and 'm'.
     *
     * @param n An number.
     * @param m Another number.
     *
     * @return The minimum of 'n' and 'm'.
     */
    template<class T> inline T minimum(const T n, const T m) {
        THE_STACK_TRACE;
        return (n < m) ? n : m;
    }


    /**
     * Compute the signum of 'n'.
     *
     * @param n A number.
     *
     * @return The signum of 'n'.
     */
    template<class T> inline T signum(const T n) {
        THE_STACK_TRACE;
        return static_cast<T>((n > static_cast<T>(0)) ? 1 
            : ((n < static_cast<T>(0)) ? -1 : 0));
    }


    /**
     * Compute the square of 'n'.
     * 
     * @param n A number.
     *
     * @return The square of 'n'.
     */
    template<class T> inline T sqr(const T n) {
        THE_STACK_TRACE;
        return (n * n);
    }


    /**
     * Answer the square root of 'n'.
     *
     * @param n A number.
     *
     * @return The square root of 'n'.
     */
    inline float sqrt(const float n) {
        THE_STACK_TRACE;
        return ::sqrtf(n);
    }


    /**
     * Answer the square root of 'n'.
     *
     * @param n A number.
     *
     * @return The square root of 'n'.
     */
    inline double sqrt(const double n) {
        THE_STACK_TRACE;
        return ::sqrt(n);
    }


    /**
     * Answer the square root of 'n'.
     *
     * @param n A number.
     *
     * @return The square root of 'n'.
     */
    inline int sqrt(const int n) {
        THE_STACK_TRACE;
        return static_cast<int>(::sqrt(static_cast<double>(n)));
    }


    /**
     * Calculates the unsigned modulo value.
     *
     * Only signed interger types can be used to instantiate this function.
     * Example:
     *   -3 % 5 = -3
     *   the::math::umod(-3, 5) = 2
     *
     * @param left  The signed dividend.
     * @param right The unsigned divisor. must not be negative.
     *
     * @return The unsigned modulo value.
     */
    template<class T>
    inline typename std::enable_if<
            std::is_integral<T>::value && std::is_signed<T>::value, T>::type
        umod(const T left, const T right) {
        THE_ASSERT(right >= 0);
        // return (left >= 0) ? (left % right) : ((left % right) + right);
        return (left >= 0) ? (left % right) : ((1 - ((left + 1) / right)) * right + left);
    }


    /**
     * Converts an angle from degrees to radians.
     *
     * @param angle Angle in degrees to be converted
     *
     * @return Converted angle in radians
     */
    inline angle_rad_t angle_deg2rad(const angle_deg_t &angle) {
        THE_STACK_TRACE;
        return static_cast<angle_rad_t>(angle
            * static_cast<angle_deg_t>(pi_double) 
            / static_cast<angle_deg_t>(180.0));
    }


    /**
     * Converts an angle from radians to degrees.
     *
     * @param angle Angle in radians to be converted
     *
     * @return Converted angle in degrees
     */
    inline angle_deg_t angle_rad2deg(const angle_rad_t &angle) {
        THE_STACK_TRACE;
        return static_cast<angle_deg_t>(angle
            * static_cast<angle_rad_t>(180.0)
            / static_cast<angle_rad_t>(pi_double));
    }


    /**
     * Compares two objects using the 'IsEqual' funtion and the '<' operator
     *
     * @param lhs The left hand side operand
     * @param rhs The right hand side operand
     *
     * @return  0 if lhs and rhs are equal,
     *         -1 if lhs < rhs,
     *          1 else.
     */
    template<class T> inline int compare(const T& lhs, const T& rhs) {
        THE_STACK_TRACE;
        if (is_equal(lhs, rhs)) return 0;
        if (lhs < rhs) return -1;
        return 1;
    }


    /**
     * Compares two pair objects based on their 'first' members
     *
     * @param lhs The left hand side operand
     * @param rhs The right hand side operand
     *
     * @return  0 if lhs and rhs are equal,
     *         -1 if lhs < rhs,
     *          1 else.
     */
    template<class T1, class T2> int compare_pairs_first(
            const std::pair<T1, T2>& lhs,
            const std::pair<T1, T2>& rhs) {
        THE_STACK_TRACE;
        return compare(lhs.first, rhs.first);
    }


    /**
     * Compares two pair objects based on their 'second' members
     *
     * @param lhs The left hand side operand
     * @param rhs The right hand side operand
     *
     * @return  0 if lhs and rhs are equal,
     *         -1 if lhs < rhs,
     *          1 else.
     */
    template<class T1, class T2> int compare_pairs_second(
            const std::pair<T1, T2>& lhs,
            const std::pair<T1, T2>& rhs) {
        THE_STACK_TRACE;
        return compare(lhs.second, rhs.second);
    }


} /* end namespace math */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_MATH_FUNCTIONS_H_INCLUDED */
