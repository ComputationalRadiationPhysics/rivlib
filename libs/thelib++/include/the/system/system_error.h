/*
 * include\the\system\system_error.h
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

#ifndef THE_SYSTEM_SYSTEM_ERROR_H_INCLUDED
#define THE_SYSTEM_SYSTEM_ERROR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <windows.h>
#endif /* THE_WINDOWS */
#include <errno.h>

#include "the/comparable.h"
#include "the/force_inline.h"
#include "the/stack_trace.h"



namespace the {
namespace system {

    /**
     * This class provides a wrapper around native system error codes, e.g.
     * errno.
     */
    class system_error : comparable<system_error> {

    public:

        /** The type of an error code. */
#if defined(THE_WINDOWS)
        typedef DWORD native_error_type;
#elif defined(THE_LINUX)
        typedef int native_error_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::system_error must define native_error_type!"
#endif /* defined(THE_WINDOWS) */

        /**
         * Retrieves the last error code value that was set by a system call.
         *
         * On Windows, this returns the value returned by the ::GetLastError()
         * function.
         *
         * On Linux, this returns errno.
         *
         * @return The last error code value.
         */
        static THE_FORCE_INLINE system_error last(void) {
            return system_error(system_error::get_last_error());
        }

        /**
         * Retrieves the last error code value that was set by a system call.
         *
         * On Windows, this returns the value returned by the ::GetLastError()
         * function.
         *
         * On Linux, this returns errno.
         *
         * @return The last error code value.
         */
        static THE_FORCE_INLINE native_error_type get_last_error(void) {
#ifdef THE_WINDOWS
            return ::GetLastError();
#else /* THE_WINDOWS */
            return errno;
#endif /* THE_WINDOWS */
        }

        /** The constant that is used to represent a non-error. */
        static const native_error_type success;

        /**
         * Initialises a new instance.
         *
         * This constructor enables implicit casts from native_error_type.
         *
         * @param error The native error code to be wrapped. This defaults to
         *              system_error::success
         */
        system_error(const native_error_type error = system_error::success);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        system_error(const system_error& rhs);

        /** Dtor. */
        virtual ~system_error(void);

        /**
         * Compares this object to 'rhs'.
         *
         * @param rhs The object to be compared.
         *
         * @return A negative number if this object is less than 'rhs', 
         *         zero if this object is equal to 'rhs',
         *         a positive number if this object is larger thatn 'rhs'.
         */
        virtual int compare(const system_error& rhs) const;

        /**
         * Determines whether the system error code represents a non-error.
         *
         * @return true if this error code is system_error::success,
         *         false otherwise.
         */
        inline bool is_success(void) const {
            THE_STACK_TRACE;
            return (this->error == system_error::success);
        }

        /**
         * Retrieves the native error code.
         *
         * @return The native error code.
         */
        inline native_error_type native_error(void) const {
            THE_STACK_TRACE;
            return this->error;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline system_error& operator =(const system_error& rhs) {
            THE_STACK_TRACE;
            this->error = rhs.error;
            return *this;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline system_error& operator =(const native_error_type& rhs) {
            THE_STACK_TRACE;
            this->error = rhs;
            return *this;
        }

        /**
         * Compare the system error with the given native error code.
         *B
         * @param rhs The right hand side operand.
         *
         * @return true if this error and 'rhs' are equal, false otherwise.
         */
        inline bool operator ==(const native_error_type& rhs) const {
            THE_STACK_TRACE;
            return (this->error == rhs);
        }

        /**
         * Compare the system error with the given native error code.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this error and 'rhs' are not equal, false otherwise.
         */
        inline bool operator !=(const native_error_type& rhs) const {
            THE_STACK_TRACE;
            return (this->error != rhs);
        }

    private:

        /** The native error code. */
        native_error_type error;
    };

} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_SYSTEM_ERROR_H_INCLUDED */
