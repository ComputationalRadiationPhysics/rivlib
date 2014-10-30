/*
 * include\the\system\net\end_point.h
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

#ifndef THE_SYSTEM_NET_END_POINT_H_INCLUDED
#define THE_SYSTEM_NET_END_POINT_H_INCLUDED
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
#include "the/stack_trace.h"
#include "the/string.h"

#include "the/system/net/address_family.h"


namespace the {
namespace system {
namespace net {

    /**
     * Identificiation of a generic network address.
     *
     * @param T The type of the derived class which actually implements the end
     *          point address.
     */
    class end_point : comparable<end_point> {

    public:

        /** Shared pointer to a constant end point. */
        typedef std::shared_ptr<const end_point> const_shared_pointer_type;

        /**
         * Shared pointer to and end point. This type should usually be used to
         * built software that can handle any type of end point.
         */
        typedef std::shared_ptr<end_point> shared_pointer_type;

        /**
         * The type that is used to measure the size of an end point address.
         */
        typedef size_t size_type;

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline end_point(const end_point& rhs) {
            THE_STACK_TRACE;
        }

        /** Dtor. */
        virtual ~end_point(void);

        /**
         * Gets the address family of the end point.
         *
         * @return The address family.
         */
        virtual the::system::net::address_family address_family(
            void) const = 0;

        //const void *raw_data(void) const;

        /**
         * Answer the size of the native end point address in bytes.
         *
         * @return The size of the address.
         */
        virtual size_type size(void) const = 0;

        /**
         * Convert the network address address to an ANSI string.
         *
         * @return The string representation of the IP address.
         */
        virtual astring to_astring(void) const = 0;

        /**
         * Convert the network address to an unicode string.
         *
         * @return The string representation of the IP address.
         */
        virtual wstring to_wstring(void) const;

    protected:

        /**
         * Initialises a new instance.
         */
        end_point(void);
    };

} /* end namespace net */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_NET_END_POINT_H_INCLUDED */
