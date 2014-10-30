/*
 * src\system\net\ip_address6.cpp
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

#include "the/system/net/ip_address6.h"

#if defined(THE_WINDOWS)
#include <ws2ipdef.h>
#elif defined(THE_LINUX)
#include <arpa/inet.h>
#include <netdb.h>
#endif /* defined(THE_WINDOWS) */

#include <cstdlib>

#include "the/argument_null_exception.h"
#include "the/format_exception.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/net/debug.h"



/*
 * the::system::net::ip_address6::from_hexadecimal_string
 */
the::system::net::ip_address6
the::system::net::ip_address6::from_hexadecimal_string(const char *address) {
    THE_STACK_TRACE;
    ip_address6 retval;

    if (address == NULL) {
        throw argument_null_exception("address", __FILE__, __LINE__);
    }

    if (::inet_pton(AF_INET6, address, &retval.address) != 1) {
        throw format_exception("The specified string does not represent a "
            "valid IP address.", __FILE__, __LINE__);
    }

    return retval;
}


/*
 * the::system::net::ip_address6::from_hexadecimal_string
 */
the::system::net::ip_address6
the::system::net::ip_address6::from_hexadecimal_string(const wchar_t *address) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    ip_address6 retval;

    if (address == NULL) {
        throw argument_null_exception("address", __FILE__, __LINE__);
    }

    if (::InetPtonW(AF_INET6, address, &retval.address) != 1) {
        throw format_exception("The specified string does not represent a "
            "valid IP address.", __FILE__, __LINE__);
    }

    return retval;

#else /* defined(THE_WINDOWS) */
    return ip_address6::from_hexadecimal_string(THE_W2A(address));
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::net::ip_address6::all_nodes_on_line
 */
const the::system::net::ip_address6 
the::system::net::ip_address6::all_nodes_on_link
#ifdef THE_WINDOWS
    (::in6addr_allnodesonlink);
#else /* THE_WINDOWS */
    = the::system::net::ip_address6::from_hexadecimal_string("ff02::1");
#endif /* THE_WINDOWS */


/*
 * the::system::net::ip_address6::all_routers_on_link
 */
const the::system::net::ip_address6 
the::system::net::ip_address6::all_routers_on_link
#ifdef THE_WINDOWS
    (::in6addr_allroutersonlink);
#else /* THE_WINDOWS */
    = the::system::net::ip_address6::from_hexadecimal_string("ff02::2");
#endif /* THE_WINDOWS */


/*
 * the::system::net::ip_address6::all_nodes_on_nodes
 */
const the::system::net::ip_address6
the::system::net::ip_address6::all_nodes_on_nodes
#ifdef THE_WINDOWS
    (::in6addr_allnodesonnode);
#else /* THE_WINDOWS */
    = the::system::net::ip_address6::from_hexadecimal_string("ff01::1");
#endif /* THE_WINDOWS */


/*
 * the::system::net::ip_address6::any
 */
const the::system::net::ip_address6 the::system::net::ip_address6::any(
    ::in6addr_any);


/*
 * the::system::net::ip_address6::loopback
 */
const the::system::net::ip_address6 the::system::net::ip_address6::loopback(
    ::in6addr_loopback);


//const the::system::net::ip_address6 the::system::net::ip_address6::multicast_all_site_local_hosts;
//
//const the::system::net::ip_address6 the::system::net::ip_address6::multicast_all_site_local_routers;

/*
 * the::system::net::ip_address6::ip_address6
 */
the::system::net::ip_address6::ip_address6(void) : base()  {
    THE_STACK_TRACE;
    // TODO: The following assertion explodes under Linux because of static 
    // init order.
    //THE_ASSERT(this->is_any());
}


/*
 * the::system::net::ip_address6::ip_address6
 */
the::system::net::ip_address6::ip_address6(const ip_address6& rhs) 
        : base(rhs)  {
    THE_STACK_TRACE;
    THE_ASSERT(this->operator ==(rhs));
}


/*
 * the::system::net::ip_address6::ip_address6
 */
the::system::net::ip_address6::ip_address6(const native_address_type& address)
        : base(address) {
    THE_STACK_TRACE;
    THE_ASSERT(::memcmp(&this->address, &address, sizeof(this->address)) == 0);
}


/*
 * the::system::net::ip_address6::ip_address6
 */
the::system::net::ip_address6::ip_address6(
        const byte b1, const byte b2, const byte b3, const byte b4,
        const byte b5, const byte b6, const byte b7, const byte b8,
        const byte b9, const byte b10, const byte b11, const byte b12,
        const byte b13, const byte b14, const byte b15, const byte b16) {
    THE_STACK_TRACE;
#define THE_IPV6_INIT_BYTE(n) this->address.s6_addr[n - 1] = b##n
    THE_IPV6_INIT_BYTE(1);
    THE_IPV6_INIT_BYTE(2);
    THE_IPV6_INIT_BYTE(3);
    THE_IPV6_INIT_BYTE(4);
    THE_IPV6_INIT_BYTE(5);
    THE_IPV6_INIT_BYTE(6);
    THE_IPV6_INIT_BYTE(7);
    THE_IPV6_INIT_BYTE(8);
    THE_IPV6_INIT_BYTE(9);
    THE_IPV6_INIT_BYTE(10);
    THE_IPV6_INIT_BYTE(11);
    THE_IPV6_INIT_BYTE(12);
    THE_IPV6_INIT_BYTE(13);
    THE_IPV6_INIT_BYTE(14);
    THE_IPV6_INIT_BYTE(15);
    THE_IPV6_INIT_BYTE(16);
#undef THE_IPV6_INIT_BYTE
}


/*
 * the::system::net::ip_address6::~ip_address6
 */
the::system::net::ip_address6::~ip_address6(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_address6::to_astring
 */
the::astring the::system::net::ip_address6::to_astring(void) const {
    THE_STACK_TRACE;

    struct sockaddr_in6 addr;   // Dummy socket address used for lookup.
    char buffer[NI_MAXHOST];    // Receives the stringised address.
    int err = 0;                // OS operation return value.

    the::zero_memory(&addr, sizeof(addr));
    addr.sin6_family = AF_INET6;
    ::memcpy(&addr.sin6_addr, &this->address, sizeof(struct in6_addr));

    if ((err = ::getnameinfo(reinterpret_cast<struct sockaddr *>(&addr),
            sizeof(struct sockaddr_in6), buffer, sizeof(buffer), NULL, 0,
            NI_NUMERICHOST)) != 0) {
#if defined(THE_WINDOWS)
#define GAI_ERROR ::gai_strerrorA(err)
#else /* defined(THE_WINDOWS) */
#define GAI_ERROR ::gai_strerror(err)
#endif /* defined(THE_WINDOWS) */
        THE_NETWORKING_TRACE_ERROR("::getnameinfo failed in %s: %s\n",
            THE_FUNCTION, GAI_ERROR);
#undef GAI_ERROR
        buffer[0] = 0;
    }

    return buffer;
}

