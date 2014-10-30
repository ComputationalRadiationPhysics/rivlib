/*
 * include\the\system\net\protocol.h
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

#ifndef THE_SYSTEM_NET_PROTOCOL_H_INCLUDED
#define THE_SYSTEM_NET_PROTOCOL_H_INCLUDED
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


namespace the {
namespace system {
namespace net {

    /** 
     * The supported network protocols.
     */
    typedef enum protocol_t {

        /** IP version 4. */
        ip = IPPROTO_IP,

        /** IPv6 hop-by-hop options. */
        ip_v6_hop_by_hop_options = IPPROTO_HOPOPTS,

        /** Control message protocol. */
        icmp = IPPROTO_ICMP,

        /** Internet group mgmnt. */
        igmp = IPPROTO_IGMP,

        //PROTOCOL_GGP = IPPROTO_GGP,             // Gateway^2 (deprecated).

#ifdef THE_WINDOWS
        /** IPv4. */
        ip_v4 = IPPROTO_IPV4,
#endif /* THE_WINDOWS */

        /** TCP. */
        tcp = IPPROTO_TCP,

        /** PUP. */
        parc_universal_packet = IPPROTO_PUP,

        /** UDP. */
        udp = IPPROTO_UDP,

        /* XNS IDP. */
        idp = IPPROTO_IDP,

        /** IPv6. */
        ip_v6 = IPPROTO_IPV6,

        /** IPv6 routing header. */
        ip_v6_routing_header = IPPROTO_ROUTING,

        /** IPv6 fragmentation header. */
        ip_v6_fragment_header = IPPROTO_FRAGMENT,

        /** IPsec ESP header. */
        ipsec_esp = IPPROTO_ESP,

        /** IPsec AH. */
        ipsec_ah = IPPROTO_AH,

        /** ICMPv6. */
        icmp_v6 = IPPROTO_ICMPV6,

        /** IPv6 no next header. */
        none = IPPROTO_NONE,

        /** IPv6 destination options. */
        ip_v6_destination_options = IPPROTO_DSTOPTS,

        //PROTOCOL_ND = IPPROTO_ND,                 //!< UNOFFICIAL net disk prot.
        //PROTOCOL_ICLFXBM = IPPROTO_ICLFXBM,

        /** Raw IP packet. */
        raw_packet = IPPROTO_RAW
    } protocol;

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_PROTOCOL_H_INCLUDED */
