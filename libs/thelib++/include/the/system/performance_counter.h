/*
 * include\the\system\performance_counter.h
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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
 * PerformanceCounter.h  10.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_PERFORMANCE_COUNTER_H_INCLUDED
#define THE_SYSTEM_PERFORMANCE_COUNTER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/comparable.h"
#include "the/stack_trace.h"
#include "the/types.h"


namespace the {
namespace system {

    /**
     * This class provides a system-independent performance counter. The 
     * resolution is milliseconds. However, the zero point of the counter
     * is undefined.
     *
     * @author Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de)
     */
    class performance_counter : comparable<performance_counter> {

    public:

        /** 
         * The type that is used to specify differences between counter values.
         */
        typedef int64_t difference_type;

        /**
         * The type that is used to specify counter non-integral values e.g.
         * milliseconds.
         */
        typedef double fractional_value_type;

        /** The type that counter values are stored in. */
        typedef uint64_t value_type;

        /**
         * Query the performance counter's native frequency in counts per
         * second.
         *
         * @return The performance counter frequency in counts per second.
         *
         * @throws the::system::system_exception If the frequency could not be 
         *                                       queried.
         */
        static value_type frequency(void);

        /**
         * Query the performance counter for its current value.
         *
         * @return The current performance counter value. This value must be
         *         divided by frequency() to yield seconds.
         *
         * @throws the::system::system_exception If the performance counter
         *                                       could not be queried.
         */
        static value_type query(void);

        /**
         * Query the performance counter for its current value in milliseconds.
         * Depending on the capabilities of the underlying hardware, the value
         * might have a fractional part.
         *
         * Using this method is equivalent to the following code:
         *
         * performance_counter::value_type value = performance_counter::query();
         * performance_counter::value_type frequency
         *     = performance_counter::frequency();
         * performance_counter::fractional_value_type result
         *     = static_cast<fractional_value_type>(fractional_value_type)
         *     * performance_counter::milliseconds_per_second
         *     / static_cast<fractional_value_type>(frequency);
         *
         * @return The current perfomance counter value in milliseconds.
         *
         * @throws the::system::system_exception If the performance counter
         *                                       could not be queried, or
         *                                       if the frequency could not be
         *                                       queried.
         */
        inline static fractional_value_type query_millis(void) {
            THE_STACK_TRACE;
            return performance_counter::to_milliseconds(
                performance_counter::query());
        }

        /**
         * Convert a full resolution performance counter value to milliseconds.
         *
         * @param value The performance counter value with the full resolution 
         *              of the underlying system.
         * 
         * @return The milliseconds that 'value' represents.
         *
         * @throws the::system::system_exception If the frequency could not be 
         *                                       queried.
         */
        static fractional_value_type to_milliseconds(const value_type value);

        /** The number of microseconds one scond comprises. */
        static const value_type microseconds_per_second;

        /** The number of milliseconds one second comprises. */
        static const value_type milliseconds_per_second;

        /** The number of nanoseconds one second comprises. */
        static const value_type nanoseconds_per_second;

        /**
         * Initialises a new instance.
         */
        inline performance_counter(void) : base(), startValue(0) {
            THE_STACK_TRACE;
        }

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline performance_counter(const performance_counter& rhs) : base() {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /** Dtor. */
        virtual ~performance_counter(void);

        /**
         * Compares this object to 'rhs'.
         *
         * @param rhs The object to be compared.
         *
         * @return A negative number if this object is less than 'rhs', 
         *         zero if this object is equal to 'rhs',
         *         a positive number if this object is larger thatn 'rhs'.
         */
        virtual int compare(const performance_counter& rhs) const;

        /**
         * Answer the difference (in native counter ticks) between the current
         * performance counter value and the start mark.
         *
         * @return The difference between now and the mark.
         */
        inline difference_type elapsed(void) const {
            THE_STACK_TRACE;
            difference_type c = static_cast<difference_type>(
                performance_counter::query());
            difference_type s = static_cast<difference_type>(this->startValue);
            return (c - s);
        }

        /**
         * Answer the difference between the current performance counter value
         * and the mark in milliseconds.
         *
         * @return The difference between now and the mark in milliseconds.
         */
        inline fractional_value_type elapsed_milliseconds(void) const {
            THE_STACK_TRACE;
            fractional_value_type c = performance_counter::query_millis();
            fractional_value_type s = performance_counter::to_milliseconds(
                this->startValue);
            return (c - s);
        }

        /**
         * Set a start mark for a new measurement.
         *
         * @return The new value of the mark, i.e. the current performance 
         *         counter value.
         */
        inline value_type start(void) {
            THE_STACK_TRACE;
            return (this->startValue = performance_counter::query());
        }

        /**
         * Answer the value of the performance counter when the start mark was
         * set.
         *
         * @return The start value.
         */
        inline value_type start_value(void) const {
            THE_STACK_TRACE;
            return this->startValue;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        performance_counter& operator =(const performance_counter& rhs);

    private:

        /** Super class typedef. */
        typedef comparable<performance_counter> base;

#ifdef THE_LINUX
        /** Defines availability of different clock implementations on Linux. */
        typedef enum has_rtc_type_t {

            /**
             * The initial value of the 'hasRtc' member in case glibc does not
             * indicate by compile-time constants that the RTC is guaranteed to
             * be available. If this value is set, the performance counter will
             * determine availability of a real-time clock when the clock is
             * first queried.
             */
            unknown,

            /** Indicates that the real-time clock is available. */
            yes,

            /**
             * Indicate that the real-time clock is not available and we must
             * fall back to gettimeofday for getting the time.
             */
            no
        } has_rtc_type;

        /**
         * Determines whether the system supports the POSIX real-time clock API.
         *
         * @return has_rtc_type::yes if the RTC is available, has_rtc_type::no
         *         otherwise.
         */
        static has_rtc_type check_rtc_availability(void);
#endif /* THE_LINUX */

        /**
         * Caches the frequency, which cannot change while the system is 
         * running.
         */
        static value_type frequencyValue;

#if (defined(THE_LINUX) && defined(_POSIX_TIMERS))
        /** Determines whether Linux system has real-time clock. */
        static has_rtc_type hasRtc;
#endif /* (defined(THE_LINUX) && defined(_POSIX_TIMERS)) */

        /**
         * The mark, i.e. the performance counter value when the last
         * measurement was started.
         */
        value_type startValue;
    };

} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_PERFORMANCE_COUNTER_H_INCLUDED */
