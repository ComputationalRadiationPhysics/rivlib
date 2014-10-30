/*
 * src\system\net\ip_address4.cpp
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

#include "the/system/net/ip_address4.h"

#if defined(THE_WINDOWS)
#include <ws2ipdef.h>
#elif defined(THE_LINUX)
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#endif /* defined(THE_WINDOWS) */

#include "the/argument_null_exception.h"
#include "the/format_exception.h"

#include "the/text/string_converter.h"


/*
 * the::system::net::ip_address4::from_dotted_string
 */
the::system::net::ip_address4
the::system::net::ip_address4::from_dotted_string(const char *address) {
    THE_STACK_TRACE;
    ip_address4 retval;

    if (address == NULL) {
        throw argument_null_exception("address", __FILE__, __LINE__);
    }

    if ((retval.address.s_addr = ::inet_addr(address)) == INADDR_NONE) {
        throw format_exception("The specified string does not represent a "
            "valid IP address.", __FILE__, __LINE__);
    }

    return retval;
}


/*
 * the::system::net::ip_address4::from_dotted_string
 */
the::system::net::ip_address4 
the::system::net::ip_address4::from_dotted_string(const wchar_t *address) {
    THE_STACK_TRACE;
    return ip_address4::from_dotted_string(THE_W2A(address));
}


/*
 * the::system::net::ip_address4::all_nodes_on_link
 */
const the::system::net::ip_address4
the::system::net::ip_address4::all_nodes_on_link
#ifdef _WIN32
    (::in4addr_allnodesonlink);
#else /* _WIN32 */
    = the::system::net::ip_address4::from_dotted_string("224.0.0.1");
#endif /* _WIN32 */


/*
 * the::system::net::ip_address4::all_routers_on_link
 */
const the::system::net::ip_address4 
the::system::net::ip_address4::all_routers_on_link
#ifdef THE_WINDOWS
    (::in4addr_allroutersonlink);
#else /* THE_WINDOWS */
    = the::system::net::ip_address4::from_dotted_string("224.0.0.2");
#endif /* THE_WINDOWS */


/*
 * the::system::net::ip_address4::any
 */
const the::system::net::ip_address4 the::system::net::ip_address4::any(
    static_cast<uint32_t>(INADDR_ANY));


/*
 * the::system::net::ip_address4::broadcast
 */
const the::system::net::ip_address4 the::system::net::ip_address4::broadcast(
    static_cast<uint32_t>(INADDR_BROADCAST));


/*
 * the::system::net::ip_address4::loopback
 */
const the::system::net::ip_address4 the::system::net::ip_address4::loopback(
    static_cast<uint32_t>(INADDR_LOOPBACK));


/*
 * the::system::net::ip_address4::none
 */
const the::system::net::ip_address4 the::system::net::ip_address4::none(
    static_cast<uint32_t>(INADDR_NONE));


/*
 * the::system::net::ip_address4::ip_address4
 */
the::system::net::ip_address4::ip_address4(void) : base()  {
    THE_STACK_TRACE;
    // TODO: The following assertion explodes under Linux because of static 
    // init order.
    //THE_ASSERT(this->is_any());
}


/*
 * the::system::net::ip_address4::ip_address4
 */
the::system::net::ip_address4::ip_address4(const ip_address4& rhs) 
        : base(rhs)  {
    THE_STACK_TRACE;
    THE_ASSERT(this->operator ==(rhs));
}


/*
 * the::system::net::ip_address4::ip_address4
 */
the::system::net::ip_address4::ip_address4(const native_address_type& address)
        : base(address) {
    THE_STACK_TRACE;
    THE_ASSERT(this->address.s_addr == address.s_addr);
}


/*
 * the::system::net::ip_address4::ip_address4
 */
the::system::net::ip_address4::ip_address4(const byte b1, const byte b2, 
        const byte b3, const byte b4) : base() {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    this->address.S_un.S_un_b.s_b1 = b1;
    this->address.S_un.S_un_b.s_b2 = b2;
    this->address.S_un.S_un_b.s_b3 = b3;
    this->address.S_un.S_un_b.s_b4 = b4;

#else /* defined(THE_WINDOWS) */
    this->address.s_addr
        = (static_cast<unsigned int>(b1))
        + (static_cast<unsigned int>(b2) << 8)
        + (static_cast<unsigned int>(b3) << 16)
        + (static_cast<unsigned int>(b4) << 24);
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::net::ip_address4::ip_address4
 */
the::system::net::ip_address4::ip_address4(const uint32_t address, 
        const bool isHostByteOrder) {
    THE_STACK_TRACE;
    this->address.s_addr = isHostByteOrder 
        ? ip_address4::host_to_network_order(address) 
        : address;
}


/*
 * the::system::net::ip_address4::~ip_address4
 */
the::system::net::ip_address4::~ip_address4(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_address4::to_astring
 */
the::astring the::system::net::ip_address4::to_astring(void) const {
    THE_STACK_TRACE;
    astring retval(::inet_ntoa(this->address));
    return retval;
}
