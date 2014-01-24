/*
 * include\the\system\net\socket_option.h
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

#ifndef THE_SYSTEM_NET_SOCKET_OPTION_H_INCLUDED
#define THE_SYSTEM_NET_SOCKET_OPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/memory.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace net {

    /**
     * Encapsulates a socket option.
     *
     * The non-instantiable class the::system::net::socket_options provides
     * a set of well-known socket options that can be used out-of-the-box.
     *
     * @tparam L The level at which the option is defined.
     * @tparam N The socket option for which the value is to be set.
     * @tparam T The type of the option (this must have the correct size).
     */
    template<int L, int N, class T> class socket_option {

    public:

        /** The type of the option (this must have the correct size). */
        typedef T option_type;

        /** The type used for measuring the size of a socket option. */
#ifdef THE_WINDOWS
        typedef int size_type;
#elif THE_LINUX
        typedef socklen_t size_type;
#else  /* THE_WINDOWS */
#error "the::system::net::socket_option must define size_type!"
#endif /* THE_WINDOWS */

        /** The level at which the option is defined. */
        static const int level;

        /* The socket option for which the value is to be set. */
        static const int name;

        /** The size of the option type. */
        static const size_type size;

        /**
         * Initialises a new instance.
         */
        socket_option(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        socket_option(const socket_option& rhs);

        /**
         * Initialises a new instance with the given value.
         *
         * @param value The initial value of the option.
         */
        explicit socket_option(const option_type& value);

        /** Dtor. */
        ~socket_option(void);

        /**
         * Gets the current value of the option.
         *
         * @return The current value of the option.
         */
        inline const option_type& value(void) const {
            THE_STACK_TRACE;
            return this->value;
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        socket_option& operator =(const socket_option& rhs);

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        socket_option& operator =(const option_type& rhs);

        /**
         * Cast to the native option value.
         *
         * @return The option value.
         */
        inline operator const option_type&(void) const {
            THE_STACK_TRACE;
            return this->option_value;
        }

    private:

        /** The actual option value. */
        option_type option_value;
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::net::socket_option<L, N, T>::level
 */
template<int L, int N, class T>
const int the::system::net::socket_option<L, N, T>::level = L;


/*
 * the::system::net::socket_option<L, N, T>::name
 */
template<int L, int N, class T>
const int the::system::net::socket_option<L, N, T>::name = N;


/*
 * the::system::net::socket_option<L, N, T>::option_size
 */
template<int L, int N, class T>
const typename the::system::net::socket_option<L, N, T>::size_type
the::system::net::socket_option<L, N, T>::size = static_cast<typename
    the::system::net::socket_option<L, N, T>::size_type>(sizeof(T));


/*
 * the::system::net::socket_option<L, N, T>::socket_option
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>::socket_option(void) {
    THE_STACK_TRACE;
    the::zero_memory(&this->option_value);
}


/*
 * the::system::net::socket_option<L, N, T>::socket_option
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>::socket_option(
        const socket_option& rhs) {
    THE_STACK_TRACE;
    *this = rhs;
}


/*
 * the::system::net::socket_option<L, N, T>::socket_option
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>::socket_option(
        const option_type& value) {
    THE_STACK_TRACE;
    *this = value;
}


/*
 * the::system::net::socket_option<L, N, T>::~socket_option
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>::~socket_option(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::net::socket_option<L, N, T>::operator =
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>&
the::system::net::socket_option<L, N, T>::operator =(const socket_option& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        ::memcpy(&this->option_value, rhs.option_value,
            sizeof(this->option_value));
    }
    return *this;
}


/*
 * the::system::net::socket_option<L, N, T>::operator =
 */
template<int L, int N, class T>
the::system::net::socket_option<L, N, T>&
the::system::net::socket_option<L, N, T>::operator =(const option_type& rhs) {
    THE_STACK_TRACE;
    if (&this->option_value != &rhs) {
        ::memcpy(&this->option_value, &rhs, sizeof(this->option_value));
    }
    return *this;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_SOCKET_OPTION_H_INCLUDED */
