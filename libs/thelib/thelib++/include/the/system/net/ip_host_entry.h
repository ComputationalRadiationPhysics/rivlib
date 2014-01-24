/*
 * include\the\system\net\ip_host_entry.h
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

#ifndef THE_SYSTEM_NET_IP_HOST_ENTRY_H_INCLUDED
#define THE_SYSTEM_NET_IP_HOST_ENTRY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if defined(THE_WINDOWS)
#include <WinSock2.h>
#include <Windows.h>
#elif defined(THE_LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif /* defined(THE_WINDOWS) */

#include <vector>

#include "the/argument_exception.h"
#include "the/argument_null_exception.h"
#include "the/equatable.h"
#include "the/stack_trace.h"
#include "the/string.h"

#include "the/system/system_string.h"

#include "the/system/net/ip_address.h"
#include "the/system/net/ip_address4.h"
#include "the/system/net/ip_address6.h"

#include "the/text/string_converter.h"


namespace the {
namespace system {
namespace net {

    /**
     * The ip_host_entry class is used as a helper to return answers from DNS.
     * It associates DNS host names with their respective IP addresses.
     */
    template<class T> class ip_host_entry : the::equatable<ip_host_entry<T> > {

    public:

        /** The type of a single address. */
        typedef ip_address::shared_pointer_type address_type;

        /** The type of the address list associated with the entry. */
        typedef std::vector<address_type> address_list_type;

        /** The type of strings used for the host name and aliases. */
        typedef T string_type;

        /**
         * Initialises a new instance.
         */
        ip_host_entry(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        ip_host_entry(const ip_host_entry& rhs);

        /** Dtor. */
        virtual ~ip_host_entry(void);

        /**
         * Determines whether this object and 'rhs' are equal.
         *
         * @param rhs The object to be compared.
         *
         * @return true if this object and 'rhs' are equal, false otherwise.
         */
        virtual bool equals(const ip_host_entry& rhs) const;

        /**
         * Gets the addresses of the host.
         *
         * @return The addresses of the host.
         */
        inline const address_list_type& get_addresses(void) const {
            THE_STACK_TRACE;
            return this->addresses;
        }

        /**
         * Gets the name of the host.
         *
         * @return The name of the host.
         */
        inline const string_type& get_host_name(void) const {
            THE_STACK_TRACE;
            return this->hostName;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        ip_host_entry& operator =(const ip_host_entry& rhs);

        /**
         * Assigns the data from the native address info structure to this
         * object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         *
         * @throws argument_null_exception If 'rhs' is NULL.
         * @throws argument_exception If 'rhs' contains an address with an
         *                            address family other the IPv4 or IPv6.
         */
        ip_host_entry& operator =(const struct addrinfo *rhs);

#ifdef THE_WINDOWS
        /**
         * Assigns the data from the native address info structure to this
         * object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this
         *
         * @throws argument_null_exception If 'rhs' is NULL.
         * @throws argument_exception If 'rhs' contains an address with an
         *                            address family other the IPv4 or IPv6.
         */
        ip_host_entry& operator =(const ADDRINFOW *rhs);
#endif /* THE_WINDOWS */

    protected:

        /** Super class typedef. */
        typedef the::equatable<ip_host_entry<T> > base;

    private:

        /** The list of IP addresses registered for the host. */
        address_list_type addresses;

        /** The canonical host name. */
        string_type hostName;
    };

    /** An IP host entry using ANSI strings for host name and aliases. */
    typedef ip_host_entry<astring> aip_host_entry;

    /** An IP host entry using the native character set of the OS. */
    typedef ip_host_entry<system_string> sip_host_entry;

    /** An IP host entry using TCHAR strings for host name and aliases. */
    typedef ip_host_entry<tstring> tip_host_entry;

    /** An IP host entry using wide strings for host name and aliases. */
    typedef ip_host_entry<wstring> wip_host_entry;

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::ip_host_entry<T>::ip_host_entry
 */
template<class T> the::system::net::ip_host_entry<T>::ip_host_entry(void) 
        : base() {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_host_entry<T>::ip_host_entry
 */
template<class T>
the::system::net::ip_host_entry<T>::ip_host_entry(const ip_host_entry& rhs) {
    THE_STACK_TRACE;
    *this = rhs;
}


/*
 * the::system::net::ip_host_entry<T>::~ip_host_entry
 */
template<class T>
the::system::net::ip_host_entry<T>::~ip_host_entry(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_host_entry<T>::equals
 */
template<class T> bool the::system::net::ip_host_entry<T>::equals(
        const ip_host_entry& rhs) const {
    THE_STACK_TRACE;

    if ((this->addresses.size() == rhs.addresses.size())
            && (this->hostName == rhs.hostName)) {
        address_list_type::const_iterator leftEnd = this->addresses.end();
        address_list_type::const_iterator rightEnd = rhs.addresses.end();

        for (address_list_type::const_iterator it = this->addresses.begin();
                it != leftEnd; ++it) {
#ifdef THE_LINUX
            bool fu__YouLinux = false;
            for (address_list_type::const_iterator it2 = rhs.addresses.begin();
                    it2 != rightEnd; ++it) {
                if ((*it)->equals(**it2)) {
                    fu__YouLinux = true;
                    break;
                }
            }
            if (!fu__YouLinux) {
                return false;
            }
#else /* THE_LINUX */
            if (std::find(rhs.addresses.begin(), rightEnd, *it) == rightEnd) {
                return false;
            }
#endif /* THE_LINUX */
        }
        /* No mismatch found at this point. */

        return true;
    } else {
        return false;
    }
}


/*
 * the::system::net::ip_host_entry<T>::operator =
 */
template<class T>
the::system::net::ip_host_entry<T>& 
the::system::net::ip_host_entry<T>::operator =(const ip_host_entry& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        base::operator =(rhs);
        this->addresses = rhs.addresses;
        this->aliases = rhs.aliases;
        this->hostName = rhs.hostName;
    }
    return *this;
}


/*
 * the::system::net::ip_host_entry<T>::::operator =
 */
template<class T> the::system::net::ip_host_entry<T>& 
the::system::net::ip_host_entry<T>::operator =(const struct addrinfo *rhs) {
    THE_STACK_TRACE;
    const struct addrinfo *ai = rhs;

    if (ai == NULL) {
        throw argument_null_exception("rhs", __FILE__, __LINE__);
    }

    while (ai != NULL) {
        switch (ai->ai_family) {
            case AF_INET:
                this->addresses.push_back(ip_host_entry::address_type(
                    new ip_address4(reinterpret_cast<const sockaddr_in *>(
                    ai->ai_addr)->sin_addr)));
                break;

            case AF_INET6:
                this->addresses.push_back(ip_host_entry::address_type(
                    new ip_address6(reinterpret_cast<const sockaddr_in6 *>(
                    ai->ai_addr)->sin6_addr)));
                break;

            default:
                this->~ip_host_entry();
                throw argument_exception("ai", __FILE__, __LINE__);
        }

        if (((ai->ai_flags & AI_CANONNAME) != 0) 
                && (ai->ai_canonname != NULL)) {
            text::string_converter::convert(this->hostName, ai->ai_canonname);
        }

        ai = ai->ai_next;
    }

    return *this;
}


#ifdef THE_WINDOWS
/*
 * the::system::net::ip_host_entry<T>::::operator =
 */
template<class T> the::system::net::ip_host_entry<T>& 
the::system::net::ip_host_entry<T>::operator =(const ADDRINFOW *rhs) {
    THE_STACK_TRACE;
    const ADDRINFOW *ai = rhs;

    if (ai == NULL) {
        throw argument_null_exception("rhs", __FILE__, __LINE__);
    }

    while (ai != NULL) {
        switch (ai->ai_family) {
            case AF_INET:
                this->addresses.push_back(ip_host_entry::address_type(
                    new ip_address4(reinterpret_cast<const sockaddr_in *>(
                    ai->ai_addr)->sin_addr)));
                break;

            case AF_INET6:
                this->addresses.push_back(ip_host_entry::address_type(
                    new ip_address6(reinterpret_cast<const sockaddr_in6 *>(
                    ai->ai_addr)->sin6_addr)));
                break;

            default:
                this->~ip_host_entry();
                throw argument_exception("ai", __FILE__, __LINE__);
        }

        if (((ai->ai_flags & AI_CANONNAME) != 0) 
                && (ai->ai_canonname != NULL)) {
            text::string_converter::convert(this->hostName, ai->ai_canonname);
        }

        ai = ai->ai_next;
    }

    return *this;
}
#endif /* THE_WINDOWS */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_IP_HOST_ENTRY_H_INCLUDED */
