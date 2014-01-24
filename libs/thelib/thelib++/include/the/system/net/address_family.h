/*
 * include\the\system\net\address_family.h
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

#ifndef THE_SYSTEM_NET_ADDRESS_FAMILY_H_INCLUDED
#define THE_SYSTEM_NET_ADDRESS_FAMILY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if defined(THE_WINDOWS)
#include <WinSock2.h>
#elif defined(THE_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#endif  /* defined(THE_WINDOWS) */


namespace the {
namespace system {
namespace net {

    /**
     * Specifies the addressing scheme of e.g. a socket.
     */
    typedef enum address_family_t {

        /** Unspecified address. */
        unspecified = AF_UNSPEC,

        /** Unix local to host address. */
        unix = AF_UNIX,

        /** IP version 4 adress. */
        inter_network4 = AF_INET,

#if defined(THE_WINDOWS)
        /** ARPANET IMP address. */
        imp_link = AF_IMPLINK,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** PUP protocol address. */
        pup = AF_PUP,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** MIT CHAOS address. */
        chaos = AF_CHAOS,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** Xerox NS address. */
        ns = AF_NS,
#endif /* defined(THE_WINDOWS) */

        /** IPX or SPX address. */
        ipx = AF_IPX,

#if defined(THE_WINDOWS)
        /** Address for ISO protocols. */
        iso = AF_ISO,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** Address for OSI protocols. */
        osi = AF_OSI,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** ECMA address. */
        ecma = AF_ECMA,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** Data kit address. */
        data_kit = AF_DATAKIT,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** CCITT address (e.g. X.25). */
        ccitt = AF_CCITT,
#endif /* defined(THE_WINDOWS) */

        /** IBM SNA address. */
        sna = AF_SNA,

        /** DECnet address. */
        dec_net = AF_DECnet,

#if defined(THE_WINDOWS)
        /** Direct data-link interface address. */
        data_link = AF_DLI,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** LAT address. */
        lat = AF_LAT,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** NSC Hyperchannel address. */
        hyper_channel = AF_HYLINK,
#endif /* defined(THE_WINDOWS) */

        /** AppleTalk address. */
        apple_talk = AF_APPLETALK,

#if defined(THE_WINDOWS)
        /** NetBios address. */
        net_bios = AF_NETBIOS,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** VoiceView address. */
        voice_view = AF_VOICEVIEW,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /* FireFox address. */
        fire_fox = AF_FIREFOX,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** 
         * Unknown address (original documentation: "Somebody is using this!").
         */
        unknown1 = AF_UNKNOWN1,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** Banyan address. */
        banyan = AF_BAN,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** Native ATM services address. */
        atm = AF_ATM,
#endif /* defined(THE_WINDOWS) */

        /** IPv6 address. */
        inter_network6 = AF_INET6,

#if defined(THE_WINDOWS)
        /** Microsoft cluster product address. */
        cluster = AF_CLUSTER,
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
        /** IEEE 1284.4 workgroup address. */
        ieee12844 = AF_12844,
#endif /* defined(THE_WINDOWS) */

        /** IrDA address. */
        irda = AF_IRDA,

#if defined(THE_WINDOWS)
        /** Address for Network Designers OSI gateway-enabled protocols. */
        network_designers = AF_NETDES,
#endif /* defined(THE_WINDOWS) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0501))
        tcn_process = AF_TCNPROCESS,

        tcn_message = AF_TCNMESSAGE,

        iclfxbm = AF_ICLFXBM,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0501)) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0600))
        /** Bluetooth RFCOMM/L2CAP protocols */
        bth = AF_BTH,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0600)) */

#if ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0601))
        link = AF_LINK,
#endif /* ((defined(_WIN32_WINNT)) && (_WIN32_WINNT >= 0x0601)) */

        /** MAX address. */
        max = AF_MAX

    } address_family;

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_ADDRESS_FAMILY_H_INCLUDED */
