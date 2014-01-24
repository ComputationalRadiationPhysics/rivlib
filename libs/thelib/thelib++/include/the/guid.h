/*
 * include\the\guid.h
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
 * GUID.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_GUID_H_INCLUDED
#define THE_GUID_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if defined(THE_WINDOWS)
#include <guiddef.h>
#elif defined(THE_LINUX)
#include <uuid/uuid.h>
#endif /* defined(THE_WINDOWS) */

#include "the/comparable.h"
#include "the/force_inline.h"
#include "the/stack_trace.h"
#include "the/string.h"
#include "the/types.h"

#ifdef _MSC_VER
#pragma comment(lib, "rpcrt4")
#endif /* _MSC_VER */


namespace the {

    /**
     * This class represents a Globally Unique Identifier (GUID).
     */
    class guid : the::comparable<guid> {

    public:

        /** The type returned as hash code of the GUID. */
        typedef size_t hash_type;

#if defined(THE_WINDOWS)
        typedef ::GUID native_guid_type;
#elif defined(THE_LINUX)
        typedef ::uuid_t native_guid_type;
#else /* defined(THE_WINDOWS) */
#error "the::guid must defined native_guid_type!"
#endif /* defined(THE_WINDOWS) */

        /**
         * Make a new GUID.
         *
         * @param outGuid     Receives the newly created GUID.
         * @param doNotUseMac If this flag is set, an algorithm will be used
         *                    that does not include the MAC address in the
         *                    GUID. This is the default.
         *
         * @return outGuid.
         *
         * @throws the::invalid_operation_exception If the operation fail, e.g.
         *                                          because the machine has no
         *                                          network adapter.
         */
        static guid& create(guid& outGuid, const bool doNotUseMac = true);


        /**
         * Make a new GUID.
         *
         * @param doNotUseMac If this flag is set, an algorithm will be used
         *                    that does not include the MAC address in the
         *                    GUID. This is the default.
         *
         * @return A guid object representing a new GUID.
         *
         * @throws the::invalid_operation_exception If the operation fail, e.g.
         *                                          because the machine has no
         *                                          network adapter.
         */
        static inline guid create(const bool doNotUseMac = true) {
            THE_STACK_TRACE;
            guid retval;
            return guid::create(retval, doNotUseMac);
        }

        /**
         * Parse the GUID from a string representation.
         *
         * @param outGuid Receives the newly created GUID.
         * @param str     The string representation of the GUID.
         *
         * @return outGuid.
         *
         * @throws the::argument_null_exception If 'str' is nullptr.
         * @throws the::format_exception If 'str' does not represent a GUID.
         */
        static guid& parse(guid& outGuid, const char *str);

        /**
         * Parse the GUID from a string representation.
         *
         * @param outGuid Receives the newly created GUID.
         * @param str     The string representation of the GUID.
         *
         * @return outGuid.
         *
         * @throws the::argument_null_exception If 'str' is nullptr.
         * @throws the::format_exception If 'str' does not represent a GUID.
         */
        static guid& parse(guid& outGuid, const wchar_t *str);

        /**
         * Parse the GUID from a string representation.
         *
         * @param str The string representation of the GUID.
         *
         * @return The GUID that represents 'str'.
         *
         * @throws the::argument_null_exception If 'str' is nullptr.
         * @throws the::format_exception If 'str' does not represent a GUID.
         */
        static inline guid parse(const char *str) {
            THE_STACK_TRACE;
            guid retval;
            return guid::parse(retval, str);
        }

        /**
         * Parse the GUID from a string representation.
         *
         * @param str The string representation of the GUID.
         *
         * @return The GUID that represents 'str'.
         *
         * @throws the::argument_null_exception If 'str' is nullptr.
         * @throws the::format_exception If 'str' does not represent a GUID.
         */
        static inline guid parse(const wchar_t *str) {
            THE_STACK_TRACE;
            guid retval;
            return guid::parse(retval, str);
        }

        /**
         * Parse the GUID from a string representation.
         *
         * @param str The string representation of the GUID.
         *
         * @return The GUID that represents 'str'.
         *
         * @throws the::argument_null_exception If 'str' is nullptr.
         * @throws the::format_exception If 'str' does not represent a GUID.
         */
        static inline guid parse(const the::astring& str) {
            THE_STACK_TRACE;
            guid retval;
            return guid::parse(retval, str.c_str());
        }

        /**
         * Parse the GUID from a string representation.
         *
         * @param str The string representation of the GUID.
         *
         * @return The GUID that represents 'str'.
         *
         * @throws the::argument_null_exception If 'str' is nullptr.
         * @throws the::format_exception If 'str' does not represent a GUID.
         */
        static inline guid parse(const the::wstring& str) {
            THE_STACK_TRACE;
            guid retval;
            return guid::parse(retval, str.c_str());
        }

        /**
         * Create a zero-initialised GUID.
         */
        guid(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline guid(const guid& rhs) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /**
         * Create a GUID object from a native_guid_type structure.
         *
         * @param guid The native_guid_type structure.
         */
        inline guid(const native_guid_type& guid) {
            THE_STACK_TRACE;
            *this = guid;
        }

        /**
         * Create a GUID from an array of 16 bytes.
         *
         * @param b An array of 16 bytes.
         * 
         * @throws the::argument_null_exception If b is nullptr.
         */
        guid(const byte b[16]);

        /**
         * Create a GUID from 16 bytes.
         *
         * @param b1  Byte # 1.
         * @param b2  Byte # 2.
         * @param b3  Byte # 3.
         * @param b4  Byte # 4.
         * @param b5  Byte # 5.
         * @param b6  Byte # 6.
         * @param b7  Byte # 7.
         * @param b8  Byte # 8.
         * @param b9  Byte # 9.
         * @param b10 Byte # 10.
         * @param b11 Byte # 11.
         * @param b12 Byte # 12.
         * @param b13 Byte # 13.
         * @param b14 Byte # 14.
         * @param b15 Byte # 15.
         * @param b16 Byte # 16.
         */
        guid(const byte b1, const byte b2, const byte b3, const byte b4,
            const byte b5, const byte b6, const byte b7, const byte b8,
            const byte b9, const byte b10, const byte b11, const byte b12,
            const byte b13, const byte b14, const byte b15, const byte b16);

        /**
         * Create a GUID from an integer, two shorts and eight bytes.
         *
         * @param i   The first four bytes.
         * @param s1  Bytes #5 and 6.
         * @param s2  Bytes #7 and 8.
         * @param b9  Byte # 9.
         * @param b10 Byte # 10.
         * @param b11 Byte # 11.
         * @param b12 Byte # 12.
         * @param b13 Byte # 13.
         * @param b14 Byte # 14.
         * @param b15 Byte # 15.
         * @param b16 Byte # 16.
         */
        guid(const uint32_t i, const uint16_t s1, const uint16_t s2,
            const byte b1, const byte b2, const byte b3, const byte b4,
            const byte b5, const byte b6, const byte b7, const byte b8);

        /**
         * Create a GUID from an integer, two shorts and an array of eight 
         * bytes.
         *
         * @param i   The first four bytes.
         * @param s1  Bytes #5 and 6.
         * @param s2  Bytes #7 and 8.
         * @param b9  Byte # 9.
         * @param b   The last eight bytes.
         *
         * @throws argument_null_exception If 'b' is nullptr.
         */
        guid(const uint32_t i, const uint16_t s1, const uint16_t s2, 
            const byte b[8]);

        /** Dtor. */
        virtual ~guid(void);

        /**
         * Set the whole GUID zero.
         */
        void clear(void);

        /**
         * Compares this object to 'rhs'.
         *
         * @param rhs The object to be compared.
         *
         * @return A negative number if this object is less than 'rhs', 
         *         zero if this object is equal to 'rhs',
         *         a positive number if this object is larger thatn 'rhs'.
         */
        virtual int compare(const guid& rhs) const;

        /**
         * Determines whether this object and 'rhs' are equal.
         *
         * @param rhs The object to be compared.
         *
         * @return true if this object and 'rhs' are equal, false otherwise.
         */
        virtual bool equals(const guid& rhs) const;

        /**
         * Answer a hash code of the GUID.
         *
         * @return A hash code of the GUID.
         */
        hash_type hash_code(void) const;

        /**
         * Return a string representation of the GUID.
         *
         * @return A string representation of the GUID.
         *
         * @throws std::bad_alloc In case there is insufficient memory.
         */
        the::astring to_astring(void) const;

        /**
         * Return a string representation of the GUID.
         *
         * @return A string representation of the GUID.
         *
         * @throws std::bad_alloc In case there is insufficient memory.
         */
        the::wstring to_wstring(void) const;

        /**
         * Answer whether the GUID is completely filled with zeros.
         *
         * @return true if the GUID is zero, false otherwise.
         */
        bool zero(void) const;

        /**
         * Assignment.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        guid& operator =(const guid& rhs);

        /**
         * Assignment from a native GUID.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        guid& operator =(const native_guid_type& rhs);

        /**
         * Assignment from a native GUID.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         *
         * @throws the::argument_null_exception If 'rhs' is nullptr.
         */
        guid& operator =(const native_guid_type *rhs);

        /**
         * Provides access to the native GUID data.
         *
         * @return The native GUID data.
         */
        inline operator native_guid_type& (void) {
            THE_STACK_TRACE;
            return this->id;
        }

        /**
         * Provides access to the native GUID data.
         *
         * @return The native GUID data.
         */
        inline operator const native_guid_type& (void) const {
            THE_STACK_TRACE;
            return this->id;
        }

    private:

        /**
         * Provides access to the raw GUID data.
         *
         * @param id The native_guid_type to get the raw GUID data from.
         *
         * @return The raw GUID data.
         */
        static THE_FORCE_INLINE byte *data(native_guid_type& id) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return reinterpret_cast<byte *>(&id);
#elif defined(THE_LINUX)
            return static_cast<byte *>(id);
#else /* defined(THE_WINDOWS) */
#error "the::guid::data() must be implemented."
#endif /* defined(THE_WINDOWS) */
        }

        /**
         * Provides access to the raw GUID data.
         *
         * @param id The native_guid_type to get the raw GUID data from.
         *
         * @return The raw GUID data.
         */
        static THE_FORCE_INLINE const byte *data(const native_guid_type& id) {
            THE_STACK_TRACE;
#if defined(THE_WINDOWS)
            return reinterpret_cast<const byte *>(&id);
#elif defined(THE_LINUX)
            return static_cast<const byte *>(id);
#else /* defined(THE_WINDOWS) */
#error "the::guid::data() must be implemented."
#endif /* defined(THE_WINDOWS) */
        }

        /** The GUID structure wrapped by this object. */
        native_guid_type id;
    };

} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_GUID_H_INCLUDED */
