/*
 * include\the\system\net\dns.h
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
 * DNS.h
 *
 * Copyright (C) 2009 by Christoph Müller. Alle Rechte vorbehalten.
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_NET_DNS_H_INCLUDED
#define THE_SYSTEM_NET_DNS_H_INCLUDED
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
#endif /* THE_WINDOWS */

#include "the/not_instantiable.h"
#include "the/stack_trace.h"
#include "the/string.h"

#include "the/system/net/ip_address4.h"
#include "the/system/net/ip_address6.h"
#include "the/system/net/ip_host_entry.h"


namespace the {
namespace system {
namespace net {

    /**
     * Provides name resolution via the Domain Name System.
     */
    class dns : public the::not_instantiable {

    public:

        /**
         * Answer the ip_host_entry for the specified host name or IP address
         * string.
         *
         * @param outEntry          Receives the host entry.
         * @param hostNameOrAddress The host name or stringised IP address to 
         *                          search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static aip_host_entry& get_host_entry(aip_host_entry& outEntry,
            const char *hostNameOrAddress);

        /**
         * Answer the ip_host_entry for the specified host name or IP address
         * string.
         *
         * @param outEntry          Receives the host entry.
         * @param hostNameOrAddress The host name or stringised IP address to 
         *                          search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        inline static aip_host_entry& get_host_entry(aip_host_entry& outEntry,
                const astring& hostNameOrAddress) {
            THE_STACK_TRACE;
            return dns::get_host_entry(outEntry, hostNameOrAddress.c_str());
        }

        /**
         * Answer the ip_host_entry for the specified host name or IP address
         * string.
         *
         * @param outEntry          Receives the host entry.
         * @param hostNameOrAddress The host name or stringised IP address to 
         *                          search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static wip_host_entry& get_host_entry(wip_host_entry& outEntry,
            const wchar_t *hostNameOrAddress);

        /**
         * Answer the ip_host_entry for the specified host name or IP address
         * string.
         *
         * @param outEntry          Receives the host entry.
         * @param hostNameOrAddress The host name or stringised IP address to 
         *                          search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        inline static wip_host_entry& get_host_entry(wip_host_entry& outEntry,
                const wstring& hostNameOrAddress) {
            THE_STACK_TRACE;
            return dns::get_host_entry(outEntry, hostNameOrAddress.c_str());
        }

        /**
         * Answer the ip_host_entry for the specified IP address.
         *
         * @param outEntry Receives the host entry.
         * @param address  One address of the host to search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static aip_host_entry& get_host_entry(aip_host_entry& outEntry,
            const ip_address& address);

        /**
         * Answer the ip_host_entry for the specified IP address.
         *
         * @param outEntry Receives the host entry.
         * @param address  One address of the host to search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static wip_host_entry& get_host_entry(wip_host_entry& outEntry,
            const ip_address& address);

        /**
         * Answer the ip_host_entry for the specified IP address.
         *
         * @param outEntry Receives the host entry.
         * @param address  One address of the host to search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static aip_host_entry& get_host_entry(aip_host_entry& outEntry,
            const ip_address4& address);

        /**
         * Answer the ip_host_entry for the specified IP address.
         *
         * @param outEntry Receives the host entry.
         * @param address  One address of the host to search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static wip_host_entry& get_host_entry(wip_host_entry& outEntry,
            const ip_address4& address);

        /**
         * Answer the ip_host_entry for the specified IP address.
         *
         * @param outEntry Receives the host entry.
         * @param address  One address of the host to search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static aip_host_entry& get_host_entry(aip_host_entry& outEntry,
            const ip_address6& address);

        /**
         * Answer the ip_host_entry for the specified IP address.
         *
         * @param outEntry Receives the host entry.
         * @param address  One address of the host to search.
         *
         * @return 'outEntry'.
         *
         * @throws socket_exception      In case the operation fails, e.g. the
         *                               host could not be found.
         */
        static wip_host_entry& get_host_entry(wip_host_entry& outEntry,
            const ip_address6& address);

    private:

        /**
         * Retrieve the addrinfo list for the specified host name of IP address
         * string.
         *
         * The caller takes the ownership of the returned list and must release
         * it using the appropriate API function.
         *
         * @param hostNameOrAddress The host name or stringised IP address to 
         *                          search.
         * @param addressFamily     The address family to limit the search to.
         *
         * @returns The addrinfo list if the host was found. The caller must 
         *          free this list.
         *
         * @throws SocketException In case the operation fails, e.g. the host 
         *                         could not be found.
         */
        static struct addrinfo *getAddrInfo(const char *hostNameOrAddress,
            const int addressFamily);

        /**
         * Retrieve the addrinfo list for the specified host name of IP address
         * string.
         *
         * The caller takes the ownership of the returned list and must release
         * it using the appropriate API function.
         *
         * @param hostNameOrAddress The host name or stringised IP address to 
         *                          search.
         * @param addressFamily     The address family to limit the search to.
         *
         * @returns The addrinfo list if the host was found. The caller must 
         *          free this list.
         *
         * @throws SocketException In case the operation fails, e.g. the host 
         *                         could not be found.
         */
#if (defined(_WIN32) && defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502))
        static ADDRINFOW *getAddrInfo(const wchar_t *hostNameOrAddress,
            const int addressFamily);
#else /* (defined(_WIN32) && defined(_WIN32_WINNT) && ... */
        static struct addrinfo *getAddrInfo(const wchar_t *hostNameOrAddress,
            const int addressFamily);
#endif /* (defined(_WIN32) && defined(_WIN32_WINNT) && ... */

        /** Dtor. */
        ~dns(void);

        /** Super class typedef. */
        typedef the::not_instantiable base;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_DNS_H_INCLUDED */
