/*
 * include\the\system\net\ip_address.h
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

#ifndef THE_SYSTEM_NET_IP_ADDRESS_H_INCLUDED
#define THE_SYSTEM_NET_IP_ADDRESS_H_INCLUDED
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
#endif /* THE_WINDOWS */

#include "the/comparable.h"
#include "the/string.h"
#include "the/stack_trace.h"
#include "the/types.h"

#include "the/system/net/address_family.h"


namespace the {
namespace system {
namespace net {

    /**
     * Defines the interface for a version-agnostic IP address.
     */
    class ip_address : public comparable<ip_address> {

    public:

        /** Shared pointer to a constant IP address. */
        typedef std::shared_ptr<const ip_address> const_shared_pointer_type;

        /**
         * Shared pointer to IP addresses. This type should usually be used to
         * built software that can handle any type of IP address, i.e. is
         * IP-agnostic.
         */
        typedef std::shared_ptr<ip_address> shared_pointer_type;

        /**
         * Gets the "any" address for the given address family.
         *
         * @param addressFamily The address family to get the "any" address for.
         *
         * @throws the::argument_exception If 'addressFamily' is invalid.
         */
        static shared_pointer_type any(
            const the::system::net::address_family& addressFamily);

        /**
         * Convert a 16-bit integer from host to network byte order.
         *
         * @param host The value in host byte order.
         *
         * @return The value in network byte order.
         */
        static inline uint16_t host_to_network_order(const uint16_t host) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return htons(host);
#else /* defined(THE_WINDOWS) */
            return htobe16(host);
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Convert a 32-bit integer from host to network byte order.
         *
         * @param host The value in host byte order.
         *
         * @return The value in network byte order.
         */
        static inline uint32_t host_to_network_order(const uint32_t host) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return htonl(host);
#else /* defined(THE_WINDOWS) */
            return htobe32(host);
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Convert a 64-bit integer from host to network byte order.
         *
         * @param host The value in host byte order.
         *
         * @return The value in network byte order.
         */
        static inline uint64_t host_to_network_order(const uint64_t host) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return htonll(host);
#else /* defined(THE_WINDOWS) */
            return htobe64(host);
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Convert a 16-bit integer from network to host byte order.
         *
         * @param network The value in host byte order.
         *
         * @return The value in network byte order.
         */
        static inline uint16_t network_to_host_order(const uint16_t network) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return ntohs(network);
#else /* defined(THE_WINDOWS) */
            return be16toh(network);
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Convert a 32-bit integer from network to hostbyte order.
         *
         * @param network The value in host byte order.
         *
         * @return The value in network byte order.
         */
        static inline uint32_t network_to_host_order(const uint32_t network) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return ntohl(network);
#else /* defined(THE_WINDOWS) */
            return be32toh(network);
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Convert a 64-bit integer from network to host byte order.
         *
         * @param network The value in host byte order.
         *
         * @return The value in network byte order.
         */
        static inline uint64_t network_to_host_order(const uint64_t network) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return htonll(network);
#else /* defined(THE_WINDOWS) */
            return be64toh(network);
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Parse the given host name or address into an ip_address
         *
         * @param hostNameOrAddress The name or address of the host to be 
         *                          parsed. This must be a valid host name or 
         *                          an IP address in dotted string 
         *                          representation.
         * @param preferredFamily   Determines which address family is used if
         *                          the family cannot be determined from
         *                          'hostNameOrAddress', e.g. because a host 
         *                          name which is resolved to multiple addresses
         *                          was specified. This parameter defaults to
         *                          address_family::inter_network6.
         *
         * @return An ip_address representing the given host.
         *
         * @throws the::format_exception If 'hostNameOrAddress' does not 
         *                               designate an existing host or a string 
         *                               representation of an IP address.
         * @throws the::argument_null_exception If 'hostNameOrAddress' is a 
         *                                      NULL pointer.
         */
        static shared_pointer_type parse(const char *hostNameOrAddress,
            const the::system::net::address_family preferredFamily
            = the::system::net::address_family::inter_network6);

        /**
         * Parse the given host name or address into an ip_address
         *
         * @param hostNameOrAddress The name or address of the host to be 
         *                          parsed. This must be a valid host name or 
         *                          an IP address in dotted string 
         *                          representation.
         * @param preferredFamily   Determines which address family is used if
         *                          the family cannot be determined from
         *                          'hostNameOrAddress', e.g. because a host 
         *                          name which is resolved to multiple addresses
         *                          was specified. This parameter defaults to
         *                          address_family::inter_network6.
         *
         * @return An ip_address representing the given host.
         *
         * @throws the::format_exception If 'hostNameOrAddress' does not 
         *                               designate an existing host or a string 
         *                               representation of an IP address.
         * @throws the::argument_null_exception If 'hostNameOrAddress' is a 
         *                                      NULL pointer.
         */
        static shared_pointer_type parse(const wchar_t *hostNameOrAddress,
            const the::system::net::address_family preferredFamily
            = the::system::net::address_family::inter_network6);

        /**
         * Parse the given host name or address into an ip_address
         *
         * @param hostNameOrAddress The name or address of the host to be 
         *                          parsed. This must be a valid host name or 
         *                          an IP address in dotted string 
         *                          representation.
         * @param preferredFamily   Determines which address family is used if
         *                          the family cannot be determined from
         *                          'hostNameOrAddress', e.g. because a host 
         *                          name which is resolved to multiple addresses
         *                          was specified. This parameter defaults to
         *                          address_family::inter_network6.
         *
         * @return An ip_address representing the given host.
         *
         * @throws the::format_exception If 'hostNameOrAddress' does not 
         *                               designate an existing host or a string 
         *                               representation of an IP address.
         * @throws the::argument_null_exception If 'hostNameOrAddress' is a 
         *                                      NULL pointer.
         */
        inline static shared_pointer_type parse(
                const astring& hostNameOrAddress,
                const the::system::net::address_family preferredFamily
                = the::system::net::address_family::inter_network6) {
            THE_STACK_TRACE;
            return ip_address::parse(hostNameOrAddress.c_str(),
                preferredFamily);
        }

        /**
         * Parse the given host name or address into an ip_address
         *
         * @param hostNameOrAddress The name or address of the host to be 
         *                          parsed. This must be a valid host name or 
         *                          an IP address in dotted string 
         *                          representation.
         * @param preferredFamily   Determines which address family is used if
         *                          the family cannot be determined from
         *                          'hostNameOrAddress', e.g. because a host 
         *                          name which is resolved to multiple addresses
         *                          was specified. This parameter defaults to
         *                          address_family::inter_network6.
         *
         * @return An ip_address representing the given host.
         *
         * @throws the::format_exception If 'hostNameOrAddress' does not 
         *                               designate an existing host or a string 
         *                               representation of an IP address.
         * @throws the::argument_null_exception If 'hostNameOrAddress' is a 
         *                                      NULL pointer.
         */
        inline static shared_pointer_type parse(
                const wstring& hostNameOrAddress,
                const the::system::net::address_family preferredFamily
                = the::system::net::address_family::inter_network6) {
            THE_STACK_TRACE;
            return ip_address::parse(hostNameOrAddress.c_str(),
                preferredFamily);
        }

        /** Dtor. */
        virtual ~ip_address(void);

        /**
         * Gets the address family of this address.
         *
         * @return The address family.
         */
        virtual the::system::net::address_family address_family(void) const = 0;

        /**
         * Compares two ip_address instances.
         *
         * @param rhs The object to be compared.
         *
         * @return A negative number if this object is less than 'rhs', 
         *         zero if this object is equal to 'rhs',
         *         a positive number if this object is larger than 'rhs'.
         */
        virtual int compare(const ip_address& rhs) const;

        /**
         * Answer whether this IP address is the "any" address that allows for
         * receiving from all adapters.
         *
         * @return true if this address is the any address, false otherwise.
         */
        virtual bool is_any(void) const = 0;

        /**
         * Answer whether this IP address is the loopback address.
         *
         * @return true if the address is the loopback address, false otherwise.
         */
        virtual bool is_loopback(void) const = 0;

        /**
         * Answer the size of the IP address in bytes.
         *
         * @return The size of the IP address in bytes.
         */
        virtual size_t size(void) const = 0;

        /**
         * Convert the IP address to an ANSI string.
         *
         * @return The string representation of the IP address.
         */
        virtual astring to_astring(void) const = 0;

        /**
         * Convert the IP address to an unicode string.
         *
         * @return The string representation of the IP address.
         */
        virtual wstring to_wstring(void) const;

        /**
         * Answer the 'i'th byte (in network byte order) of the address.
         *
         * @param i The byte to be retrieved, which must be within 
         *          [0, this->size()[.
         *
         * @return The value at position 'i'.
         *
         * @throws the::index_out_of_range_exception If 'i' is not within
         *                                           [0, this->size()[.
         */
        byte operator [](const int i) const;

        /**
         * Writes the string representation of the IP address into a stream.
         *
         * @param lhs The stream to write to.
         * @param rhs The address to be written.
         *
         * @return 'lhs'.
         */
        template<class C, class T>
        friend inline std::basic_ostream<C, T>& operator <<(
                std::basic_ostream<C, T>& lhs, const ip_address& rhs) {
            lhs << (rhs.to_astring().c_str());
            return lhs;
        }

    protected:

        /** Super class typedef. */
        typedef comparable<ip_address> base;

        /**
         * Convert a netmask into a prefix length in bits.
         *
         * @param netmask    The address bytes of the netmask, which comprises
         *                   'lenNetmask' bytes. The netmask is assumed to be in
         *                   network byte order.
         * @param lenNetmask The size of 'netmask'
         *
         * @return The prefix length represented by the netmask.
         * 
         * @throws argument_exception If 'netmask' is not a valid netmask.
         * @throws argument_null_exception If 'netmask' is NULL.
         */
        static uint32_t netmask_to_prefix(const byte *netmask, 
            const size_t lenNetmask);

        /**
         * Convert a prefix length in bits into a netmask.
         *
         * @param outNetmask Receives the netmask. This must be an array of at 
         *                   least 'lenNetmask' bytes. The result will be in
         *                   network byte order.
         * @param lenNetmask The size of 'outNetmask' in bytes.
         * @param prefix     The prefix length in bity to be converted.
         * 
         * @throws argument_exception If 'outNetmask' comprises less bits than
         *                            the prefix.
         * @throws argument_null_exception If 'outNetmask' is NULL.
         */
        static void prefix_to_netmask(byte *outNetmask, const size_t lenNetmask,
            const uint32_t prefix);

        /**
         * Initialises a new instance.
         */
        ip_address(void);

        /**
         * Applies a subnet mask on this IP address.
         *
         * @param mask The subnet mask.
         *
         * @throws argument_exception If 'mask' and this address do not have
         *                            the same size.
         */
        void apply_netmask(const ip_address& mask);

        /**
         * Converts the IP address into its prefix.
         *
         * @param prefixLength The length of the prefix to extract in bits.
         *
         * @throws argument_exception If 'prefixLength' is illegal for this
         *                            address.
         */
        void apply_netmask(const uint32_t prefixLength);

        /**
         * Provides access to the native address bytes (in network byte order).
         *
         * This method is used by the base class to implement several other 
         * methods like array access. The pointer should not be exposed in the 
         * public interface.
         *
         * It can be assumed that the base class will check wether any index is
         * within [0, this->size()[ before accessing data returned.
         *
         * @return The address bytes.
         */
        virtual byte *get_address_bytes(void) = 0;

        /**
         * Provides access to the native address bytes (in network byte order).
         *
         * This method is used by the base class to implement several other 
         * methods like array access. The pointer should not be exposed in the 
         * public interface.
         *
         * It can be assumed that the base class will check wether any index is
         * within [0, this->size()[ before accessing data returned.
         *
         * @return The address bytes.
         */
        virtual const byte *get_address_bytes(void) const = 0;

    private:

        /**
         * Parses 'hostNameOrAddress' using the address class (ip_address4 or 
         * ip_address6) represented by 'family'.
         *
         * If an address is returned, the caller takes ownership of it!
         *
         * @param hostNameOrAddress
         * @param family
         *
         * @return The IP address if it could be parsed by either the parsing
         *         method of ip_address4 or ip_address6. The caller takes
         *         ownership of the object returned and allocated by C++ new.
         *         If parsing failed, nullptr is returned.
         */
        static ip_address *parseOnly(const char *hostNameOrAddress,
            const the::system::net::address_family family);

    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_IP_ADDRESS_H_INCLUDED */
