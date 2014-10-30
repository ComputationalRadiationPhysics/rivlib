/*
 * src\system\net\ip_end_point.cpp
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
 * IPEndPoint.cpp
 *
 * Copyright (C) 2009 by Christoph Müller. Alle Rechte vorbehalten.
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */


#include "the/system/net/ip_end_point.h"

#include "the/assert.h"
#include "the/argument_exception.h"
#include "the/argument_null_exception.h"
#include "the/format_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"

#include "the/text/string_builder.h"
#include "the/text/string_find_utility.h"
#include "the/text/string_parse_utility.h"

#include "the/system/net/debug.h"


/*
 * the::system::net::ip_end_point::parse
 */
the::system::net::ip_end_point the::system::net::ip_end_point::parse(
        const char *str,
        const the::system::net::address_family preferredFamily) {
    THE_STACK_TRACE;
    if (str == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "str");
    }

    /* Find the last colon, which is the start of the port number. */
    const char *startPort = ::strrchr(str, ':');

    /* If there is no colon, the port is missing, which is a fatal error. */
    if (startPort == nullptr) {
        THROW_THE_EXCEPTION(format_exception, "An IP end point address must "
            "contain a port which is separated from the IP address by a "
            "colon (\":\").");
    }

    /* Create separate string for IP address and port. */
    THE_ASSERT(startPort != nullptr);
    std::string addr(str, (startPort - str) * sizeof(char));
    std::string port(startPort + 1);    // Note: Skip the colon at begin!
    THE_NETWORKING_TRACE_DEBUG("Address part is \"%s\".\n", addr.c_str());
    THE_NETWORKING_TRACE_DEBUG("Port part is \"%s\".\n", port.c_str());

    if (text::string_find_utility::contains(addr, ':')) {
        /* IPv6. */
        size_t addrBegin = addr.find('[');
        size_t addrEnd = addr.find(']');

        if ((addrBegin == std::string::npos) 
                || (addrEnd == std::string::npos)) {
            THROW_THE_EXCEPTION(format_exception, "An IPv6 end point address "
                "string must encapsulate the IP address in brackets "
                "(\"[\", \"]\").");
        }

        addr.erase(addrEnd);
        addr.erase(0, addrBegin + 1);
        THE_NETWORKING_TRACE_DEBUG("Cleaned IPv6 address is \"%s\".\n",
            addr.c_str());
    }

    /* Actually parse the two parts. */
    ip_address::shared_pointer_type valAddr = ip_address::parse(addr,
        preferredFamily);
    int valPort = text::string_parse_utility::parse_int(port);

    /* Create the end point. */
    ip_end_point retval(valAddr, valPort);
    THE_NETWORKING_TRACE_DEBUG("Parsed end point is %s.\n",
        retval.to_astring().c_str());
    return retval;
}


 /*
 * the::system::net::ip_end_point::parse
 */
the::system::net::ip_end_point the::system::net::ip_end_point::parse(
        const wchar_t *str,
        const the::system::net::address_family preferredFamily) {
    THE_STACK_TRACE;
    return ip_end_point::parse(THE_W2A(str), preferredFamily);
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(void) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(const ip_address4& ipAddress,
        const port_type port) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
    this->set_ip_address(ipAddress);
    this->set_port(port);
}

/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(const ip_address6& ipAddress,
        const port_type port) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
    this->set_ip_address(ipAddress);
    this->set_port(port);
}

/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(
        const the::system::net::ip_address& ipAddress,
        const port_type port) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
    this->set_ip_address(ipAddress);
    this->set_port(port);
}

/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(
        const the::system::net::ip_address::shared_pointer_type& ipAddress,
        const port_type port) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
    this->set_ip_address(ipAddress);
    this->set_port(port);
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(
        const the::system::net::address_family addressFamily,
        const port_type port) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
    this->set_ip_address(ip_address::any(addressFamily));
    this->set_port(port);
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(const ip_end_point& address,
        const port_type newPort) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address);
    *this = address;
    this->set_port(newPort);
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(
        const native_address_type& address) : base() {
    THE_STACK_TRACE;
    *this = address;
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(
        const struct sockaddr_in& address) : base() {
    THE_STACK_TRACE;
    *this = address;
}


/*
 * the::system::net::ip_end_point::ip_end_point
 */
the::system::net::ip_end_point::ip_end_point(
        const struct sockaddr_in6& address) : base() {
    THE_STACK_TRACE;
    *this = address;
}


/*
 * the::system::net::ip_end_point::~ip_end_point
 */
the::system::net::ip_end_point::~ip_end_point(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_end_point::address_family
 */
the::system::net::address_family
the::system::net::ip_end_point::address_family(void) const {
    THE_STACK_TRACE;
    return static_cast<the::system::net::address_family>(
        this->address.ss_family);
}


/*
 * the::system::net::ip_end_point::compare
 */
int the::system::net::ip_end_point::compare(const end_point& rhs) const {
    THE_STACK_TRACE;
    size_t ls = this->size();
    size_t rs = rhs.size();
    const ip_end_point *r = dynamic_cast<const ip_end_point *>(&rhs);

    if (r != nullptr) {
        // If 'rhs' is not an ip_end_point, we assume that it is smaller and
        // reset the size to 0.
        rs = 0;
    }

    int retval = static_cast<int>(ls) - static_cast<int>(rs);
    if (retval == 0) {
        retval = ::memcmp(&this->address, &r->address, ls);
    }

    return retval;
}


/*
 * the::system::net::ip_end_point::address
 */
the::system::net::ip_address::shared_pointer_type
the::system::net::ip_end_point::ip_address(void) const {
    THE_STACK_TRACE;
    switch (this->address.ss_family) {
        case AF_INET:
            return ip_address::shared_pointer_type(
                new ip_address4(this->asV4().sin_addr));

        case AF_INET6:
            return ip_address::shared_pointer_type(
                new ip_address6(this->asV6().sin6_addr));

        default:
            THROW_THE_INVALID_OPERATION_EXCEPTION;
    }
}


/*
 * the::system::net::ip_end_point::port
 */
the::system::net::ip_end_point::port_type
the::system::net::ip_end_point::port(void) const {
    THE_STACK_TRACE;
    switch (this->address.ss_family) {
        case AF_INET:
            return the::system::net::ip_address::network_to_host_order(
                this->asV4().sin_port);

        case AF_INET6:
            return the::system::net::ip_address::network_to_host_order(
                this->asV6().sin6_port);

        default:
            THROW_THE_INVALID_OPERATION_EXCEPTION;
    }
}


/*
 * the::system::net::ip_end_point::set_ip_address
 */
void the::system::net::ip_end_point::set_ip_address(
        const ip_address4& ipAddress) {
    THE_STACK_TRACE;
    this->address.ss_family = AF_INET;
    ::memcpy(&(this->asV4().sin_addr),
        static_cast<const struct in_addr *>(ipAddress),
        sizeof(struct in_addr));
}


/*
 * the::system::net::ip_end_point::set_ip_address
 */
void the::system::net::ip_end_point::set_ip_address(
        const ip_address6& ipAddress) {
    THE_STACK_TRACE;
    this->address.ss_family = AF_INET6;
    ::memcpy(&(this->asV6().sin6_addr),
        static_cast<const struct in6_addr *>(ipAddress),
        sizeof(struct in6_addr));
}


/*
 * the::system::net::ip_end_point::set_ip_address
 */
void the::system::net::ip_end_point::set_ip_address(
        const the::system::net::ip_address& ipAddress) {
    THE_STACK_TRACE;
    switch (ipAddress.address_family()) {
        case the::system::net::address_family::inter_network4:
            this->set_ip_address(static_cast<const ip_address4&>(ipAddress));
            break;

        case the::system::net::address_family::inter_network6:
            // SetIPAddress produces deep copy. Cast to object does not work
            // on gcc, because this program is utter sh**.
            this->set_ip_address(static_cast<const ip_address6&>(ipAddress));
            break;

        default:
            THROW_THE_EXCEPTION(argument_exception, "ipAddress");
    }
}


/*
 * the::system::net::ip_end_point::set_ip_address
 */
void the::system::net::ip_end_point::set_ip_address(
        const ip_address::shared_pointer_type& ipAddress) {
    THE_STACK_TRACE;
    if (ipAddress == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "ipAddress");
    } else {
        this->set_ip_address(*ipAddress);
    }
}


/*
 * the::system::net::ip_end_point::set_port
 */
void the::system::net::ip_end_point::set_port(const port_type port) {
    THE_STACK_TRACE;
    switch (this->address.ss_family) {
        case AF_INET:
            this->asV4().sin_port = the::system::net::ip_address
                ::host_to_network_order(port);
            break;

        case AF_INET6:
            this->asV6().sin6_port = the::system::net::ip_address
                ::host_to_network_order(port);
            break;

        default:
            THROW_THE_INVALID_OPERATION_EXCEPTION;
    }
}


/*
 * the::system::net::ip_end_point::size
 */
the::system::net::ip_end_point::size_type the::system::net::ip_end_point::size(
        void) const {
    THE_STACK_TRACE;
    switch (this->address.ss_family) {
        case AF_INET:
            return sizeof(struct sockaddr_in);
            break;

        case AF_INET6:
            return sizeof(struct sockaddr_in6);
            break;

        default:
            return sizeof(native_address_type);
    }
}


/*
 * the::system::net::ip_end_point::to_astring
 */
the::astring the::system::net::ip_end_point::to_astring(void) const {
    THE_STACK_TRACE;
    astring retval;

    switch (this->address.ss_family) {
        case AF_INET:
            retval = text::astring_builder::format("%s:%u",
                this->ip_address()->to_astring().c_str(), this->port());
            break;

        case AF_INET6:
            retval = text::astring_builder::format("[%s]:%u",
                this->ip_address()->to_astring().c_str(), this->port());
            break;

        default:
            THROW_THE_INVALID_OPERATION_EXCEPTION;
    }

    return retval;
}


/*
 * the::system::net::ip_end_point::operator =
 */
the::system::net::ip_end_point& the::system::net::ip_end_point::operator =(
        const ip_end_point& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
        ::memcpy(&this->address, &rhs.address, sizeof(this->address));
    }
    return *this;
}


/*
 * the::system::net::ip_end_point::operator =
 */
the::system::net::ip_end_point& the::system::net::ip_end_point::operator =(
       const native_address_type& rhs) {
    THE_STACK_TRACE;
    if (&this->address != &rhs) {
        ::memcpy(&this->address, &rhs, sizeof(this->address));
    }
    return *this;
}



/*
 * the::system::net::ip_end_point::operator =
 */
the::system::net::ip_end_point& the::system::net::ip_end_point::operator =(
       const struct sockaddr_in& rhs) {
    THE_STACK_TRACE;
    if (static_cast<void *>(&this->address) 
            != static_cast<const void *>(&rhs)) {
        the::zero_memory(&this->address);
        ::memcpy(&this->address, &rhs, sizeof(sockaddr_in));
    }
    return *this;
}



/*
 * the::system::net::ip_end_point::operator =
 */
the::system::net::ip_end_point& the::system::net::ip_end_point::operator =(
       const struct sockaddr_in6& rhs) {
    THE_STACK_TRACE;
    if (static_cast<void *>(&this->address) 
            != static_cast<const void *>(&rhs)) {
        the::zero_memory(&this->address);
        ::memcpy(&this->address, &rhs, sizeof(sockaddr_in6));
    }
    return *this;
}
