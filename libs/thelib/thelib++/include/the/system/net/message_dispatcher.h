/*
 * include\the\system\net\message_dispatcher.h
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
 * SimpleMessageDispatcher.h
 *
 * Copyright (C) 2006 - 2010 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_NET_MESSAGE_DISPATCHER_H_INCLUDED
#define THE_SYSTEM_NET_MESSAGE_DISPATCHER_H_INCLUDED
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

#include <atomic>

#include "the/not_copyable.h"
#include "the/not_implemented_exception.h"
#include "the/observable.h"
#include "the/stack_trace.h"

#include "the/system/net/debug.h"
#include "the/system/net/network_error_event_args.h"
#include "the/system/net/simple_message.h"
#include "the/system/net/message_dispatcher_configuration.h"
#include "the/system/net/socket.h"

#include "the/system/threading/runnable.h"


namespace the {
namespace system {
namespace net {

    /**
     * This class implements a runnable that is receiving THElib simple messages
     * via an inbound communication channel. It is intended to be run a a
     * separate that and therefore is derived from runnable.
     *
     * Clients probably want to register listeners for messages and errors.
     * These are delegates with the following signatures:
     * void OnMessage(message_dispatcher<M, S>::message_observable_type& src,
     *     message_dispatcher<M, S>::message_type& ea);
     * void OnError(message_dispatcher<M, S>::error_observable_type& src,
     *     message_dispatcher<M, S>::error_event_args_type& ea);
     */
    template<class M, class S> class message_dispatcher
            : public system::threading::runnable,
            public the::observable<M>,
            public network_error_event_args::observable_type,
            public the::not_copyable {

    public:

        /** The type encapsulating the server configuration. */
        typedef message_dispatcher_configuration<S> configuration_type;

        /** The event parameters for describing a server error. */
        typedef the::system::net::network_error_event_args
            error_event_args_type;

        /** Observable base class for errors. */
        typedef error_event_args_type::observable_type error_observable_type;

        /** Event handler type for errors. */
        typedef error_event_args_type::event_handler_type error_handler_type;

        /** Observable class for messages. */
        typedef the::observable<M> message_observable_type;

        /** Event handler type for messages. */
        typedef typename message_observable_type::event_handler_type
            message_handler_type;

        /** The type of the on-wire message header. */
        typedef typename M::header_value_type message_header_type;

        /** The type of the message begin transported. */
        typedef M message_type;

        /** The client socket type used for sending and receiving. */
        typedef S socket_type;

        /**
         * Initialises a new instance.
         */
        message_dispatcher(void);

        /** Dtor. */
        virtual ~message_dispatcher(void);

        using network_error_event_args::observable_type::add_observer;
        using the::observable<M>::add_observer;

        /**
         * Synchronously initialises the message dispatcher via the
         * message_dispatcher<T>::configuration_type passed as
         * 'userData'.
         *
         * The method will synchronously deep-copy the configuration. Therefore,
         * it can be released once the start() method of the server thread
         * returns.
         *
         * @param userData A pointer to a server configuration.
         */
        virtual void on_thread_starting(void *userData) throw();

        /**
         * Terminates the dispatcher by closing the socket.
         *
         * @return termination_behaviour::graceful.
         */
        virtual threading::thread::termination_behaviour on_thread_terminating(
            void) throw();

        using network_error_event_args::observable_type::remove_observer;
        using the::observable<M>::remove_observer;

        /**
         * Runs the server operations.
         *
         * @return 0 in case of success, -1 otherwise.
         */
        virtual int run(void);

    protected:

        /** The configuration of the dispatcher. */
        configuration_type config;

        /** Determines whether the dispatcher should continue receiving. */
        std::atomic<bool> doReceive;

    };


    /** Instantiation of message dispatcher that uses sockets. */
    typedef message_dispatcher<simple_message, socket>
        socket_simple_message_dispatcher;

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::message_dispatcher<M, S>::message_dispatcher
 */
template<class M, class S>
the::system::net::message_dispatcher<M, S>::message_dispatcher(void)
        : the::system::threading::runnable(), message_observable_type(),
        error_observable_type(), the::not_copyable() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::message_dispatcher<M, S>::~message_dispatcher
 */
template<class M, class S>
the::system::net::message_dispatcher<M, S>::~message_dispatcher(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::message_dispatcher<M, S>::on_thread_starting
 */
template<class M, class S>
void the::system::net::message_dispatcher<M, S>::on_thread_starting(
        void *userData) throw() {
    THE_STACK_TRACE;
    THE_ASSERT(userData != nullptr);
    this->config.move(static_cast<configuration_type *>(userData));
    this->doReceive = true;
}


/*
 * the::system::net::message_dispatcher<M, S>::on_thread_terminating
 */
template<class M, class S>
the::system::threading::thread::termination_behaviour
the::system::net::message_dispatcher<M, S>::on_thread_terminating(void)
        throw() {
    THE_STACK_TRACE;
    try {
        this->doReceive = false;
        this->config.socket.close();
    } catch (std::exception e) {
        THE_NETWORKING_TRACE_WARN("Exception when shutting down "
            "the::system::net::message_dispatcher: %s. This is usually "
            "no problem.\n", e.what());
    }
    return threading::thread::termination_behaviour::graceful;
}


/*
 * the::system::net::message_dispatcher<M, S>::run
 */
template<class M, class S>
int the::system::net::message_dispatcher<M, S>::run(void) {
    THE_STACK_TRACE;
    error_event_args_type errorArgs;        // Transports possible errors.
    message_type msg;                       // The message itself.
    int retval = 0;                         // Thread return value.

    /* Prepare the socket subsystem. */
    try {
        socket::startup();
        THE_NETWORKING_TRACE_INFO("socket::startup succeeded in "
            "message_dispatcher.\n", nullptr);
    } catch (...) {
        THE_NETWORKING_TRACE_ERROR("socket::startup failed in "
            "message_dispatcher.\n", nullptr);
        errorArgs = std::current_exception();
        error_observable_type::notify(errorArgs);
        retval = -1;
    }

    /* Enter receiver loop if no error so far. */
    if (retval == 0) {
        THE_NETWORKING_TRACE_DEBUG("message_dispatcher [%hs] is entering "
            "receiver loop...\n",
            threading::thread::get_current_id().to_astring().c_str());
        while (this->doReceive) {
            try {
                THE_NETWORKING_TRACE_DEBUG("[%hs] is waiting for message "
                    "header...\n",
                    threading::thread::get_current_id().to_astring().c_str());
                this->config.socket.receive(static_cast<void *>(msg),
                    msg.header_size(), true);
                THE_NETWORKING_TRACE_DEBUG("[%hs] received message header "
                    "{ id = %u, body_size = %u }.\n",
                    threading::thread::get_current_id().to_astring().c_str(),
                    msg.id(), msg.body_size());
                msg.assert_body_size();

                if (msg.body_size() > 0) {
    //            VLTRACE(Trace::LEVEL_VL_ANNOYINGLY_VERBOSE, "[%u] is waiting "
    //                "for message body ...\n", vislib::sys::Thread::CurrentID());
                    this->config.socket.receive(msg.body(), msg.body_size(),
                        true);
                    // TODO: Trace
                }

                THE_NETWORKING_TRACE_DEBUG("[%hs] is is dispatching message "
                    "to registered observers...\n",
                    threading::thread::get_current_id().to_astring().c_str());
                message_observable_type::notify(msg);
            } catch (...) {
                THE_NETWORKING_TRACE_ERROR("Error in dispatcher loop.\n",
                    nullptr);
                errorArgs = std::current_exception();
                error_observable_type::notify(errorArgs);
                retval = -1;
                this->doReceive = false;  // TODO: Make this configurable by listener
            }
        } /* end while (this->doReciev) */
        THE_NETWORKING_TRACE_DEBUG("message_dispatcher has left the receiver "
            "loop.\n", nullptr);
    }

    /* Clean up connection and socket library. */
    this->on_thread_terminating();

    try {
        socket::cleanup();
        THE_NETWORKING_TRACE_INFO("socket::cleanup succeeded in "
            "message_dispatcher.\n", nullptr);
    } catch (...) {
        THE_NETWORKING_TRACE_WARN("socket::cleanup failed in "
            "message_dispatcher.\n", nullptr);
        errorArgs = std::current_exception();
        error_observable_type::notify(errorArgs);
        retval = -1;
    }

    return retval;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_MESSAGE_DISPATCHER_H_INCLUDED */
