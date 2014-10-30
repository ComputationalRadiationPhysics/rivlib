/*
 * the/system/system_message.h
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
 * SystemMessage.h  15.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_SYSTEM_MESSAGE_H_INCLUDED
#define THE_SYSTEM_SYSTEM_MESSAGE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/stack_trace.h"

#include "the/system/system_error.h"
#include "the/system/system_string.h"


namespace the {
namespace system {

    /**
     * This class wraps the human readable string of a system error code. It 
     * does the lookup, allocates appropriate memory and releases the memory
     * on destruction.
     */
    class system_message {

    public:

        /** The type of an error code. */
        typedef system_error::native_error_type native_error_type;

        /**
         * Ctor.
         *
         * @param error A system-dependent error code.
         */
        system_message(const system_error::native_error_type error);

        /**
         * Ctor.
         *
         * @param error A system-dependent error code.
         */
        system_message(const system_error& error);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        system_message(const system_message& rhs);

        /**
         * Dtor.
         */
        ~system_message(void);

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        system_message& operator =(const system_message& rhs);

        /**
         * Cast to system_char string.
         *
         * This operator provides access to the human readable error message.
         * The returned pointer is valid until the object is destroyed.
         * The object remains owner of the memory allocated for the string.
         *
         * The error message string is created lazily.
         *
         * @return The human readable error message.
         *
         * @throws the::system::system_exception If the message for the error
         *                                       code could not be retrieved.
         */
        operator const system_char *(void) const;

        operator astring(void) const;

        operator wstring(void) const;

        /**
         * Answer the system-dependent error code associated with this 
         * message.
         *
         * @return The system error code.
         */
        inline system_error get_error(void) const {
            THE_STACK_TRACE;
            return this->error;
        }

    private:

        /**
         * Ensures that 'msg' has been initialised.
         *
         * @throws the::system::system_exception If the message for the error
         *                                       code could not be retrieved.
         */
        void assert_msg(void) const;

        /** A system-dependent error code. */
        system_error error;

        /** Caches message string (initialised lazily). */
        mutable system_char *msg;
    };

} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_SYSTEM_MESSAGE_H_INCLUDED */
