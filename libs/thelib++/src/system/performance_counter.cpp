/*
 * src\system\performance_counter.cpp
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
 * PerformanceCounter.cpp  10.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#include "the/system/performance_counter.h"

#ifdef THE_WINDOWS
#include <windows.h>
#endif /* THE_WINDOWS */

#ifdef THE_LINUX
#include <ctime>
#include <sys/time.h>
#endif /* THE_LINUX */

#include "the/assert.h"

#include "the/math/functions.h"

#include "the/system/system_exception.h"


/*
 * the::system::performance_counter::frequency
 */
the::system::performance_counter::value_type
the::system::performance_counter::frequency(void) {
    THE_STACK_TRACE;

    if (performance_counter::frequencyValue == 0) {
#if defined(THE_WINDOWS)
        LARGE_INTEGER timerFreq;

        if (!::QueryPerformanceFrequency(&timerFreq)) {
            THROW_THE_SYSTEM_EXCEPTION;
        }

        performance_counter::frequencyValue = timerFreq.QuadPart;

#elif defined(THE_LINUX)
        switch (performance_counter::check_rtc_availability()) {
            case has_rtc_type::yes:
                // clock_gettime returns nanoseconds.
                performance_counter::frequencyValue
                    = performance_counter::nanoseconds_per_second;
                break;

            case has_rtc_type::no:
                // gettimeofday returns microseconds.
                performance_counter::frequencyValue
                    = performance_counter::microseconds_per_second;
                break;

            default:
                // This should be unreachable.
                THE_ASSERT(false);
                break;
        }

#else /* defined(THE_WINDOWS) */
#error "the::system::performance_counter::frequency must be implemented!"

#endif /* defined(THE_WINDOWS) */
    }

    return performance_counter::frequencyValue;
}


/*
 * the::system::performance_counter::query
 */
the::system::performance_counter::value_type
the::system::performance_counter::query(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    LARGE_INTEGER timerCount;

    if (!::QueryPerformanceCounter(&timerCount)) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

    return timerCount.QuadPart;

#elif defined(THE_LINUX)
#ifdef _POSIX_TIMERS
    switch (performance_counter::check_rtc_availability()) {
        case has_rtc_type::yes: {
            struct timespec timerCount;

            if (::clock_gettime(CLOCK_REALTIME, &timerCount) == -1) {
                THROW_THE_SYSTEM_EXCEPTION;
            }

            return static_cast<value_type>(timerCount.tv_nsec)
                + static_cast<value_type>(timerCount.tv_sec)
                * performance_counter::nanoseconds_per_second;
            } break;

        case has_rtc_type::no: {
#endif /* _POSIX_TIMERS */
            struct timeval timeValue;

            if (::gettimeofday(&timeValue, NULL) == -1) {
                THROW_THE_SYSTEM_EXCEPTION;
            }

            return static_cast<value_type>(timeValue.tv_usec)
                + static_cast<value_type>(timeValue.tv_sec)
                * performance_counter::microseconds_per_second;
#ifdef _POSIX_TIMERS
            } break;

        default:
            // This should be unreachable.
            THE_ASSERT(false);
            break;
    } /* end switch (performance_counter::hasRtc) */
#endif /* _POSIX_TIMERS */
#endif /* THE_WINDOWS */
}


/*
 * the::system::performance_counter::to_milliseconds
 */
the::system::performance_counter::fractional_value_type
the::system::performance_counter::to_milliseconds(const value_type value) {
    THE_STACK_TRACE;
    fractional_value_type v = static_cast<fractional_value_type>(value);
    fractional_value_type s = static_cast<fractional_value_type>(
        performance_counter::milliseconds_per_second);
    fractional_value_type f = static_cast<fractional_value_type>(
        performance_counter::frequency());
    return ((v * s) / f);
}


/*
 * the::system::performance_counter::microseconds_per_second
 */
const the::system::performance_counter::value_type
the::system::performance_counter::microseconds_per_second = 1000 * 1000;


/*
 * the::system::performance_counter::milliseconds_per_second
 */
const the::system::performance_counter::value_type
the::system::performance_counter::milliseconds_per_second = 1000;


/*
 * the::system::performance_counter::nanoseconds_per_second
 */
const the::system::performance_counter::value_type
the::system::performance_counter::nanoseconds_per_second = 1000 * 1000 * 1000;


/*
 * the::system::performance_counter::~performance_counter
 */
the::system::performance_counter::~performance_counter(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::performance_counter::compare
 */
int the::system::performance_counter::compare(
        const performance_counter& rhs) const {
    THE_STACK_TRACE;
    fractional_value_type l = performance_counter::to_milliseconds(
        this->startValue);
    fractional_value_type r = performance_counter::to_milliseconds(
        rhs.startValue);
    return static_cast<int>(math::signum(l - r));
}


/*
 * the::system::performance_counter::operator =
 */
the::system::performance_counter& the::system::performance_counter::operator =(
        const performance_counter& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->startValue = rhs.startValue;
    }
    return *this;
}


#ifdef THE_LINUX
/*
 * the::system::performance_counter::check_rtc_availability
 */
the::system::performance_counter::has_rtc_type
the::system::performance_counter::check_rtc_availability(void) {
    THE_STACK_TRACE;
#ifdef _POSIX_TIMERS
    if (performance_counter::hasRtc == has_rtc_type::unknown) {
        /*
         * The system provides the POSIX timer API, but it was not known at
         * compile time whether the real-time clock is available. Determine
         * availability at run time.
         */
        performance_counter::hasRtc = (::sysconf(_SC_THREAD_CPUTIME) > 0)
            ? has_rtc_type::yes : has_rtc_type::no;
    }

    return performance_counter::hasRtc;

#else /* _POSIX_TIMERS */
    return has_rtc_type::no;
#endif /* _POSIX_TIMERS */
}
#endif /* THE_LINUX  */


/*
 * the::system::performance_counter::frequencyValue
 */
the::system::performance_counter::value_type
the::system::performance_counter::frequencyValue = 0;


#if (defined(THE_LINUX) && defined(_POSIX_TIMERS))
/*
 * the::system::performance_counter::hasRtc
 */
the::system::performance_counter::has_rtc_type
the::system::performance_counter::hasRtc
#if (defined(_POSIX_THREAD_CPUTIME) && (_POSIX_THREAD_CPUTIME > 0))
    = the::system::performance_counter::has_rtc_type::yes;
#else /* (defined(_POSIX_THREAD_CPUTIME) && (_POSIX_THREAD_CPUTIME > 0)) */
    = the::system::performance_counter::has_rtc_type::unknown;
#endif /* (defined(_POSIX_THREAD_CPUTIME) && (_POSIX_THREAD_CPUTIME > 0)) */
#endif /* (defined(THE_LINUX) && defined(_POSIX_TIMERS)) */
