/*
 * include\the\system\net\discovery_service.h
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
 * ClusterDiscoveryService.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_NET_DISCOVERY_SERVICE_H_INCLUDED
#define THE_SYSTEM_NET_DISCOVERY_SERVICE_H_INCLUDED
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

#include "the/observable.h"
#include "the/not_copyable.h"

#include "the/system/net/discovery_event_args.h"
#include "the/system/net/ip_end_point.h"
#include "the/system/net/network_error_event_args.h"
#include "the/system/net/socket.h"

#include "the/system/threading/critical_section.h"
#include "the/system/threading/runnable_thread.h"


namespace the {
namespace system {
namespace net {

    /**
     * TODO: Document 'discovery_service'.
     */
    class discovery_service : public not_copyable,
            public observable<discovery_event_args>,
            public network_error_event_args::observable_type {

    public:

        /** Observable base class for discovery events. */
        typedef observable<discovery_event_args> discovery_observable_type;

        /** The event parameters for describing a server error. */
        typedef the::system::net::network_error_event_args
            error_event_args_type;

        /** Observable base class for errors. */
        typedef error_event_args_type::observable_type error_observable_type;

        /** Event handler type for errors. */
        typedef error_event_args_type::event_handler_type error_handler_type;

        /** The default port number used by the discovery service. */
        static const uint16_t default_port;

        /**
         * Initialises a new instance.
         */
        discovery_service(const std::string& name,
            const ip_end_point& responseAddr,
            const ip_address4& bcastAddr,
            const uint16_t port = default_port);

        /**
         * Initialises a new instance.
         */
        //discovery_service(const std::string& name,
        //    const ip_end_point& bcastAddr,
        //    const uint16_t port);

        /** Dtor. */
        virtual ~discovery_service(void);

        using discovery_observable_type::add_observer;
        using error_observable_type::add_observer;

        /**
         * Advertises this node, i.e. broadcasts a discovery packet.
         *
         * Note that you must have started the discovery_service before you can
         * advertise a node.
         * Note that a node will advertise itself if the discovery_service is
         * started.
         *
         * @throws the::system::net::socket_exception 
         */
        void advertise(void);

        using discovery_observable_type::remove_observer;
        using error_observable_type::remove_observer;

        /**
         * Starts the receiver thread that will asynchronously detect other
         * nodes and advertises this node a single time.
         *
         * @throws the::system::net::socket_exception 
         * @throws the::invalid_operation_exception If the discovery_service is
         *                                          already running.
         */
        void start(void);

        /**
         * Stops the receiver thread.
         *
         * @throws the::system::net::socket_exception If unloading the socket
         *                                            API fails (Windows only).
         */
        void stop(void);

    private:

        /**
         * This class implements a worker thread that is waiting for discovery
         * messages and raising the appropriate events.
         */
        class discovery_worker : public threading::runnable {

        public:

            /**
             * Initialises a new instance.
             */
            discovery_worker(void);

            /** Dtor.*/
            virtual ~discovery_worker(void);

            /**
             * Initialise the worker synchronously.
             *
             * @param userData A pointer to the enclosing discovery_service.
             */
            virtual void on_thread_starting(void *userData) throw();

            /**
             * Requests the worker to exit.
             *
             * @return thread::termination_behaviour::graceful.
             */
            virtual threading::thread::termination_behaviour
                on_thread_terminating(void) throw();

            /**
             * Wait for discovery broadcasts and dispatch those to registered
             * listeners.
             *
             * @return 0
             */
            virtual int run(void);

        private:

            /** Stores the discovery_service the dispatcher works for. */
            discovery_service *owner;

            /** The socket used for receiving broadcast messages. */
            socket receiverSocket;
        }; /* end class discovery_worker */

        /** The magic number at the begin of each message. */
        //static const UINT32 MAGIC_NUMBER;

        /** This is the broadcast address to send requests to. */
        ip_end_point bcastAddr;

        /** The address that the response thread binds to. */
        ip_end_point bindAddr;

        /** Lock for protecting 'senderSocket'. */
        threading::critical_section lockSenderSocket;

        /** The name of the cluster this discovery service should form. */
        std::string name;

        /** The address we send in a response message. */
        ip_end_point responseAddr;

        /** The thread receiving discovery requests. */
        threading::runnable_thread<discovery_worker> receiverThread;

        /** The socket used for advertising this node. */
        socket senderSocket;
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_DISCOVERY_SERVICE_H_INCLUDED */
