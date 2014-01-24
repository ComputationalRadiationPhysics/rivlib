/*
 * include\the\system\net\server.h
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

#ifndef THE_SYSTEM_NET_SERVER_H_INCLUDED
#define THE_SYSTEM_NET_SERVER_H_INCLUDED
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
#include <exception>

#include "the/assert.h"
#include "the/not_copyable.h"
#include "the/observable.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/net/client_connected_event_args.h"
#include "the/system/net/debug.h"
#include "the/system/net/network_error_event_args.h"
#include "the/system/net/server_configuration.h"
#include "the/system/net/server_state_event_args.h"
#include "the/system/net/socket.h"

#include "the/system/threading/runnable.h"


namespace the {
namespace system {
namespace net {

    /**
     * A server implementation based on the::system::net::sockets.
     *
     * @tparam S The server socket type used for listening.
     * @tparam C The client socket type used for sending and receiving.
     */
    template<class S, class C>
    class server : public system::threading::runnable,
            public client_connected_event_args<C>::observable_type,
            public network_error_event_args::observable_type,
            public server_state_event_args::observable_type,
            public not_copyable {

    public:

        /** The event parameters for describing a new connection. */
        typedef the::system::net::client_connected_event_args<C>
            client_connected_event_args_type;

        /** Observable base class for new connections. */
        typedef typename client_connected_event_args_type::observable_type
            client_connected_observable_type;

        /** Event handler type for new connections. */
        typedef typename client_connected_event_args_type::event_handler_type
            client_connected_handler_type;

        /** The type of end point specifiying a client address. */
        typedef typename C::end_point_type client_end_point_type;

        /** The client socket type used for sending and receiving. */
        typedef C client_socket_type;

        /** The type encapsulating the server configuration. */
        typedef server_configuration<S> configuration_type;

        /** The event parameters for describing a server error. */
        typedef the::system::net::network_error_event_args
            error_event_args_type;

        /** Observable base class for errors. */
        typedef typename error_event_args_type::observable_type
            error_observable_type;

        /** Event handler type for errors. */
        typedef typename error_event_args_type::event_handler_type
            error_handler_type;

        /** The type of end point specifying a server address. */
        typedef typename S::end_point_type server_end_point_type;

        /** The server socket type used for listening. */
        typedef S server_socket_type;

        /** The event parameters for signaling a state change. */
        typedef the::system::net::server_state_event_args
            state_event_args_type;

        /** Observable base class for state changes. */
        typedef typename state_event_args_type::observable_type
            state_observable_type;

        /** Event handler type for server state changes. */
        typedef typename state_event_args_type::event_handler_type
            state_handler_type;

        /**
         * Initialises a new instance.
         */
        server(void);

        /** Dtor. */
        virtual ~server(void);

        using client_connected_event_args<C>::observable_type::add_observer;
        using network_error_event_args::observable_type::add_observer;
        using server_state_event_args::observable_type::add_observer;

        /**
         * Synchronously initialises the server via the 
         * server<S, C>::configuration_type passed as 'userData'.
         *
         * The method will synchronously deep-copy the configuration. Therefore,
         * it can be released once the start() method of the server thread
         * returns.
         *
         * @param userData A pointer to a server configuration.
         */
        virtual void on_thread_starting(void *userData) throw();

        /**
         * Terminates the server by closing the server socket.
         *
         * @return termination_behaviour::graceful.
         */
        virtual threading::thread::termination_behaviour on_thread_terminating(
            void) throw();

        using client_connected_event_args<C>::observable_type::remove_observer;
        using network_error_event_args::observable_type::remove_observer;
        using server_state_event_args::observable_type::remove_observer;

        /**
         * Runs the server operations.
         *
         * @return 0 in case of success, -1 otherwise.
         */
        virtual int run(void);

    protected:

        virtual bool on_notifying(const client_connected_handler_type& handler,
            client_connected_event_args_type& eventArgs) throw();

    private:

        /** The configuration of the server. */
        configuration_type config;

        /** 
         * Flag indicating that the server should proceed with accepting 
         * clients.
         */
        std::atomic<bool> doServe;

    };

    typedef server<socket, socket> socket_server;

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */



/*
 * the::system::net::server<S, C>::server
 */
template<class S, class C>
the::system::net::server<S, C>::server(void) : the::system::threading::runnable(),
        client_connected_observable_type(), error_observable_type(),
        state_observable_type(), the::not_copyable() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::server<S, C>::~server
 */
template<class S, class C> the::system::net::server<S, C>::~server(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::server<S, C>::on_thread_starting
 */
template<class S, class C>
void the::system::net::server<S, C>::on_thread_starting(void *userData) throw() {
    THE_STACK_TRACE;
    THE_ASSERT(userData != nullptr);
    this->config.move(static_cast<configuration_type *>(userData));
    this->doServe = true;
}


/*
 * the::system::net::server<S, C>::on_thread_terminating
 */
template<class S, class C>
the::system::threading::thread::termination_behaviour
the::system::net::server<S, C>::on_thread_terminating(void) throw() {
    THE_STACK_TRACE;
    try {
        this->doServe = false;
        this->config.socket.close();
    } catch (std::exception e) {
        THE_NETWORKING_TRACE_WARN("Exception when shutting down "
            "the::system::net::server: %s. This is usually no problem.\n",
            e.what());
    }
    return threading::thread::termination_behaviour::graceful;
}


/*
 * the::system::net::server<S, C>::run
 */
template<class S, class C> int the::system::net::server<S, C>::run(void) {
    THE_STACK_TRACE;
    client_end_point_type clientAddr;       // Address of connected client.
    error_event_args_type errorArgs;        // Transports possible errors.
    socket_options::tcp_no_delay noDelay;   // Configuration of Nagle's algo.
    int retval = 0;                         // Thread return value.
    state_event_args_type stateArgs;        // Transports state changes.

    stateArgs = state_event_args_type(
        state_event_args_type::state_type::starting);
    state_observable_type::notify(stateArgs);

    /* Prepare the socket subsystem. */
    try {
        socket::startup();
        THE_NETWORKING_TRACE_INFO("socket::startup succeeded in server.\n",
            nullptr);
    } catch (...) {
        THE_NETWORKING_TRACE_ERROR("socket::startup failed in server.\n",
            nullptr);
        errorArgs = std::current_exception();
        error_observable_type::notify(errorArgs);
        retval = -1;
    }

    /* Bind the end point. */
    try {
        this->config.socket.bind(this->config.endPoint);
        THE_NETWORKING_TRACE_INFO("Server socket bound to %s.\n",
            this->config.endPoint.to_astring().c_str());
    } catch (...) {
        THE_NETWORKING_TRACE_ERROR("Binding server socket failed.\n", nullptr);
        errorArgs = std::current_exception();
        error_observable_type::notify(errorArgs);
        retval = -1;
    }

    /* Put the connection in listening state. */
    try {
        this->config.socket.listen(socket::maximum_connections);
        THE_NETWORKING_TRACE_INFO("Server is listening...\n");
    } catch (socket_exception e) {
        THE_NETWORKING_TRACE_ERROR("socket::listen failed.\n", nullptr);
        errorArgs = std::current_exception();
        error_observable_type::notify(errorArgs);
        retval = -1;
    }

    /* Prepare socket options. */
    noDelay = this->config.flags.isTcpNoDelay;

    /* Enter server loop if no error so far. */
    if (retval == 0) {
        stateArgs = state_event_args_type(stateArgs,
            state_event_args_type::state_type::running);
        state_observable_type::notify(stateArgs);

        THE_NETWORKING_TRACE_DEBUG("Server is entering server loop...\n");
        while (this->doServe) {
            try {
                /* Wait for new client to connect. */
                client_socket_type client = this->config.socket.accept(
                    clientAddr);
                THE_NETWORKING_TRACE_INFO("Server accepted new client with "
                    "address %hs.\n", clientAddr.to_astring().c_str());

                // Configure Nagle's algorithm according to users's wishes.
                client.set_option(noDelay);

                // Notify the registered listeners.
                client_connected_event_args_type ea(
// TODO: Does not compile with GCC 4.7.3 for some reason
#ifdef THE_WINDOWS
std::move
#endif
(client),
                    clientAddr);
                client_connected_observable_type::notify(ea);
                if (!ea.is_accepted()) {
                    THE_NETWORKING_TRACE_INFO("None of the registered "
                        "listeners took ownership of the client with "
                        "address %hs. Closing connection...\n",
                        clientAddr.to_astring().c_str());
                    client.close();
                }
            } catch (...) {
                THE_NETWORKING_TRACE_ERROR("Error in server loop.\n", nullptr);
                errorArgs = std::current_exception();
                error_observable_type::notify(errorArgs);
                retval = -1;
                this->doServe = false;  // TODO: Make this configurable by listener
            }
        } /* end while (this->doServe) */
        THE_NETWORKING_TRACE_DEBUG("Server has left the server loop.\n",
            nullptr);
    }

    stateArgs = state_event_args_type(stateArgs,
        state_event_args_type::state_type::stopping);
    state_observable_type::notify(stateArgs);

    /* Clean up connection and socket library. */
    this->on_thread_terminating();

    try {
        socket::cleanup();
        THE_NETWORKING_TRACE_INFO("socket::cleanup succeeded in server.\n",
            nullptr);
    } catch (...) {
        THE_NETWORKING_TRACE_WARN("socket::cleanup failed in server.\n",
            nullptr);
        errorArgs = std::current_exception();
        error_observable_type::notify(errorArgs);
        retval = -1;
    }

    stateArgs = state_event_args_type(stateArgs,
        state_event_args_type::state_type::stopped);
    state_observable_type::notify(stateArgs);

    return retval;
}


/*
 * the::system::net::server<S, C>::on_notifying
 */
template<class S, class C> bool the::system::net::server<S, C>::on_notifying(
        const client_connected_handler_type& handler,
        client_connected_event_args_type& eventArgs) throw() {
    THE_STACK_TRACE;
    THE_NETWORKING_TRACE_DEBUG("Checking whether new connection has already "
        "been consumed...\n", nullptr);
    return !eventArgs.is_accepted();
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SERVER_H_INCLUDED */
