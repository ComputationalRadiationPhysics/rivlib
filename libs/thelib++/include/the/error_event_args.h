/*
 * include\the\error_event_args.h
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

#ifndef THE_ERROR_EVENT_ARGS_H_INCLUDED
#define THE_ERROR_EVENT_ARGS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <exception>

#include "the/event_args.h"


namespace the {

    /**
     * This is a utility class encapsulating a std::exception_ptr for use
     * in the::observables.
     *
     * Note: If you derive from this class, you should override the typedef
     * made by event_args<error_event_args> in order to prevent ambiguities.
     */
    class error_event_args : public event_args<error_event_args> {

    public:

        /**
         * Initialises a new instance.
         */
        error_event_args(void);

        /**
         * Initialises a new instance.
         *
         * @param exPtr The exception representing the error.
         */
        error_event_args(const std::exception_ptr& exPtr);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        error_event_args(const error_event_args& rhs);

        /** Dtor. */
        virtual ~error_event_args(void);

        /**
         * Answer the exception transported by the event.
         *
         * @return The exception.
         */
        inline const std::exception_ptr& error(void) const {
            THE_STACK_TRACE;
            return this->exPtr;
        }

        /**
         * Answer whether the event repesents an actual error.
         *
         * @return true if the event transports a exception, false otherwise.
         */
        bool is_error(void) const;

        /**
         * If the event represents an error, rethrow the exception.
         *
         * It is save to call this method if this->is_error() returns false.
         * Nothing will happen in this case.
         *
         * @throws this->error() in case it represents a real exception.
         */
        void rethrow(void) const;

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        error_event_args& operator =(const error_event_args& rhs);

    private:

        /** Superclass typedef. */
        typedef event_args<error_event_args> base;

        /** Instance representing no exception. */
        static const std::exception_ptr EMPTY_EXCEPTION;

        /** The exception representing the error. */
        std::exception_ptr exPtr;

    };

} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_ERROR_EVENT_ARGS_H_INCLUDED */
