/*
 * the/system/net/socket_exception.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
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
 * SocketException.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_NET_SOCKET_EXCEPTION_H_INCLUDED
#define THE_SYSTEM_NET_SOCKET_EXCEPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/stack_trace.h"

#include "the/system/system_exception.h"


namespace the {
namespace system {
namespace net {

    /**
     * This exception indicates a socket error.
     */
    class socket_exception : public system_exception {

    public:

        /** The native error code for a timeout. */
        static const native_error_type error_timeout;

        /**
         * Create a new exception using the system message for the given
         * error code as exception message.
         *
         * @param errorCode A socket error code.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        socket_exception(const native_error_type errorCode, const char *file,
            const int line);

        /**
         * Create a new exception using the specified error code, but
         * overwrite the message with the specified value. If 'msg' is
         * NULL, this ctor behaves exactly as socket_exception(
         * const native_error_type errorCode, const char *file, const int line).
         *
         * @param errorCode A socket error code.
         * @param msg       A user-defined message to be used instead of
         *                  the system message associated with the given
         *                  error code.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        socket_exception(const native_error_type errorCode, const char *msg,
            const char *file, const int line);

        /**
         * Create a new exception using the specified error code, but
         * overwrite the message with the specified value. If 'msg' is
         * NULL, this ctor behaves exactly as socket_exception(
         * const native_error_type errorCode, const char *file, const int line).
         *
         * @param errorCode A socket error code.
         * @param msg       A user-defined message to be used instead of
         *                  the system message associated with the given
         *                  error code.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        socket_exception(const native_error_type errorCode, const wchar_t *msg,
            const char *file, const int line);

        /**
         * Create a new exception using the last socket error code, i. e.
         * ::WSAGetLastError() on Windows or ::errno on Linux systems.
         *
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        socket_exception(const char *file, const int line);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        socket_exception(const socket_exception& rhs);

        /** Dtor. */
        virtual ~socket_exception(void) throw();

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        virtual socket_exception& operator =(const socket_exception& rhs);

        /**
         * Answer whether the exception represents a timeout.
         *
         * @return true, if the exception represents a timeout, false otherwise.
         */
        inline bool is_timeout(void) const {
            THE_STACK_TRACE;
            return (this->get_error() == socket_exception::error_timeout);
        }

    protected:

        /** Super class typedef. */
        typedef system_exception base;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/**
 * Throws a the::system::net::socket_exception with the last system error code.
 */
#define THROW_THE_SOCKET_EXCEPTION throw the::system::net::socket_exception(\
    __FILE__, __LINE__)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SOCKET_EXCEPTION_H_INCLUDED */
