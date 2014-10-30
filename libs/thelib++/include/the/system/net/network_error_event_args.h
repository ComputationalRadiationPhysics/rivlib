/*
 * include\the\system\net\network_error_event_args.h
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

#ifndef THE_SYSTEM_NET_NETWORK_ERROR_EVENT_ARGS_H_INCLUDED
#define THE_SYSTEM_NET_NETWORK_ERROR_EVENT_ARGS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/error_event_args.h"
#include "the/not_copyable.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace net {

    /**
     * This specialised version of error_event_args enables listeners to
     * determine whether network classes like the server and message
     * dispatchers should continue doing their work once an error occurred.
     */
    class network_error_event_args : public the::error_event_args {

    public:

        /**
         * Override delegate type from superclass in order to prevent
         * ambiguities.
         */
        typedef the::observable<network_error_event_args,
            lock_type>::event_handler_type event_handler_type;

        /**
         * Override observable type from superclass in order to prevent
         * ambiguities.
         */
        typedef the::observable<network_error_event_args, lock_type>
            observable_type;

        /**
         * Initialises a new instance.
         */
        network_error_event_args(void);

        /**
         * Initialises a new instance.
         *
         * @param exPtr The exception representing the error.
         */
        network_error_event_args(const std::exception_ptr& exPtr);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline network_error_event_args(const network_error_event_args& rhs)
                : base() {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /** Dtor. */
        virtual ~network_error_event_args(void);

        /**
         * Answer if any of the listeners marked the error non-fatal.
         *
         * @return true if the event was marked non-fatal, false otherwise.
         */
        inline bool is_non_fatal(void) const {
            THE_STACK_TRACE;
            return this->isNonFatal;
        }

        /**
         * Marks the event as non-fatal.
         */
        void mark_non_fatal(void);

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        network_error_event_args& operator =(
            const network_error_event_args& rhs);

    private:

        /** Superclass typedef. */
        typedef error_event_args base;

        /** Remembers whether the error was marked non-fatal by a listener. */
        bool isNonFatal;
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_NETWORK_ERROR_EVENT_ARGS_H_INCLUDED */
