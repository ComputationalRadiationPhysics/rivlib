/*
 * include\the\system\net\ip_end_point.h
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
 * IPEndPoint.h
 *
 * Copyright (C) 2009 by Christoph Müller. Alle Rechte vorbehalten.
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_NET_IP_END_POINT_H_INCLUDED
#define THE_SYSTEM_NET_IP_END_POINT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/stack_trace.h"

#include "the/system/net/ip_address.h"
#include "the/system/net/ip_address4.h"
#include "the/system/net/ip_address6.h"
#include "the/system/net/end_point.h"


namespace the {
namespace system {
namespace net {

    /**
     * The IPEndPoint class represents a socket address that is IP-agnostic,
     * i.e. can be used with both, IPv4 and IPv6. It supersedes the
     * SocketAddress class of VISlib, which supports only IPv4. In contrast to
     * SocketAddress, IPEndPoint only supports the address families AF_INET and
     * AF_INET6.
     */
    class ip_end_point : public end_point {

    public:

        /** The native type used for storing an end point address. */
        typedef struct sockaddr_storage native_address_type;

        /** The type used to specify a port number. */
        typedef unsigned short port_type;

        /**
         * Parse an IP end point from the given string.
         *
         * @param str             The IP end point. This must be a valid IP 
         *                        address or host name followed by a color (":")
         *                        and a port number.
         * @param preferredFamily Determines which address family is used if 
         *                        the family cannot be determined from 'str', 
         *                        e.g. because a host  name which is resolved 
         *                        to multiple addresses was specified. This 
         *                        parameter defaults to 
         *                        address_family::inter_network6.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'str' does not designate a
         *                               valid end point address.
         * @throws the::argument_null_exception If 'str' is NULL.
         */
        static ip_end_point parse(const char *str,
            const the::system::net::address_family preferredFamily
            = the::system::net::address_family::inter_network6);

        /**
         * Parse an IP end point from the given string.
         *
         * @param str             The IP end point. This must be a valid IP 
         *                        address or host name followed by a color (":")
         *                        and a port number.
         * @param preferredFamily Determines which address family is used if 
         *                        the family cannot be determined from 'str', 
         *                        e.g. because a host  name which is resolved 
         *                        to multiple addresses was specified. This 
         *                        parameter defaults to 
         *                        address_family::inter_network6.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'str' does not designate a
         *                               valid end point address.
         * @throws the::argument_null_exception If 'str' is NULL.
         */
        static ip_end_point parse(const wchar_t *str,
            const the::system::net::address_family preferredFamily
            = the::system::net::address_family::inter_network6);

        /**
         * Parse an IP end point from the given string.
         *
         * @param str             The IP end point. This must be a valid IP 
         *                        address or host name followed by a color (":")
         *                        and a port number.
         * @param preferredFamily Determines which address family is used if 
         *                        the family cannot be determined from 'str', 
         *                        e.g. because a host  name which is resolved 
         *                        to multiple addresses was specified. This 
         *                        parameter defaults to 
         *                        address_family::inter_network6.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'str' does not designate a
         *                               valid end point address.
         * @throws the::argument_null_exception If 'str' is NULL.
         */
        static inline ip_end_point parse(const the::astring& str,
                const the::system::net::address_family preferredFamily
                = the::system::net::address_family::inter_network6) {
            THE_STACK_TRACE;
            return ip_end_point::parse(str.c_str(), preferredFamily);
        }

        /**
         * Parse an IP end point from the given string.
         *
         * @param str             The IP end point. This must be a valid IP 
         *                        address or host name followed by a color (":")
         *                        and a port number.
         * @param preferredFamily Determines which address family is used if 
         *                        the family cannot be determined from 'str', 
         *                        e.g. because a host  name which is resolved 
         *                        to multiple addresses was specified. This 
         *                        parameter defaults to 
         *                        address_family::inter_network6.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'str' does not designate a
         *                               valid end point address.
         * @throws the::argument_null_exception If 'str' is NULL.
         */
        static inline ip_end_point parse(const the::wstring& str,
                const the::system::net::address_family preferredFamily
                = the::system::net::address_family::inter_network6) {
            THE_STACK_TRACE;
            return ip_end_point::parse(str.c_str(), preferredFamily);
        }

        /**
         * Initialises a new instance.
         */
        ip_end_point(void);

        /**
         * Creates a new IPv4 end point using the specified address and port.
         *
         * @param ipAddress The IP address of the end point.
         * @param port      The port number of the end point.
         */
        ip_end_point(const ip_address4& ipAddress, const port_type port);

        /**
         * Creates a new IPv6 end point using the specified address and port.
         *
         * @param ipAddress The IP address of the end point.
         * @param port      The port number of the end point.
         */
        ip_end_point(const ip_address6& ipAddress, const port_type port);

        /**
         * Creates a new end point using the specified address and port.
         *
         * @param ipAddress The IP address of the end point.
         * @param port      The port number of the end point. 
         */
        ip_end_point(const the::system::net::ip_address& ipAddress,
            const port_type port);

        /**
         * Creates a new end point using the specified address and port.
         *
         * @param ipAddress The IP address of the end point.
         * @param port      The port number of the end point. 
         */
        ip_end_point(
            const the::system::net::ip_address::shared_pointer_type& ipAddress,
            const port_type port);

        /**
         * Create a new unspecified end point (ANY) for the given address 
         * family, which must be one of FAMILY_INET or FAMILY_INET6, and the
         * given port.
         *
         * @param addressFamily The address family to create the end point for.
         * @param port          The port number of the end point.
         *
         * @throw the::argument_exception If 'addressFamily' is not one of the
         *                                supported families.
         */
        ip_end_point(const the::system::net::address_family addressFamily,
            const port_type port);

        /**
         * Create a copy of 'address' but change the port to 'newPort'.
         *
         * @param address The address to be cloned.
         * @param newPort The new port.
         */
        ip_end_point(const ip_end_point& address, const port_type newPort);

        /**
         * Create an IPEndPoint from an OS IP-agnostic address structure.
         *
         * This constructor enables implicit casts from native_address_type.
         *
         * @param address The address storage.
         */
        ip_end_point(const native_address_type& address);

        /**
         * Create an IPEndPoint from an OS IPv4 structure.
         *
         * This constructor enables implicit casts from sockaddr_in.
         *
         * @param address The IPv4 address to set.
         */
        ip_end_point(const struct sockaddr_in& address);

        /**
         * Create an IPEndPoint from an OS IPv6 structure.
         *
         * This constructor enables implicit casts from sockaddr_in6.
         *
         * @param address The IPv6 address to set.
         */
        ip_end_point(const struct sockaddr_in6& address);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline ip_end_point(const ip_end_point& rhs) : base(rhs) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /** Dtor. */
        virtual ~ip_end_point(void);

        /**
         * Gets the address family of the end point.
         *
         * @return The address family.
         */
        virtual the::system::net::address_family address_family(
            void) const;

        /**
         * Compares this object to 'rhs'.
         *
         * @return A negative number if this object is less than 'rhs', 
         *         zero if this object is equal to 'rhs',
         *         a positive number if this object is larger thatn 'rhs'.
         */
        virtual int compare(const end_point& rhs) const;

        /**
         * Answer the IP address of the IP end point.
         *
         * @return The IP address of the end point.
         *
         * @throws the::invalid_operation_exception If the address family is 
         *                                          illegal.
         */
        net::ip_address::shared_pointer_type ip_address(void) const;

        /**
         * Answer the port of the IP end point.
         *
         * @return The port of the IP end point. This will be in host byte 
         *         order.
         *
         * @throws the::invalid_operation_exception If the address family is 
         *                                          illegal.
         */
        port_type port(void) const;

        /**
         * Set a new IPv4 address. This will also change the address family.
         *
         * @param ipAddress The new IP address.
         */
        void set_ip_address(const ip_address4& ipAddress);

        /**
         * Set a new IPv6 address. This will also change the address family.
         *
         * @param ipAddress The new IP address.
         */
        void set_ip_address(const ip_address6& ipAddress);

        /**
         * Set a new IP address. This will also change the address family.
         *
         * @param ipAddress The new IP address.
         *
         * @throws the::argument_exception If the address family of 'ipAddress'
         *                                 is illegal.
         */
        void set_ip_address(const the::system::net::ip_address& ipAddress);

        /**
         * Set a new IP address. This will also change the address family.
         *
         * @param ipAddress The new IP address.
         *
         * @throws the::argument_exception If the address family of 'ipAddress'
         *                                 is illegal.
         * @throws the::argument_null_exception If 'ipAddress' is nullptr.
         */
        void set_ip_address(
            const the::system::net::ip_address::shared_pointer_type& ipAddress);

        /**
         * Set a new port number.
         *
         * @param port The new port number. The port number must be specified in
         *             host byte order.
         */
        void set_port(const port_type port);

        /**
         * Answer the size of the native end point address in bytes.
         *
         * @return The size of the address.
         */
        size_type size(void) const;

        /**
         * Convert the network address address to an ANSI string.
         *
         * @return The string representation of the IP address.
         */
        virtual astring to_astring(void) const;

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        ip_end_point& operator =(const ip_end_point& rhs);

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        ip_end_point& operator =(const native_address_type& rhs);

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        ip_end_point& operator =(const struct sockaddr_in& rhs);

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        ip_end_point& operator =(const struct sockaddr_in6& rhs);

        /**
         * Access the internal storage member as generic sockaddr pointer.
         *
         * @return Pointer to the sockaddr_storage used for the end point.
         */
        inline operator const struct sockaddr *const(void) const {
            return reinterpret_cast<const sockaddr *>(&this->address);
        }

        /**
         * Access the internal storage member as generic sockaddr pointer.
         *
         * @return Pointer to the sockaddr_storage used for the end point.
         */
        inline operator struct sockaddr *(void) {
            return reinterpret_cast<sockaddr *>(&this->address);
        }

        /**
         * Access the internal storage member.
         *
         * @return Reference to the sockaddr_storage used for the end point.
         */
        inline operator const native_address_type&(void) const {
            return this->address;
        }

        /**
         * Access the internal storage member.
         *
         * @return Reference to the sockaddr_storage used for the end point.
         */
        inline operator native_address_type&(void) {
            return this->address;
        }

        /**
         * Access the internal storage member.
         *
         * @return Pointer to the sockaddr_storage used for the end point.
         */
        inline operator const native_address_type *const(void) const {
            return &this->address;
        }

        /**
         * Access the internal storage member.
         *
         * @return Pointer to the sockaddr_storage used for the end point.
         */
        inline operator native_address_type *(void) {
            return &this->address;
        }

    protected:

        /** Super class typedef. */
        typedef the::system::net::end_point base;

        /**
         * Access the address storage as IPv4 socket address.
         *
         * @return Reference to the address storage reinterpreted for IPv4.
         */
        inline struct sockaddr_in& asV4(void) {
            return reinterpret_cast<struct sockaddr_in&>(this->address);
        }

        /**
         * Access the address storage as IPv4 socket address.
         *
         * @return Reference to the address storage reinterpreted for IPv4.
         */
        inline const struct sockaddr_in& asV4(void) const {
            return reinterpret_cast<const struct sockaddr_in&>(this->address);
        }

        /**
         * Access the address storage as IPv6 socket address.
         *
         * @return Reference to the address storage reinterpreted for IPv6.
         */
        inline struct sockaddr_in6& asV6(void) {
            return reinterpret_cast<struct sockaddr_in6&>(this->address);
        }

        /**
         * Access the address storage as IPv6 socket address.
         *
         * @return Reference to the address storage reinterpreted for IPv6.
         */
        inline const struct sockaddr_in6& asV6(void) const {
            return reinterpret_cast<const struct sockaddr_in6&>(this->address);
        }

    private:

        /** The storage for the generic address. */
        native_address_type address;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_IP_END_POINT_H_INCLUDED */
