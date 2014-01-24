/*
 * include\the\system\net\client_connected_event_args.h
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

#ifndef THE_SYSTEM_NET_CLIENT_CONNECTED_EVENT_ARGS_H_INCLUDED
#define THE_SYSTEM_NET_CLIENT_CONNECTED_EVENT_ARGS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <WinSock2.h>
#endif /* THE_WINDOWS */

#include "the/event_args.h"
#include "the/not_copyable.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace net {

    /**
     * This event structure is used to communicate that a new client connected
     * to a the::system::net::server.
     *
     * The registered listeners can choose to accept the connection by calling
     * client_connected_event_args::accept(). If the first listener does so, the
     * server assumes that the listener took ownership of the socket. If no
     * listener takes ownership, the socket will be closed afterwards.
     *
     * @tparam T The client socket type.
     */
    template<class T> class client_connected_event_args
        : public event_args<client_connected_event_args<T> >, public not_copyable {

    public:

        /** The type of addresses to specify end points. */
        typedef typename T::end_point_type end_point_type;

        /** The server socket type. */
        typedef T socket_type;

        /**
         * Initialises a new instance.
         *
         * @param clientSocket The socket that was accepted by the server.
         */
        client_connected_event_args(socket_type& clientSocket,
            const end_point_type& clientAddress);

        /** Dtor. */
        virtual ~client_connected_event_args(void);

        /**
         * Answer the address of the client that connected.
         *
         * @return The address of the remote end point.
         */
        inline const end_point_type& client_address(void) const {
            THE_STACK_TRACE;
            return this->clientAddress;
        }

        /**
         * Accepts the client and prevents other listeners being notified.
         *
         * @return The socket representing the new connection.
         */
        inline socket_type&& accept(void) {
            THE_STACK_TRACE;
            this->isAccepted = true;
            return std::move(this->clientSocket);
        }

        /**
         * Answers whether some one has called accept() before.
         *
         * @return true If the connection has been accepted before,
         *         false otherwise.
         */
        inline bool is_accepted(void) const {
            THE_STACK_TRACE;
            return this->isAccepted;
        }

    private:

        /** First superclass typedef. */
        typedef event_args<client_connected_event_args<T> > base1;

        /** Secon superclass typedef. */
        typedef not_copyable base2;

        /** Remember whether a listener has accepted the clients. */
        bool isAccepted;

        /** The address of the client that has been accepted. */
        end_point_type clientAddress;

        /** The client socket that has been accepted. */
        socket_type clientSocket;
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::client_connected_event_args<T>::client_connected_event_args
 */
template<class T>
the::system::net::client_connected_event_args<T>::client_connected_event_args(
        socket_type& clientSocket, const end_point_type& clientAddress) 
        : base1(), base2(), isAccepted(false), clientAddress(clientAddress),
        clientSocket(std::move(clientSocket)) {
    THE_STACK_TRACE;
}


/*
 * ...::net::client_connected_event_args<T>::~client_connected_event_args
 */
template<class T>
the::system::net::client_connected_event_args<T>::~client_connected_event_args(
        void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_CLIENT_CONNECTED_EVENT_ARGS_H_INCLUDED */
