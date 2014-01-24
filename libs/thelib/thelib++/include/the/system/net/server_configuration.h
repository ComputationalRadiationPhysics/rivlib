/*
 * include\the\system\net\server_configuration.h
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

#ifndef THE_SYSTEM_NET_SERVER_CONFIGURATION_H_INCLUDED
#define THE_SYSTEM_NET_SERVER_CONFIGURATION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/memory.h"
#include "the/not_copyable.h"
#include "the/stack_trace.h"

#include "the/system/net/end_point.h"



namespace the {
namespace system {
namespace net {

    /**
     * This class contains the configuration for the::system::net::server.
     *
     * @tparam T The type of the server socket that is being used.
     */
    template<class T> class server_configuration : not_copyable {

    public:

        /** The type of end point addresses of servers. */
        typedef typename T::end_point_type end_point_type;

        /** The server socket type. */
        typedef T socket_type;

        /**
         * Initialises a new instance.
         */
        server_configuration(void);

        /**
         * Initialise a new instance with the given socket and end point.
         *
         * @param socket       The socket used by the server. The callee takes
         *                     ownership of the socket.
         * @param endPoint     The end point to bind the server to.
         * @param isTcpNoDelay Instructs the server to disable the Nagle
         *                     algorithm on all accepted clients.
         *
         * @throws the::argument_null_exception If 'socket' is nullptr.
         * @throws the::argument_null_exception If 'endPoint' is nullptr.
         */
        server_configuration(socket_type& socket,
            const end_point_type& endPoint,
            const bool isTcpNoDelay = false);

        /** Dtor. */
        virtual ~server_configuration(void);

    private:

        /**
         * Moves the content of 'rhs' into this object.
         *
         * This method is intended for the server to make its deep copy of the
         * user-provided configuration.
         *
         * @param rhs A pointer to the configuration being moved.
         */
        inline void move(server_configuration *rhs) {
            THE_STACK_TRACE;
            THE_ASSERT(rhs != nullptr);
            THE_ASSERT(this != rhs);
            this->endPoint = rhs->endPoint;
            this->flags = rhs->flags;
            this->socket = std::move(rhs->socket);
        }

        /** Superclass typedef. */
        typedef not_copyable base;

        /** The end point to bind the server to. */
        end_point_type endPoint;

        /** Stores boolean server flags. */
        struct flags_t {
            bool isTcpNoDelay : 1;
        } flags;

        /** The socket used by the server. */
        socket_type socket;

        /** Allow the server to access the internal resources. */
        template<class S, class C> friend class server;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::server_configuration<T>::server_configuration
 */
template<class T>
the::system::net::server_configuration<T>::server_configuration(void) {
    THE_STACK_TRACE;
    the::zero_memory(&this->flags);
}


/*
 * the::system::net::server_configuration<T>::server_configuration
 */
template<class T>
the::system::net::server_configuration<T>::server_configuration(
        socket_type& socket, const end_point_type& endPoint,
        const bool isTcpNoDelay)
        : endPoint(endPoint), socket(std::move(socket)) {
    THE_STACK_TRACE;
    the::zero_memory(&this->flags);
    this->flags.isTcpNoDelay = isTcpNoDelay;
}


/*
 * the::system::net::server_configuration<T>::~server_configuration
 */
template<class T>
the::system::net::server_configuration<T>::~server_configuration(void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SERVER_CONFIGURATION_H_INCLUDED */
