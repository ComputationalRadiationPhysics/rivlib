/*
 * include\the\system\net\ip_address4.h
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

#ifndef THE_SYSTEM_NET_IP_ADDRESS4_H_INCLUDED
#define THE_SYSTEM_NET_IP_ADDRESS4_H_INCLUDED
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
#include <arpa/inet.h>
#endif  /* defined(THE_WINDOWS) */

#include "the/stack_trace.h"

#include "the/system/net/ip_address_base.h"


namespace the {
namespace system {
namespace net {

    /**
     * Represents a version 4 IP address.
     */
    class ip_address4 : public ip_address_base<ip_address4,
            struct in_addr, address_family::inter_network4> {

    public:

        /**
         * Parse and IPv4 address from the well-known dotted string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in dotted string representation. Host names
         *                are not valid.
         *
         * @return The IP address represented by the string. The IP address is
         *         in network-byte order (bytes ordered left to right) and
         *         therefore ready-to-use.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         * @throws the::argument_null_exception If 'address' is NULL.
         */
        static ip_address4 from_dotted_string(const char *address);

        /**
         * Parse and IPv4 address from the well-known dotted string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in dotted string representation. Host names
         *                are not valid.
         *
         * @return The IP address represented by the string. The IP address is
         *         in network-byte order (bytes ordered left to right) and
         *         therefore ready-to-use.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         * @throws the::argument_null_exception If 'address' is NULL.
         */
        static ip_address4 from_dotted_string(const wchar_t *address);

        /**
         * Parse and IPv4 address from the well-known dotted string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in dotted string representation. Host names
         *                are not valid.
         *
         * @return The IP address represented by the string. The IP address is
         *         in network-byte order (bytes ordered left to right) and
         *         therefore ready-to-use.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         */
        inline static ip_address4 from_dotted_string(const astring& address) {
            THE_STACK_TRACE;
            return ip_address4::from_dotted_string(address.c_str());
        }

        /**
         * Parse and IPv4 address from the well-known dotted string format.
         *
         * @param address The address to be parsed. This must be a valid IP 
         *                address in dotted string representation. Host names
         *                are not valid.
         *
         * @return The IP address represented by the string. The IP address is
         *         in network-byte order (bytes ordered left to right) and
         *         therefore ready-to-use.
         *
         * @throws the::format_exception If 'address' does not designate a
         *                               valid IP address.
         */
        inline static ip_address4 from_dotted_string(const wstring& address) {
            THE_STACK_TRACE;
            return ip_address4::from_dotted_string(address.c_str());
        }

        /** 
         * The all-hosts group multicast address addressing all nodes in the 
         * subnet (224.0.0.1).
         */
        static const ip_address4 all_nodes_on_link;

        /** 
         * The all-routers multicast group address addressing all routers in the
         * subnet (224.0.0.2).
         */
        static const ip_address4 all_routers_on_link;

        /** 
         * Constant special IP address that allows receiving from all available
         * adapters and sending from the default (lowest-numbered adapter)
         * interface.
         */
        static const ip_address4 any;

        /** Constant broadcast address (255.255.255.255). */
        static const ip_address4 broadcast;

        /** Constant loopback address (127.0.0.1). */
        static const ip_address4 loopback;

        /** Constant invalid IP address. */
        static const ip_address4 none;

        /**
         * Initialises a new instance.
         *
         * The address is equal to ip_address4::any after initialisation.
         */
        ip_address4(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        ip_address4(const ip_address4& rhs);

        /**
         * Create new instance representing the given native address.
         *
         * This ctor allows implicit casts.
         *
         * @param address The native address to be wrapped.
         */
        ip_address4(const native_address_type& address);

        /**
         * Create an IPv4 address from individual bytes. The bytes must be 
         * specified in network byte order.
         *
         * @param b1  Byte number 1 (in network byte order) of the IP address.
         * @param b2  Byte number 2 (in network byte order) of the IP address.
         * @param b3  Byte number 3 (in network byte order) of the IP address.
         * @param b4  Byte number 4 (in network byte order) of the IP address.
         */
        ip_address4(const byte b1, const byte b2, const byte b3, const byte b4);

        /**
         * Initialises a new instance.
         *
         * This ctor does not allow implicit casts.
         *
         * @param address         The address bytes as a single DWORD. If not 
         *                        specified differently, the address is assumed 
         *                        to be in host byte order.
         * @param isHostByteOrder True if 'address' is given in host byte order 
         *                        and therefore must be converted to network 
         *                        byte order. Thie parameter defaults to true.
         */
        explicit ip_address4(const uint32_t address, 
            const bool isHostByteOrder = true);

        /** Dtor. */
        virtual ~ip_address4(void);

        /**
         * Convert the IP address to an ANSI string.
         *
         * @return The string representation of the IP address.
         */
        virtual astring to_astring(void) const;

        /**
         * Applies a subnet mask on this IP address.
         *
         * @param mask The subnet mask.
         *
         * @return *this.
         *
         * @throws argument_exception If 'mask' and this address do not have
         *                            the same size.
         */
        inline ip_address4& operator &=(const ip_address4& rhs) {
            THE_STACK_TRACE;
            this->apply_netmask(rhs);
            return *this;
        }

        /**
         * Creates an IP address representing this address with the given subnet
         * mask applied.
         *
         * @param mask The subnet mask.
         *
         * @return This address with the subnet mask applied.
         *
         * @throws argument_exception If 'mask' and this address do not have
         *                            the same size.
         */
        inline ip_address4 operator &(const ip_address4& rhs) {
            THE_STACK_TRACE;
            ip_address4 retval(*this);
            retval.apply_netmask(rhs);
            return retval;
        }

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
        inline ip_address4& operator &=(const uint32_t prefix) {
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
        inline ip_address4 operator &(const uint32_t prefix) {
            THE_STACK_TRACE;
            ip_address4 retval(*this);
            retval.apply_netmask(prefix);
            return retval;
        }

    protected:

        /** Super class typedef. */
        typedef ip_address_base<ip_address4, struct in_addr,
            the::system::net::address_family::inter_network4> base;

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_IP_ADDRESS4_H_INCLUDED */
