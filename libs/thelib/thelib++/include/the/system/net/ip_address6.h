/*
 * include\the\system\net\ip_address6.h
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

#ifndef THE_SYSTEM_NET_IP_ADDRESS6_H_INCLUDED
#define THE_SYSTEM_NET_IP_ADDRESS6_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if defined(THE_WINDOWS)
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(THE_LINUX)
#include <arpa/inet.h>
#endif  /* defined(THE_WINDOWS) */

#include "the/system/net/ip_address_base.h"


namespace the {
namespace system {
namespace net {

    /**
     * Represents a version 6 IP address.
     */
    class ip_address6 : public ip_address_base<ip_address6, 
            struct in6_addr, the::system::net::address_family::inter_network6> {

    public:

        /**
         * Parse an IPv6 address from the well-known hexadecimal string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in hexadecimal string representation. Host 
         *                names are not valid.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         * @throws the::argument_null_exception If 'address' is NULL.
         */
        static ip_address6 from_hexadecimal_string(const char *address);

        /**
         * Parse an IPv6 address from the well-known hexadecimal string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in hexadecimal string representation. Host 
         *                names are not valid.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         * @throws the::argument_null_exception If 'address' is NULL.
         */
        static ip_address6 from_hexadecimal_string(const wchar_t *address);

        /**
         * Parse an IPv6 address from the well-known hexadecimal string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in hexadecimal string representation. Host 
         *                names are not valid.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         */
        inline static ip_address6 from_hexadecimal_string(
                const astring& address) {
            THE_STACK_TRACE;
            return ip_address6::from_hexadecimal_string(address.c_str());
        }

        /**
         * Parse an IPv6 address from the well-known hexadecimal string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in hexadecimal string representation. Host 
         *                names are not valid.
         *
         * @return The IP address represented by the string.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         */
        inline static ip_address6 from_hexadecimal_string(
                const wstring& address) {
            THE_STACK_TRACE;
            return ip_address6::from_hexadecimal_string(address.c_str());
        }

        /** 
         * The all-hosts group multicast address addressing all nodes in the 
         * link local (subnet) scope.
         */
        static const ip_address6 all_nodes_on_link;

        /** 
         * The all-routers group multicast address addressing all nodes in the
         * link local (subnet) scope.
         */
        static const ip_address6 all_routers_on_link;

        /** 
         * The all-hosts group multicast address addressing all nodes in the 
         * node local (interface) scope.
         */
        static const ip_address6 all_nodes_on_nodes;

        /** 
         * Constant special IP address that allows receiving from all available
         * adapters and sending from the default (lowest-numbered adapter)
         * interface.
         */
        static const ip_address6 any;

        /** Constant loopback address (::1). */
        static const ip_address6 loopback;

        ///** 
        // * The all-hosts group multicast address addressing all nodes in the 
        // * site local scope (packets may be routed, but not by border routers).
        // */
        //static const ip_address6 multicast_all_site_local_hosts;

        ///** 
        // * The all-routers group multicast address addressing all nodes in the 
        // * site local scope (packets may be routed, but not by border routers).
        // */
        //static const ip_address6 multicast_all_site_local_routers;

        /**
         * Initialises a new instance.
         *
         * The address is equal to ip_address6::any after initialisation.
         */
        ip_address6(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        ip_address6(const ip_address6& rhs);

        /**
         * Create new instance representing the given native address.
         *
         * This ctor allows implicit casts.
         *
         * @param address The native address to be wrapped.
         */
        ip_address6(const native_address_type& address);

        /**
         * Create an IPv6 address from individual bytes. The bytes must be 
         * specified in network byte order.
         *
         * @param b1  Byte number 1 (in network byte order) of the IP address.
         * @param b2  Byte number 2 (in network byte order) of the IP address.
         * @param b3  Byte number 3 (in network byte order) of the IP address.
         * @param b4  Byte number 4 (in network byte order) of the IP address.
         * @param b5  Byte number 5 (in network byte order) of the IP address.
         * @param b6  Byte number 6 (in network byte order) of the IP address.
         * @param b7  Byte number 7 (in network byte order) of the IP address.
         * @param b8  Byte number 8 (in network byte order) of the IP address.
         * @param b9  Byte number 9 (in network byte order) of the IP address.
         * @param b10 Byte number 10 (in network byte order) of the IP address.
         * @param b11 Byte number 11 (in network byte order) of the IP address.
         * @param b12 Byte number 12 (in network byte order) of the IP address.
         * @param b13 Byte number 13 (in network byte order) of the IP address.
         * @param b14 Byte number 14 (in network byte order) of the IP address.
         * @param b15 Byte number 15 (in network byte order) of the IP address.
         * @param b16 Byte number 16 (in network byte order) of the IP address.
         */
        ip_address6(const byte b1, const byte b2, const byte b3, const byte b4,
            const byte b5, const byte b6, const byte b7, const byte b8,
            const byte b9, const byte b10, const byte b11, const byte b12,
            const byte b13, const byte b14, const byte b15, const byte b16);

        /** Dtor. */
        virtual ~ip_address6(void);

        /**
         * Determines whether the address is a link local address.
         *
         * @return true if the address is a link local address, false otherwise.
         */
        inline bool is_link_local(void) const {
            return (IN6_IS_ADDR_LINKLOCAL(&this->address) != 0);
        }

        /**
         * Answer whether the IP address is an IPv6 multicast global address.
         *
         * @return true if the IP address is multicast global address,
         *         false otherwise.
         */
        inline bool is_multicast(void) const {
            return (IN6_IS_ADDR_MULTICAST(&this->address) != 0);
        }

        /**
         * Answer whether the IP address is an IPv6 site local address.
         *
         * @return true if the address is a site local address, false otherwise.
         */
        inline bool is_site_local(void) const {
            return (IN6_IS_ADDR_SITELOCAL(&this->address) != 0);
        }

        /**
         * Answer whether the address is unspecified, i.e. ANY.
         *
         * @return true if the address is unspecified, false otherwise.
         */
        inline bool is_unspecified(void) const {
            return (IN6_IS_ADDR_UNSPECIFIED(&this->address) != 0);
        }

        /**
         * Answer whether the IP address is an IPv4-compatible IPv6 address.
         *
         * An IPv6 address is IPv4 compatible if it is assigned to an IPv6/IPv4 
         * node, which bears the high-order 96-bit prefix 0:0:0:0:0:0, and an
         * IPv4 address in the low-order 32-bits. IPv4-compatible addresses are 
         * used by the automatic tunneling mechanism.
         *
         * @return true if the IPv6 address is an IPv4-compatible address,
         *         false otherwise.
         */
        inline bool is_v4_compatible(void) const {
            return (IN6_IS_ADDR_V4COMPAT(&this->address) != 0);
        }

        /**
         * Answer thether the IP address is an IPv4-mapped IPv6 address.
         *
         * @return true if the IPv6 address is an IPv4-mapped address,
         *         false otherwise.
         */
        inline bool is_v4_mapped(void) const {
            return (IN6_IS_ADDR_V4MAPPED(&this->address) != 0);
        }

        /**
         * Convert the IP address to an ANSI string.
         *
         * @return The string representation of the IP address.
         */
        virtual astring to_astring(void) const;

        /**
         * Converts the IP address into its prefix.
         *
         * @param prefixLength The length of the prefix to extract in bits.
         *
         * @return *this.
         *
         * @throws argument_exception If 'prefixLength' is illegal for this
         *                            address.
         */
        inline ip_address6& operator &=(const uint32_t prefix) {
            THE_STACK_TRACE;
            this->apply_netmask(prefix);
            return *this;
        }

        /**
         * Returns the prefix of the specified length.
         *
         * @param prefixLength The length of the prefix to extract in bits.
         *
         * @return The prefix of the given length.
         *
         * @throws argument_exception If 'prefixLength' is illegal for this
         *                            address.
         */
        inline ip_address6 operator &(const uint32_t prefix) {
            THE_STACK_TRACE;
            ip_address6 retval(*this);
            retval.apply_netmask(prefix);
            return retval;
        }

    protected:

        /** Super class typedef. */
        typedef ip_address_base<ip_address6, struct in6_addr,
            the::system::net::address_family::inter_network6> base;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_IP_ADDRESS6_H_INCLUDED */
