/*
 * include\the\system\net\socket_options.h
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

#ifndef THE_SYSTEM_NET_SOCKET_OPTIONS_H_INCLUDED
#define THE_SYSTEM_NET_SOCKET_OPTIONS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <WinSock2.h>
#include <Windows.h>
#elif THE_LINUX
#include <arpa/inet.h>
#include <netinet/tcp.h>
#endif /* THE_WINDOWS */

#include "the/not_instantiable.h"

#include "the/system/net/socket_option.h"


namespace the {
namespace system {
namespace net {

    /**
     * This class provdes constant values for well-known socket options.
     */
    class socket_options : public the::not_instantiable {

    public:

        /** The type used for storing boolean options. */
        typedef unsigned int bool_storage_type;

        /** The type used for storing timeout options. */
        typedef unsigned int timeout_storage_type;

        /** Configures a socket for sending broadcast data. */
        typedef socket_option<SOL_SOCKET, SO_BROADCAST,
            bool_storage_type> broadcast;

#if defined(THE_WINDOWS)
        /**
         * Enables incoming connections are to be accepted or rejected by the
         * application, not by the protocol stack. 
         */
        typedef socket_option<SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
            bool_storage_type> conditional_accept;
#endif /* defined(THE_WINDOWS) */

        /**
         * Enables debug output.
         * 
         * Microsoft providers currently do not output any debug information.
         */
        typedef socket_option<SOL_SOCKET, SO_DEBUG, bool_storage_type> debug;

#if defined(THE_WINDOWS)
        /**
         * Does not block close waiting for unsent data to be sent. Setting this
         * option is equivalent to setting SO_LINGER with l_onoff set to zero.
         */
        typedef socket_option<SOL_SOCKET, SO_DONTLINGER,
            bool_storage_type> do_not_linger;
#endif /* defined(THE_WINDOWS) */

        /**
         * Sets whether outgoing data should be sent on interface the socket is
         * bound to and not a routed on some other interface.
         *
         * This option is not supported on ATM sockets (results in an error).
         */
        typedef socket_option<SOL_SOCKET, SO_DONTROUTE,
            bool_storage_type> do_not_route;

#if defined(THE_WINDOWS)
        /**
         * Enables sending keep-alive packets for a socket connection.
         *
         * Not supported on ATM sockets (results in an error).
         */
        typedef socket_option<SOL_SOCKET, SO_GROUP_PRIORITY,
            int> group_priority;
#endif /* defined(THE_WINDOWS) */

        /** Lingers on close if unsent data is present. */
        typedef socket_option<SOL_SOCKET, SO_LINGER, struct linger> linger;

        typedef socket_option<IPPROTO_IP, IP_ADD_MEMBERSHIP,
            struct ip_mreq> multicast_add_membership4;

        typedef socket_option<IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP,
            struct ipv6_mreq> multicast_add_membership6;

        typedef socket_option<IPPROTO_IP, IP_DROP_MEMBERSHIP,
            struct ip_mreq> multicast_drop_membership4;

        typedef socket_option<IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP,
            struct ipv6_mreq> multicast_drop_membership6;

        typedef socket_option<IPPROTO_IP, IP_MULTICAST_IF, struct in_addr>
            multicast_interface4;

        typedef socket_option<IPPROTO_IPV6, IPV6_MULTICAST_IF, struct in6_addr>
            multicast_interface6;

//         * In Winsock, the IP_MULTICAST_LOOP option applies only to the 
//         * receive path. 
//         * In the UNIX version, the IP_MULTICAST_LOOP option applies to the 
//         * send path.
        typedef socket_option<IPPROTO_IP, IP_MULTICAST_LOOP,
            bool_storage_type> multicast_loop4;

//         * In Winsock, the IP_MULTICAST_LOOP option applies only to the 
//         * receive path. 
//         * In the UNIX version, the IP_MULTICAST_LOOP option applies to the 
//         * send path.
        typedef socket_option<IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
            bool_storage_type> multicast_loop6;

        typedef socket_option<IPPROTO_IP, IP_MULTICAST_TTL, unsigned char>
            multicast_ttl;

        /**
         * Indicates that out-of-bound data should be returned in-line with
         * regular data.
         *
         * This option is only valid for connection-oriented protocols that
         * support out-of-band data.
         */
        typedef socket_option<SOL_SOCKET, SO_OOBINLINE,
            bool_storage_type> out_of_bound_inline;

        /**
         * Specifies the total per-socket buffer space reserved for receives.
         */
        typedef socket_option<SOL_SOCKET, SO_RCVBUF, int> receive_buffer;

        /**
         * Allows the socket to be bound to an address that is already in use.
         * 
         * For more information, see bind. Not applicable on ATM sockets.
         */
        typedef socket_option<SOL_SOCKET, SO_REUSEADDR,
            bool_storage_type> reuse_address;

#if defined(THE_WINDOWS)
        /**
         * Enables a socket to be bound for exclusive access.
         *
         * Does not require administrative privilege on Windows.
         */
        typedef socket_option<SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
            bool_storage_type> exclusive_address_use;
#endif /* defined(THE_WINDOWS) */

        /** Sets the timeout, in milliseconds, for blocking receive calls. */
        typedef socket_option<SOL_SOCKET, SO_RCVTIMEO, 
            timeout_storage_type> receive_timeout;

        /** Specifies the total per-socket buffer space reserved for sends. */
        typedef socket_option<SOL_SOCKET, SO_SNDBUF, int> send_buffer;

        /** The timeout, in milliseconds, for blocking send calls. */
        typedef socket_option<SOL_SOCKET, SO_SNDTIMEO,
            timeout_storage_type> send_timeout;

        ///** Updates the accepting socket with the context of the listening socket.  */
        //typedef socket_option<SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, int> debug;

        ///**
        // * This object stores the configuration information for the service
        // * provider associated with socket s. The exact format of this data
        // * structure is service provider specific.
        // */
        //typedef socket_option<SOL_SOCKET, PVD_CONFIG, int> debug;

        /** Disables the Nagle algorithm for send coalescing. */
        typedef socket_option<IPPROTO_TCP, TCP_NODELAY,
            bool_storage_type> tcp_no_delay;

        ///**  */
        //typedef socket_option<NSPROTO_IPX, TCP_NODELAY,
        //    bool_storage_type> no_delay;

        ///**  */
        //typedef socket_option<NSPROTO_IPX, TCP_NODELAY,
        //    bool_storage_type> no_delay;

    protected:

        /** Super class typedef. */
        typedef the::not_instantiable base;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SOCKET_OPTIONS_H_INCLUDED */
