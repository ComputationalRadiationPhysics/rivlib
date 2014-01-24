/*
 * include\the\system\net\ip_address_base.h
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

#ifndef THE_SYSTEM_NET_IP_ADDRESS_BASE_H_INCLUDED
#define THE_SYSTEM_NET_IP_ADDRESS_BASE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <WinSock2.h>
#endif /* THE_WINDOWS */

#include <string>

#include "the/index_out_of_range_exception.h"
#include "the/memory.h"
#include "the/stack_trace.h"

#include "the/system/net/ip_address.h"


namespace the {
namespace system {
namespace net {

    /**
     * This class provides the base implementation for IP addresses.
     *
     * This base class uses the "Curiously Recurring Template Pattern", which is
     * also widely used in ATL, to implement most of the behaviour that the 
     * derived classes should expose.
     *
     * @tparam D The type of the derived class.
     * @tparam T The type of the native address, e.g. struct in_addr in case of
     *           an IPv4 address and struct in6_addr in case of an IPv6
     *           address.
     * @tparam F The address family (from the::system::net::address_family
     *           enumeration that is represented by the derived class D).
     *           Implementors are responsible for ensuring that the native
     *           address type and the address family match.
     */
    template<class D, class T, address_family F>
    class ip_address_base : public the::system::net::ip_address {

    public:

        /** The actual address type that is derived from this class. */
        typedef D address_type;

        /** The type of the native IP address. */
        typedef T native_address_type;

        /** Dtor. */
        virtual ~ip_address_base(void);

        /**
         * Gets the address family of this address.
         *
         * @return The address family.
         */
        virtual net::address_family address_family(void) const;

        /**
         * Answer whether this IP address is the "any" address that allows for
         * receiving from all adapters.
         *
         * Note: Subclasses must provide a static instance called 'any' in 
         * order to make this implementation work.
         *
         * @return true if this address is the any address, false otherwise.
         */
        virtual bool is_any(void) const;

        /**
         * Answer whether this IP address is the loopback address.
         *
         * Note: Subclasses must provide a static instance called 'loopback' in
         * order to make this implementation work.
         *
         * @return true if the address is the loopback address, false otherwise.
         */
        virtual bool is_loopback(void) const;

        /**
         * Answer the size of the IP address in bytes.
         *
         * @return The size of the IP address in bytes.
         */
        virtual size_t size(void) const;

        /**
         * Cast to native_address_type.
         *
         * @return The native representation of the address.
         */
        inline operator const native_address_type&(void) const {
            THE_STACK_TRACE;
            return this->address;
        }

        /**
         * Cast to native_address_type.
         *
         * @return A reference to the native representation of the address.
         */
        inline operator native_address_type&(void) const {
            THE_STACK_TRACE;
            return this->address;
        }

        /**
         * Cast to a pointer on the native_address_type.
         *
         * @return A pointer to the native representation of the address.
         */
        inline operator const native_address_type *(void) const {
            THE_STACK_TRACE;
            return &this->address;
        }

        /**
         * Cast to a pointer on the native_address_type.
         *
         * @return A pointer to the native representation of the address.
         */
        inline operator native_address_type *(void) const {
            THE_STACK_TRACE;
            return &this->address;
        }

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        inline address_type& operator =(const address_type& rhs) {
            THE_STACK_TRACE;
            return this->operator =(rhs.address);
        }

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        address_type& operator =(const native_address_type& rhs);

    protected:

        /** Super class typedef. */
        typedef the::system::net::ip_address base;

        /**
         * Initialises a new instance.
         */
        ip_address_base(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline ip_address_base(const ip_address_base& rhs) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /**
         * Initialises a new instance.
         *
         * @param address The native address value.
         */
        explicit ip_address_base(const native_address_type& address);

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
        virtual byte *get_address_bytes(void);

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
        virtual const byte *get_address_bytes(void) const;

        /** The native IP address. */
        native_address_type address;
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::ip_address_base<D, T, F>::~ip_address_base
 */
template<class D, class T, the::system::net::address_family F>
the::system::net::ip_address_base<D, T, F>::~ip_address_base(
        void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::ip_address_base<D, T, F>::address_family
 */
template<class D, class T, the::system::net::address_family F>
the::system::net::address_family 
the::system::net::ip_address_base<D, T, F>::address_family(void) const {
    THE_STACK_TRACE;
    return F;
}


/*
 * the::system::net::ip_address_base<D, T, F>::is_any
 */
template<class D, class T, the::system::net::address_family F>
bool the::system::net::ip_address_base<D, T, F>::is_any(void) const {
    THE_STACK_TRACE;
    return this->equals(D::any);
}


/*
 * the::system::net::ip_address_base<D, T, F>::is_loopback
 */
template<class D, class T, the::system::net::address_family F>
bool the::system::net::ip_address_base<D, T, F>::is_loopback(
        void) const {
    THE_STACK_TRACE;
    return this->equals(D::loopback);
}


/*
 * the::system::net::ip_address_base<D, T, F>::size
 */
template<class D, class T, the::system::net::address_family F>
size_t the::system::net::ip_address_base<D, T, F>::size(void) const {
    THE_STACK_TRACE;
    return sizeof(native_address_type);
}



/* 
 * the::system::net::ip_address_base<D, T, F>::operator =
 */
template<class D, class T, the::system::net::address_family F> 
typename the::system::net::ip_address_base<D, T, F>::address_type&
the::system::net::ip_address_base<D, T, F>::operator =(
        const native_address_type& rhs) {
    THE_STACK_TRACE;
    if (this->address != &rhs) {
        base::operator =(rhs);
        ::memcpy(&this->address, &rhs, sizeof(this->address));
    }
    return *this;
}


/*
 * the::system::net::ip_address_base<D, T, F>::ip_address_base
 */
template<class D, class T, the::system::net::address_family F>
the::system::net::ip_address_base<D, T, F>::ip_address_base(
        void) : base() {
    THE_STACK_TRACE;
    the::zero_memory(&this->address, sizeof(this->address));
}


/*
 * the::system::net::ip_address_base<D, T, F>::ip_address_base
 */
template<class D, class T, the::system::net::address_family F>
the::system::net::ip_address_base<D, T, F>::ip_address_base(
        const native_address_type& address) : base() {
    THE_STACK_TRACE;
    ::memcpy(&this->address, &address, sizeof(this->address));
}


/*
 * the::system::net::ip_address_base<D, T, F>::get_address_bytes
 */
template<class D, class T, the::system::net::address_family F>
byte *the::system::net::ip_address_base<D, T, F>::get_address_bytes(
        void) {
    THE_STACK_TRACE;
    return reinterpret_cast<byte *>(&this->address);
}


/*
 * the::system::net::ip_address_base<D, T, F>::get_address_bytes
 */
template<class D, class T, the::system::net::address_family F>
const byte *the::system::net::ip_address_base<D, T, F>
        ::get_address_bytes(void) const {
    THE_STACK_TRACE;
    return reinterpret_cast<const byte *>(&this->address);
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_IP_ADDRESS_BASE_H_INCLUDED */
