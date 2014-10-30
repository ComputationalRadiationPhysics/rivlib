/*
 * the\version_number.h
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

#ifndef THE_VERSION_NUMBER_H_INCLUDED
#define THE_VERSION_NUMBER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"
#include "the/stack_trace.h"
#include "the/string.h"


namespace the {

    /**
     * This class represets a version number containing four unsigned integer,
     * representing the major version number, minor version number, build 
     * number and revision number.
     */
    class version_number {
    public:

        /** type for version number value */
        typedef unsigned short value_type;

        /**
         * Initialises a new instance.
         *
         * @param major_number The major version number.
         * @param minor_number The minor version number.
         * @param build_number The build number.
         * @param revision_number The revision number.
         */
        version_number(value_type major_number = 0,
            value_type minor_number = 0, value_type build_number = 0,
            value_type revision_number = 0);

        /** 
         * Copy ctor.
         *
         * @param src The source object to copy from
         */
        version_number(const version_number& src);

        /**
         * Ctor.
         * If the string could not be parsed successfully, the version is set
         * to 0.0.0.0
         *
         * @see version_number::parse
         *
         * @param ver The version number as string.
         */
        version_number(const char *ver);

        /**
         * Ctor.
         * If the string could not be parsed successfully, the version is set
         * to 0.0.0.0
         *
         * @see version_number::parse
         *
         * @param ver The version number as string.
         */
        version_number(const wchar_t *ver);

        /**
         * Ctor.
         * If the string could not be parsed successfully, the version is set
         * to 0.0.0.0
         *
         * @see version_number::parse
         *
         * @param ver The version number as string.
         */
        version_number(const the::astring& ver);

        /**
         * Ctor.
         * If the string could not be parsed successfully, the version is set
         * to 0.0.0.0
         *
         * @see version_number::parse
         *
         * @param ver The version number as string.
         */
        version_number(const the::wstring& ver);

        /** Dtor. */
        ~version_number(void);

        /**
         * Answer the build number.
         *
         * @return The build number.
         */
        inline value_type get_build_number(void) const {
            THE_STACK_TRACE;
            return this->build_number;
        }

        /**
         * Answer the major version number.
         *
         * @return The major version number.
         */
        inline value_type get_major(void) const {
            THE_STACK_TRACE;
            return this->major_version;
        }

        /**
         * Answer the minor version number.
         *
         * @return The minor version number.
         */
        inline value_type get_minor(void) const {
            THE_STACK_TRACE;
            return this->minor_version;
        }

        /**
         * Answer the revision number.
         *
         * @return The revision number.
         */
        inline value_type get_revision(void) const {
            THE_STACK_TRACE;
            return this->revision;
        }

        /**
         * Sets the version number to the value of a string. The string must
         * contain between one and four positive numbers separated by '.' 
         * characters. This string must not contain any white space characters.
         * However, any trailing characters behinde the last number will be
         * ignored. If not all version numbers could be successfully parsed
         * the one not set, will be set to zero.
         *
         * @param ver The version string.
         *
         * @return The number of version numbers successfully parsed from the
         *         string. Zero means that the string could not be parsed. One
         *         indicates that only the major version number is set. ...
         *         A value of four indicates that all four version numbers has
         *         been successfully parsed.
         */
        unsigned int parse(const char *ver);

        /**
         * Sets the version number to the value of a string. The string must
         * contain between one and four positive numbers separated by '.' 
         * characters. This string must not contain any white space characters.
         * However, any trailing characters behinde the last number will be
         * ignored. If not all version numbers could be successfully parsed
         * the one not set, will be set to zero.
         *
         * @param ver The version string.
         *
         * @return The number of version numbers successfully parsed from the
         *         string. Zero means that the string could not be parsed. One
         *         indicates that only the major version number is set. ...
         *         A value of four indicates that all four version numbers has
         *         been successfully parsed.
         */
        unsigned int parse(const wchar_t *ver);

        /**
         * Sets the version number to the value of a string. The string must
         * contain between one and four positive numbers separated by '.' 
         * characters. This string must not contain any white space characters.
         * However, any trailing characters behinde the last number will be
         * ignored. If not all version numbers could be successfully parsed
         * the one not set, will be set to zero.
         *
         * @param ver The version string.
         *
         * @return The number of version numbers successfully parsed from the
         *         string. Zero means that the string could not be parsed. One
         *         indicates that only the major version number is set. ...
         *         A value of four indicates that all four version numbers has
         *         been successfully parsed.
         */
        inline unsigned int parse(const the::astring& ver) {
            THE_STACK_TRACE;
            return this->parse(ver.c_str());
        }

        /**
         * Sets the version number to the value of a string. The string must
         * contain between one and four positive numbers separated by '.' 
         * characters. This string must not contain any white space characters.
         * However, any trailing characters behinde the last number will be
         * ignored. If not all version numbers could be successfully parsed
         * the one not set, will be set to zero.
         *
         * @param ver The version string.
         *
         * @return The number of version numbers successfully parsed from the
         *         string. Zero means that the string could not be parsed. One
         *         indicates that only the major version number is set. ...
         *         A value of four indicates that all four version numbers has
         *         been successfully parsed.
         */
        inline unsigned int parse(const the::wstring& ver) {
            THE_STACK_TRACE;
            return this->parse(ver.c_str());
        }

        /**
         * Sets the version numbers.
         *
         * @param major_number The major version number.
         * @param minor_number The minor version number.
         * @param build_number The build number.
         * @param revision_number The revision number.
         */
        inline void set(value_type major_number, value_type minor_number = 0,
                value_type build_number = 0, value_type revision_number = 0) {
            THE_STACK_TRACE;
            this->major_version = major_number;
            this->minor_version = minor_number;
            this->build_number = build_number;
            this->revision = revision_number;
        }

        /**
         * Sets the build number
         *
         * @param v The new value
         */
        inline void set_build_number(value_type v) {
            THE_STACK_TRACE;
            this->build_number = v;
        }

        /**
         * Sets the major version number
         *
         * @param v The new value
         */
        inline void set_major(value_type v) {
            THE_STACK_TRACE;
            this->major_version = v;
        }

        /**
         * Sets the minor version number
         *
         * @param v The new value
         */
        inline void set_minor(value_type v) {
            THE_STACK_TRACE;
            this->minor_version = v;
        }

        /**
         * Sets the revision
         *
         * @param v The new value
         */
        inline void set_revision(value_type v) {
            THE_STACK_TRACE;
            this->revision = v;
        }

        /**
         * Creates and returns a string holding the version numbers. The string
         * is created with the same format used by 'parse'.
         *
         * @see parse
         *
         * @param num The number of version number to output. A value of one 
         *            will produce a string containing only the major version
         *            number, a value of two a string with the major and minor
         *            version numbers, ... a value of four or above will 
         *            produce a string containing all four version numbers. A
         *            value of Zero will produce a string with at least the
         *            major version and will stop as soon as the remaining
         *            values all are zero.
         *
         * @return The string holding the version number.
         */
        the::astring to_astring(unsigned int num = 0) const;

        /**
         * Creates and returns a string holding the version numbers. The string
         * is created with the same format used by 'parse'.
         *
         * @see parse
         *
         * @param num The number of version number to output. A value of one 
         *            will produce a string containing only the major version
         *            number, a value of two a string with the major and minor
         *            version numbers, ... a value of four or above will 
         *            produce a string containing all four version numbers. A
         *            value of Zero will produce a string with at least the
         *            major version and will stop as soon as the remaining
         *            values all are zero.
         *
         * @return The string holding the version number.
         */
        the::wstring to_wstring(unsigned int num = 0) const;

        /**
         * assignment operator
         *
         * @param rhs The right hand side operand.
         *
         * @return Reference to this
         */
        inline version_number& operator=(const version_number& rhs) {
            THE_STACK_TRACE;
            this->major_version = rhs.major_version;
            this->minor_version = rhs.minor_version;
            this->build_number = rhs.build_number;
            this->revision = rhs.revision;
            return *this;
        }

        /**
         * Test for equality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if all four version numbers of both operands are equal.
         */
        inline bool operator==(const version_number& rhs) const {
            THE_STACK_TRACE;
            return (this->major_version == rhs.major_version) 
                && (this->minor_version == rhs.minor_version)
                && (this->build_number == rhs.build_number)
                && (this->revision == rhs.revision);
        }

        /**
         * Test for inequality.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if at least one of the version numbers of the two 
         *         operands is not equal.
         */
        inline bool operator!=(const version_number& rhs) const {
            THE_STACK_TRACE;
            return !(*this == rhs);
        }

        /**
         * Test if this version number is less than rhs.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this version numbers is less than rhs.
         */
        inline bool operator<(const version_number& rhs) const {
            THE_STACK_TRACE;
            return ((this->major_version < rhs.major_version)
                || ((this->major_version == rhs.major_version)
                && ((this->minor_version < rhs.minor_version)
                || ((this->minor_version == rhs.minor_version)
                && ((this->build_number < rhs.build_number)
                || ((this->build_number == rhs.build_number)
                && (this->revision < rhs.revision)))))));
        }

        /**
         * Test if this version number is less than or equal rhs.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this version numbers is less than or equal rhs.
         */
        inline bool operator<=(const version_number& rhs) const {
            THE_STACK_TRACE;
            return (*this == rhs) || (*this < rhs);
        }

        /**
         * Test if this version number is more than rhs.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this version numbers is more than rhs.
         */
        inline bool operator>(const version_number& rhs) const {
            THE_STACK_TRACE;
            return (!(*this == rhs)) && (!(*this < rhs));
        }

        /**
         * Test if this version number is more than or equal rhs.
         *
         * @param rhs The right hand side operand.
         *
         * @return true if this version numbers is more than or equal rhs.
         */
        inline bool operator>=(const version_number& rhs) const {
            THE_STACK_TRACE;
            return !(*this < rhs);
        }

    private:

        /** major version number */
        value_type major_version;

        /** minor version number */
        value_type minor_version;

        /** build number */
        value_type build_number;

        /** revision number */
        value_type revision;

    };

} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_VERSION_NUMBER_H_INCLUDED */
