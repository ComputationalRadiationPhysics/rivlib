/*
 * rivlib
 * ip_communicator_impl.h
 *
 * Copyright TUD 2013
 * Alle Rechte vorbehalten. All rights reserved
 */

#ifndef VICCI_RIVLIB_IP_COMMUNICATOR_IMPL_H_INCLUDED
#define VICCI_RIVLIB_IP_COMMUNICATOR_IMPL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "rivlib/common.h"
#include "rivlib/ip_communicator.h"
#include "element_node.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/thread.h"
#include "vislib/Socket.h"
#include "vislib/SmartRef.h"
#include "vislib/TcpCommChannel.h"


namespace eu_vicci {
namespace rivlib {


    /**
     * The communication connection using ip protocols
     */
    class ip_communicator_impl : public ip_communicator, public element_node,
        public the::system::threading::runnable {
    public:

        /**
         * ctor
         *
         * @param port The port to listen on
         */
        ip_communicator_impl(unsigned short port = DEFAULT_PORT);

        /** dtor */
        virtual ~ip_communicator_impl(void);

        /**
         * Called to fire the 'core_discovered' event
         */
        virtual void on_core_discovered(void);

        /**
         * Called to fire the 'core_lost' event
         */
        virtual void on_core_lost(void);

        /**
         * Terminates the server thread
         *
         * @return termination_behaviour::graceful
         */
        virtual the::system::threading::thread::termination_behaviour
            on_thread_terminating(void) throw();

        /**
         * The thread main method
         *
         * @return The thread return value
         */
        virtual int run(void);

        /**
         * Answer the number of public uris the specified provider will be
         * available through this communicator
         *
         * @param prov The provider
         *
         * @return The number of public uris
         */
        virtual size_t count_public_uris(provider::ptr prov);

        /**
         * Copies one public uri to 'buf' under which the specified provider
         * will be available through this communicator
         *
         * @param prov The provider
         * @param idx The zero-based index of the public uri to be returned
         * @param buf The buffer to write the uri to. If 'nullptr' no data
         *            will be written
         * @param buflen The number of bytes that can safely be written to
         *            'buf'
         *
         * @return If 'buf' is nullptr, returns the length of the requested
         *         uri in bytes/characters. If 'buf' is not nullptr, return
         *         the number of bytes/characters written.
         */
        virtual size_t public_uri(provider::ptr prov, size_t idx, char *buf, size_t buflen);

    protected:

    private:

        /**
         * Rebuilds the public uri cache is the specified provider is not the
         * one the current public uri cache was built for
         *
         * @param prov The provided to use the public uri cache on next
         */
        void assert_public_uri_cache(provider::ptr prov);

        /** The tcp port to listen at */
        unsigned short port;

        /** The server comm channel */
        vislib::SmartRef<vislib::net::TcpCommChannel> comm;

        /** The server worker thread */
        the::system::threading::thread *worker;

        /** The provider the public uri cache was built for */
        provider* public_uri_cache_src;

        /** The public uri cache */
        std::vector<std::string> public_uri_cache;

        /** The lock object when accessing the public uri cache */
        the::system::threading::critical_section public_uri_cache_lock;

    };


} /* end namespace rivlib */
} /* end namespace eu_vicci */

#endif /* VICCI_RIVLIB_IP_COMMUNICATOR_IMPL_H_INCLUDED */
