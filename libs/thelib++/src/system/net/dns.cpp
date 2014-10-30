/*
 * src\system\net\dns.cpp
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
 * DNS.cpp
 *
 * Copyright (C) 2009 by Christoph Müller. Alle Rechte vorbehalten.
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#include "the/system/net/dns.h"

#ifdef _WIN32
#include <ws2tcpip.h>
#endif /* _WIN32 */

#include "the/assert.h"
#include "the/tchar.h"

#include "the/system/net/socket_exception.h"


// This is a little hack for making the implementation more readable.
// The macros are #undef'ed at the end of this file.
#if !(defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502))
#define ADDRINFOW struct addrinfo
#define FreeAddrInfoW freeaddrinfo
#endif /* !(defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502)) */


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::aip_host_entry&
the::system::net::dns::get_host_entry(aip_host_entry& outEntry,
            const char *hostNameOrAddress) {
    THE_STACK_TRACE;
    struct addrinfo *entries = dns::getAddrInfo(hostNameOrAddress, AF_UNSPEC);

    try {
        THE_ASSERT(entries != NULL);
        outEntry = entries;
        ::freeaddrinfo(entries);
        return outEntry;
    } catch (...) {
        ::freeaddrinfo(entries);
        throw;
    }
}


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::wip_host_entry&
the::system::net::dns::get_host_entry(wip_host_entry& outEntry,
            const wchar_t *hostNameOrAddress) {
    THE_STACK_TRACE;
    ADDRINFOW *entries = dns::getAddrInfo(hostNameOrAddress, AF_UNSPEC);

    try {
        THE_ASSERT(entries != NULL);
        outEntry = entries;
        ::FreeAddrInfoW(entries);
        return outEntry;
    } catch (...) {
        ::FreeAddrInfoW(entries);
        throw;
    }
}


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::aip_host_entry&
the::system::net::dns::get_host_entry(aip_host_entry& outEntry,
        const ip_address& address) {
    THE_STACK_TRACE;
    return dns::get_host_entry(outEntry, address.to_astring().c_str());
}


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::wip_host_entry&
the::system::net::dns::get_host_entry(wip_host_entry& outEntry,
        const ip_address& address) {
    THE_STACK_TRACE;
    return dns::get_host_entry(outEntry, address.to_wstring().c_str());
}


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::aip_host_entry&
the::system::net::dns::get_host_entry(aip_host_entry& outEntry,
        const ip_address4& address) {
    THE_STACK_TRACE;
    return dns::get_host_entry(outEntry, address.to_astring().c_str());
}


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::wip_host_entry&
the::system::net::dns::get_host_entry(wip_host_entry& outEntry,
        const ip_address4& address) {
    THE_STACK_TRACE;
    return dns::get_host_entry(outEntry, address.to_wstring().c_str());
}


/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::aip_host_entry&
the::system::net::dns::get_host_entry(aip_host_entry& outEntry,
        const ip_address6& address) {
    THE_STACK_TRACE;
    return dns::get_host_entry(outEntry, address.to_astring().c_str());
}

/*
 * the::system::net::dns::get_host_entry
 */
the::system::net::wip_host_entry&
the::system::net::dns::get_host_entry(wip_host_entry& outEntry,
        const ip_address6& address) {
    THE_STACK_TRACE;
    return dns::get_host_entry(outEntry, address.to_wstring().c_str());
}


/*
 * the::system::net::dns::~dns
 */
the::system::net::dns::~dns(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::dns::getAddrInfo
 */
struct addrinfo *the::system::net::dns::getAddrInfo(const char *hostNameOrAddress,
        const int addressFamily) {
    THE_STACK_TRACE;

    struct addrinfo *retval = NULL;     // Receives the address infos.
    struct addrinfo hints;              // The hints about the info we want.
    int err = 0;                        // Receives lookup error codes.

    /*
     * Set the lookup hints:
     * - The input string is either a host name or a human readable IP address.
     * - Return the addresses for any protocol family.
     */
    the::zero_memory(&hints);
    hints.ai_flags = 0; //AI_CANONNAME | AI_NUMERICHOST;
    hints.ai_family = addressFamily;

    if ((err = ::getaddrinfo(hostNameOrAddress, NULL, &hints, &retval)) != 0) {
        ASSERT(retval == NULL);
#ifdef _WIN32
        throw socket_exception(__FILE__, __LINE__);
#else /* _WIN32 */
        throw socket_exception(err, ::gai_strerror(err), __FILE__, __LINE__);
#endif /* _WIN32 */
    }

    return retval;
}


/*
 * the::system::net::dns::getAddrInfo
 */
ADDRINFOW *the::system::net::dns::getAddrInfo(const wchar_t *hostNameOrAddress,
        const int addressFamily) {
    THE_STACK_TRACE;

    ADDRINFOW *retval = NULL;           // Receives the address infos.
    ADDRINFOW hints;                    // The hints about the info we want.
    int err = 0;                        // Receives lookup error codes.

    /*
     * Set the lookup hints:
     * - The input string is either a host name or a human readable IP address.
     * - Return the addresses for any protocol family.
     */
    the::zero_memory(&hints);
    hints.ai_flags = 0; //AI_CANONNAME | AI_NUMERICHOST;
    hints.ai_family = addressFamily;

#if (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502))
    if (::GetAddrInfoW(hostNameOrAddress, NULL, &hints, &retval) != 0) {
#else /* (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502)) */
    if (::getaddrinfo(THE_W2A(hostNameOrAddress), NULL, &hints, &retval) != 0) {
#endif /* (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502)) */
        ASSERT(retval == NULL);
#ifdef _WIN32
        throw socket_exception(__FILE__, __LINE__);
#else /* _WIN32 */
        throw socket_exception(err, ::gai_strerror(err), __FILE__, __LINE__);
#endif /* _WIN32 */
    }

    return retval;
}


#if !(defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502))
#undef ADDRINFOW
#undef FreeAddrInfoW
#endif /* !(defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0502)) */
