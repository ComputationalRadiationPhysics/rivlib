/*
 * include\the\system\net\shutdown_manifest.h
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

#ifndef THE_SYSTEM_NET_SHUTDOWN_MANIFEST_H_INCLUDED
#define THE_SYSTEM_NET_SHUTDOWN_MANIFEST_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <WinSock2.h>
#elif THE_LINUX
#include <sys/socket.h>
#endif /* THE_WINDOWS */


namespace the {
namespace system {
namespace net {

    /**
     * Flag that describes what types of operation will no longer be allowed 
     * when calling e.g. the::system::net::socket::shutdown.
     */
    typedef enum shutdown_manifest_t {

        /**
         * If this parameter is passed to shutdown, the behaviour is like
         * a combination of shutdown_manifest::receive and
         * shutdown_manifest::send.
         */
        both
#ifdef THE_WINDOWS
            = SD_BOTH,
#else /* THE_WINDOWS */
            = SHUT_RDWR,
#endif /* THE_WINDOWS */

        /**
         * If this parameter is passed to shutdown, the socket will disallow
         * subsequent calls to receive. This has no effect on the lower
         * protocol layers. For TCP sockets, if there is still data queued on
         * the socket waiting to be received, or data arrives subsequently,
         * the connection is reset, since the data cannot be delivered to the
         * user. For UDP sockets, incoming datagrams are accepted and queued.
         * In no case will an ICMP error packet be generated.
         */
        receive
#ifdef THE_WINDOWS
            = SD_RECEIVE,
#else /* THE_WINDOWS */
            = SHUT_RD,
#endif /* THE_WINDOWS */

        /**
         * If this parameter is passed to shutdown, the socket will disallow
         * subsequent calls to send. TCP sockets will send a FIN after all data
         * in the buffer have been sent and acknowledged by the receiver.
         */
        send
#ifdef THE_WINDOWS
            = SD_SEND,
#else /* THE_WINDOWS */
            = SHUT_WR,
#endif /* THE_WINDOWS */

        } shutdown_manifest;

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SHUTDOWN_MANIFEST_H_INCLUDED */
