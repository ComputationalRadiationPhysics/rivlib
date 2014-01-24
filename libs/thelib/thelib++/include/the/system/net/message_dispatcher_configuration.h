/*
 * include\the\system\net\message_dispatcher_configuration.h
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

#ifndef THE_SYSTEM_NET_MESSAGE_DISPATCHER_CONFIGURATION_H_INCLUDED
#define THE_SYSTEM_NET_MESSAGE_DISPATCHER_CONFIGURATION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/not_copyable.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace net {

    /**
     * This class contains the configuration for a
     * the::system::net::simple_message_dispatcher.
     */
    template<class T>
    class message_dispatcher_configuration : public the::not_copyable {

    public:

        /** The client socket type. */
        typedef T socket_type;

        /**
         * Initialises a new instance.
         */
        message_dispatcher_configuration(void);

        /**
         * Initialises a new instance.
         *
         * @param socket
         */
        message_dispatcher_configuration(socket_type& socket);

        /** Dtor. */
        virtual ~message_dispatcher_configuration(void);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

        /**
         * Moves the content of 'rhs' into this object and invalidates 'rhs'.
         *
         * @param rhs Pointer to the configuration to be moved into this object.
         */
        inline void move(message_dispatcher_configuration *rhs) {
            THE_STACK_TRACE;
            THE_ASSERT(rhs != nullptr);
            THE_ASSERT(this != rhs);
            this->socket = std::move(rhs->socket);
        }

        /** The socket used by the dispatcher. */
        socket_type socket;

        /** Allow the server to access the internal resources. */
        template<class M, class S> friend class message_dispatcher;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * ...::message_dispatcher_configuration<T>::message_dispatcher_configuration
 */
template<class T>
the::system::net::message_dispatcher_configuration<T>
::message_dispatcher_configuration(void) : base() {
    THE_STACK_TRACE;
}


/*
 * ...::message_dispatcher_configuration<T>::message_dispatcher_configuration
 */
template<class T>
the::system::net::message_dispatcher_configuration<T>
::message_dispatcher_configuration(socket_type& socket)
        : base(), socket(std::move(socket)) {
    THE_STACK_TRACE;
}


/*
 * ...::message_dispatcher_configuration<T>::~message_dispatcher_configuration
 */
template<class T>
the::system::net::message_dispatcher_configuration<T>
::~message_dispatcher_configuration(void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_MESSAGE_DISPATCHER_CONFIGURATION_H_INCLUDED */
