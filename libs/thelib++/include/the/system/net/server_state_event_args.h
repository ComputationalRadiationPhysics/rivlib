/*
 * include\the\system\net\server_state_event_args.h
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

#ifndef THE_SYSTEM_NET_SERVER_STATE_EVENT_ARGS_H_INCLUDED
#define THE_SYSTEM_NET_SERVER_STATE_EVENT_ARGS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/event_args.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace net {

    /**
     * These event arguments are used to communicate state changes of the
     * network server thread.
     */
    class server_state_event_args
            : public the::event_args<server_state_event_args> {

    public:

        /** The possible states of the server. */
        typedef enum state_type_t {
            unknown = 0,
            starting,
            running,
            stopping,
            stopped,
        } state_type;

        /**
         * Initialises a new instance.
         */
        server_state_event_args(void);

        /**
         * Initialises a new instance.
         *
         * @param state The state to be transported.
         */
        server_state_event_args(const state_type state);

        /**
         * Initialises a new instance.
         *
         * @param oldState The previous state of the server.
         * @param newState The new state of the server.
         */
        server_state_event_args(const state_type oldState,
            const state_type newState);

        /**
         * Initialises a new instance.
         *
         * @param oldState The previous state of the server.
         * @param newState The new state of the server.
         */
        server_state_event_args(const server_state_event_args& oldState,
            const state_type newState);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        server_state_event_args(const server_state_event_args& rhs);

        /** Dtor. */
        virtual ~server_state_event_args(void);

        /**
         * Answer the previous state of the server.
         *
         * @return The previous state of the server.
         */
        inline state_type old_state(void) const {
            THE_STACK_TRACE;
            return this->oldState;
        }

        /**
         * Answer the new state of the server.
         *
         * @return The new state of the server.
         */
        inline state_type new_state(void) const {
            THE_STACK_TRACE;
            return this->newState;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        server_state_event_args& operator =(const server_state_event_args& rhs);

    protected:

        /** Super class typedef. */
        typedef the::event_args<server_state_event_args> base;

        /** The previous server state. */
        state_type oldState;

        /** The new server state. */
        state_type newState;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SERVER_STATE_EVENT_ARGS_H_INCLUDED */
