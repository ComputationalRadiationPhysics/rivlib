/*
 * the/text/string_builder.h
 *
 * Copyright (c) 2012, TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_TEXT_STRINGBUILDER_H_INCLUDED
#define THE_TEXT_STRINGBUILDER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <cstdarg>

#include "the/assert.h"
#include "the/index_out_of_range_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/string.h"
#include "the/types.h"
#include "the/text/string_utility.h"


namespace the {
namespace text {

    /**
     * The string builder is a performance optimized string container
     * focussing on string manipulation (e.g. prepend, append, etc.)
     *
     * The template parameter is a string class.
     */
    template<class T>
    class string_builder {
    public:

        /**
         * Formats a string similar to 'printf'
         *
         * @param fmt The format string
         * @param ... The additional parameters
         *
         * @return The formatted string result
         */
        static inline T format(const T *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt->c_str(), arglist);
            va_end(arglist);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param fmt The format string
         * @param ... The additional parameters
         *
         * @return The formatted string result
         */
        static inline T format(const typename T::value_type *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt, arglist);
            va_end(arglist);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param str The string to receive the formatted string result
         * @param fmt The format string
         * @param ... The additional parameters
         *
         * @return A reference to 'str'
         */
        static inline T& format_to(T& str, const T *fmt, ...) {
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt->c_str(), arglist);
            va_end(arglist);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param str The string to receive the formatted string result
         * @param fmt The format string
         * @param ... The additional parameters
         *
         * @return A reference to 'str'
         */
        static inline T& format_to(T& str,
                const typename T::value_type *fmt, ...) {
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt, arglist);
            va_end(arglist);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         *
         * @return The formatted string result
         */
        static inline T formatVa(const T& fmt, va_list argptr) {
            T str;
            formatVa_to(str, fmt.c_str(), argptr);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         *
         * @return The formatted string result
         */
        static inline T formatVa(const typename T::value_type *fmt,
                va_list argptr) {
            T str;
            formatVa_to(str, fmt, argptr);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param str The string to receive the formatted string result
         * @param fmt The format string
         * @param argPtr The variable parameters list
         *
         * @return A reference to 'str'
         */
        static inline T& formatVa_to(T& str, const T& fmt, va_list argptr) {
            formatVa_to(str, fmt.c_str(), argptr);
            return str;
        }

        /**
         * Formats a string similar to 'printf'
         *
         * @param str The string to receive the formatted string result
         * @param fmt The format string
         * @param argPtr The variable parameters list
         *
         * @return A reference to 'str'
         */
        static T& formatVa_to(T& str, const typename T::value_type *fmt,
            va_list argptr);

        /**
         * Ctor
         */
        string_builder(void);

        /**
         * Copy Ctor
         *
         * @param src The object to copy from
         */
        string_builder(const string_builder& src);

        /**
         * Ctor
         *
         * @param str The string to initialise the builder
         */
        string_builder(const T& str);

        /**
         * Ctor
         *
         * @param c The character to initialise the builder
         * @param cnt The number of characters to initialise the buffer
         */
        string_builder(const typename T::value_type c, size_t cnt = 1);

        /**
         * Ctor
         *
         * @param str The string to initialise the builder
         */
        string_builder(const typename T::value_type *str);

        /**
         * Ctor
         *
         * @param str The string to initialise the builder
         * @param len The length of 'str' in characters
         */
        string_builder(const typename T::value_type *str, size_t len);

        /**
         * Dtor
         */
        ~string_builder(void);

        /**
         * Appends the content of 'src' to this builder
         *
         * @param src The source object
         */
        void append(const string_builder& src);

        /**
         * Appends 'str' to this builder
         *
         * @param str The string to append
         */
        inline void append(const T& str) {
            this->append(str.c_str(), str.length());
        }

        /**
         * Appends 'c' to this builder
         *
         * @param c The character to append
         * @param cnt The number of characters to append
         */
        inline void append(const typename T::value_type c, size_t cnt = 1) {
            T str(cnt, c);
            this->append(str.c_str(), str.length());
        }

        /**
         * Appends 'str' to this builder
         *
         * @param str The string to append
         */
        inline void append(const typename T::value_type *str) {
            this->append(str, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(str)));
        }

        /**
         * Appends 'str' to this builder
         *
         * @param str The string to append
         * @param len The length of 'str'
         */
        void append(const typename T::value_type *str, size_t len);

        /**
         * Appends a formatted string to this builder, following printf syntax
         *
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void append_formatted(const T *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt->c_str(), arglist);
            va_end(arglist);
            this->append(str.c_str(), str.length());
        }

        /**
         * Appends a formatted string to this builder, following printf syntax
         *
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void append_formatted(const typename T::value_type *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt, arglist);
            va_end(arglist);
            this->append(str.c_str(), str.length());
        }

        /**
         * Appends a formatted string to this builder, following printf syntax
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void append_formattedVa(const T& fmt, va_list argptr) {
            T str;
            formatVa_to(str, fmt.c_str(), argptr);
            this->append(str.c_str(), str.length());
        }

        /**
         * Appends a formatted string to this builder, following printf syntax
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void append_formattedVa(const typename T::value_type *fmt,
                va_list argptr) {
            T str;
            formatVa_to(str, fmt, argptr);
            this->append(str.c_str(), str.length());
        }

        /**
         * Clears this builder
         */
        void clear(void);

        /**
         * Inserts the content of 'src' into this builder at position 'pos'
         *
         * @param pos The position to insert at
         * @param src The object holding the content to insert
         */
        void insert(size_t pos, const string_builder& src);

        /**
         * Inserts 'str' into this builder at position 'pos'
         *
         * @param pos The position to insert at
         * @param str The string to insert
         */
        inline void insert(size_t pos, const T& str) {
            this->insert(pos, str.c_str(), str.length());
        }

        /**
         * Inserts 'c' characters into this builder at position 'pos'
         *
         * @param pos The position to insert at
         * @param c The character to insert
         * @param cnt The number of characters to insert
         */
        inline void insert(size_t pos, const typename T::value_type c,
                size_t cnt = 1) {
            T str(cnt, c);
            this->insert(pos, str.c_str(), str.length());
        }

        /**
         * Inserts 'str' into this builder at position 'pos'
         *
         * @param pos The position to insert at
         * @param str The string to insert
         */
        inline void insert(size_t pos, const typename T::value_type *str) {
            this->insert(pos, str, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(str)));
        }

        /**
         * Inserts 'str' into this builder at position 'pos'
         *
         * @param pos The position to insert at
         * @param str The string to insert
         * @param len The length of 'str' in characters
         */
        void insert(size_t pos, const typename T::value_type *str,
            size_t len);

        /**
         * Inserts a formatted string to this builder at position 'pos',
         * following printf syntax
         *
         * @param pos The position to insert at
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void insert_formatted(size_t pos, const T *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt->c_str(), arglist);
            va_end(arglist);
            this->insert(pos, str.c_str(), str.length());
        }

        /**
         * Inserts a formatted string to this builder at position 'pos',
         * following printf syntax
         *
         * @param pos The position to insert at
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void insert_formatted(size_t pos,
                const typename T::value_type *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt, arglist);
            va_end(arglist);
            this->insert(pos, str.c_str(), str.length());
        }

        /**
         * Inserts a formatted string to this builder at position 'pos',
         * following printf syntax
         *
         * @param pos The position to insert at
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void insert_formattedVa(size_t pos, const T& fmt,
                va_list argptr) {
            T str;
            formatVa_to(str, fmt.c_str(), argptr);
            this->insert(pos, str.c_str(), str.length());
        }

        /**
         * Inserts a formatted string to this builder at position 'pos',
         * following printf syntax
         *
         * @param pos The position to insert at
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void insert_formattedVa(size_t pos,
                const typename T::value_type *fmt, va_list argptr) {
            T str;
            formatVa_to(str, fmt, argptr);
            this->insert(pos, str.c_str(), str.length());
        }

        /**
         * Answer if this builder is empty
         *
         * @return True if this builder is empty
         */
        bool is_empty(void) const;

        /**
         * finds the first occurance of 'c' in this builder after position
         * 'beginningAt'
         *
         * @param c The content to search for
         * @param beginningAt searches from this position
         *
         * @return The found position or '-1' is not found
         */
        size_t find(const typename T::value_type c, size_t beginningAt = 0);

        /**
         * finds the first occurance of 'str' in this builder after position
         * 'beginningAt'
         *
         * @param str The content to search for
         * @param beginningAt searches from this position
         *
         * @return The found position or '-1' is not found
         */
        inline size_t find(const typename T::value_type *str,
                size_t beginningAt = 0) {
            return find(str, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(str)),
                beginningAt);
        }

        /**
         * finds the first occurance of 'str' in this builder after position
         * 'beginningAt'
         *
         * @param str The content to search for
         * @param strLen The length of 'str'
         * @param beginningAt searches from this position
         *
         * @return The found position or '-1' is not found
         */
        size_t find(const typename T::value_type *str, size_t strLen,
            size_t beginningAt);

        /**
         * finds the first occurance of 'str' in this builder after position
         * 'beginningAt'
         *
         * @param str The content to search for
         * @param beginningAt searches from this position
         *
         * @return The found position or '-1' is not found
         */
        inline size_t find(const T& str, size_t beginningAt = 0) {
            return this->find(str.c_str(), beginningAt);
        }

        /**
         * Answer the length of the content of this builder in number of
         * characters
         *
         * @return The length of the content of this builder in number of
         *         characters
         */
        size_t length(void) const;

        /**
         * prepends the content of 'src' to this builder
         *
         * @param src The source object
         */
        void prepend(const string_builder& src);

        /**
         * prepends 'str' to this builder
         *
         * @param str The string to prepend
         */
        inline void prepend(const T& str) {
            this->prepend(str.c_str(), str.length());
        }

        /**
         * prepends 'c' to this builder
         *
         * @param c The character to prepend
         * @param cnt The number of characters to prepend
         */
        inline void prepend(const typename T::value_type c, size_t cnt = 1) {
            T str(cnt, c);
            this->prepend(str.c_str(), str.length());
        }

        /**
         * prepends 'str' to this builder
         *
         * @param str The string to prepend
         */
        inline void prepend(const typename T::value_type *str) {
            this->prepend(str, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(str)));
        }

        /**
         * prepends 'str' to this builder
         *
         * @param str The string to prepend
         * @param len The length of 'str' in characters
         */
        void prepend(const typename T::value_type *str, size_t len);

        /**
         * prepends a formatted string to this builder, following printf
         * syntax
         *
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void prepend_formatted(const T *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt->c_str(), arglist);
            va_end(arglist);
            this->prepend(str.c_str(), str.length());
        }

        /**
         * prepends a formatted string to this builder, following printf
         * syntax
         *
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void prepend_formatted(const typename T::value_type *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt, arglist);
            va_end(arglist);
            this->prepend(str.c_str(), str.length());
        }

        /**
         * prepends a formatted string to this builder, following printf
         * syntax
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void prepend_formattedVa(const T& fmt, va_list argptr) {
            T str;
            formatVa_to(str, fmt.c_str(), argptr);
            this->prepend(str.c_str(), str.length());
        }

        /**
         * prepends a formatted string to this builder, following printf
         * syntax
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void prepend_formattedVa(const typename T::value_type *fmt,
                va_list argptr) {
            T str;
            formatVa_to(str, fmt, argptr);
            this->prepend(str.c_str(), str.length());
        }

        /**
         * removes all content from this builder following position 'pos'
         *
         * @param pos The position to remove from
         */
        inline void remove(size_t pos) {
            this->remove(pos, this->length() - pos);
        }

        /**
         * removes a portion of the content of this string builder
         *
         * @param pos The position to remove from
         * @param len The number of characters to remove
         */
        void remove(size_t pos, size_t len);

        /**
         * replaces occurances of 'oldChar' by 'newChar'
         *
         * @param oldChar The old character value
         * @param newChar The new character value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        size_t replace(const typename T::value_type oldChar,
            const typename T::value_type newChar, size_t limit = 0);

        /**
         * replaces occurances of 'oldChar' by 'newStr'
         *
         * @param oldChar The old character value
         * @param newStr The new string value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const typename T::value_type oldChar,
                const typename T::value_type *newStr, size_t limit = 0) {
            typename T::value_type os[2];
            os[0] = oldChar;
            os[1] = static_cast<typename T::value_type>(0);
            return this->replace(os, 1, newStr, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(newStr)),
                limit);
        }

        /**
         * replaces occurances of 'oldChar' by 'newStr'
         *
         * @param oldChar The old character value
         * @param newStr The new string value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const typename T::value_type oldChar,
                const T& newStr, size_t limit = 0) {
            typename T::value_type os[2];
            os[0] = oldChar;
            os[1] = static_cast<typename T::value_type>(0);
            return this->replace(os, 1, newStr.c_str(), newStr.length(),
                limit);
        }

        /**
         * replaces occurances of 'oldStr' by 'newChar'
         *
         * @param oldStr The old string value
         * @param newChar The new character value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const typename T::value_type *oldStr,
                const typename T::value_type newChar, size_t limit = 0) {
            typename T::value_type ns[2];
            ns[0] = newChar;
            ns[1] = static_cast<typename T::value_type>(0);
            return this->replace(oldStr, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(oldStr)),
                ns, 1, limit);
        }

        /**
         * replaces occurances of 'oldStr' by 'newStr'
         *
         * @param oldStr The old string value
         * @param newStr The new string value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const typename T::value_type *oldStr,
                const typename T::value_type *newStr, size_t limit = 0) {
            return this->replace(oldStr, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(oldStr)),
                newStr, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(newStr)),
                limit);
        }

        /**
         * replaces occurances of 'oldStr' by 'newStr'
         *
         * @param oldStr The old string value
         * @param oldStrLen The length of 'oldStr' in number of characters
         * @param newStr The new string value
         * @param newStrLen The length of 'newStr' in number of characters
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        size_t replace(const typename T::value_type *oldStr, size_t oldStrLen,
            const typename T::value_type *newStr, size_t newStrLen,
            size_t limit = 0);

        /**
         * replaces occurances of 'oldStr' by 'newStr'
         *
         * @param oldStr The old string value
         * @param newStr The new string value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const typename T::value_type *oldStr,
                const T& newStr, size_t limit = 0) {
            return this->replace(oldStr, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(oldStr)),
                newStr.c_str(), newStr.length(), limit);
        }

        /**
         * replaces occurances of 'oldStr' by 'newChar'
         *
         * @param oldStr The old string value
         * @param newChar The new character value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const T& oldStr,
                const typename T::value_type newChar, size_t limit = 0) {
            typename T::value_type ns[2];
            ns[0] = newChar;
            ns[1] = static_cast<typename T::value_type>(0);
            return this->replace(oldStr.c_str(), oldStr.length(),
                ns, 1, limit);
        }

        /**
         * replaces occurances of 'oldStr' by 'newStr'
         *
         * @param oldStr The old string value
         * @param newStr The new string value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const T& oldStr,
                const typename T::value_type *newStr, size_t limit = 0) {
            return this->replace(oldStr.c_str(), oldStr.length(),
                newStr, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(newStr)),
                limit);
        }

        /**
         * replaces occurances of 'oldStr' by 'newStr'
         *
         * @param oldStr The old string value
         * @param newStr The new string value
         * @param limit The maximum number of replacements to perform, or zero
         *              if all occurances should be replaced
         *
         * @return The number of performed replaces
         */
        inline size_t replace(const T& oldStr, const T& newStr,
                size_t limit = 0) {
            return this->replace(oldStr.c_str(), oldStr.length(),
                newStr.c_str(), newStr.length(), limit);
        }

        /**
         * Sets the content of this builder to the content of 'src' making a
         * deep copy
         *
         * @param src The object to copy from
         */
        void set(const string_builder& src);

        /**
         * Sets the content of this builder to 'str' making a deep copy
         *
         * @param str The string to copy from
         */
        inline void set(const T& str) {
            this->set(str.c_str(), str.length());
        }

        /**
         * Sets the content of this builder to 'c'
         *
         * @param c The charater value to set
         * @param cnt The number of characters to set
         */
        inline void set(const typename T::value_type c, size_t cnt = 1) {
            T str(cnt, c);
            this->set(str.c_str(), str.length());
        }

        /**
         * Sets the content of this builder to 'str' making a deep copy
         *
         * @param str The string to copy from
         */
        inline void set(const typename T::value_type *str) {
            this->set(str, static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(str)));
        }

        /**
         * Sets the content of this builder to 'str' making a deep copy
         *
         * @param str The string to copy from
         * @param len The length of 'str' in characters
         */
        void set(const typename T::value_type *str, size_t len);

        /**
         * Sets the content of this builder to a formatted string similar to
         * printf
         *
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void set_formatted(const T *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt->c_str(), arglist);
            va_end(arglist);
            this->set(str.c_str(), str.length());
        }

        /**
         * Sets the content of this builder to a formatted string similar to
         * printf
         *
         * @param fmt The format string
         * @param ... The additional parameters
         */
        inline void set_formatted(const typename T::value_type *fmt, ...) {
            T str;
            va_list arglist;
            va_start(arglist, fmt);
            formatVa_to(str, fmt, arglist);
            va_end(arglist);
            this->set(str.c_str(), str.length());
        }

        /**
         * Sets the content of this builder to a formatted string similar to
         * printf
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        inline void set_formattedVa(const T& fmt, va_list argptr) {
            T str;
            formatVa_to(str, fmt.c_str(), argptr);
            this->set(str.c_str(), str.length());
        }

        /**
         * Sets the content of this builder to a formatted string similar to
         * printf
         *
         * @param fmt The format string
         * @param argPtr The variable parameters list
         */
        void set_formattedVa(const typename T::value_type *fmt,
                va_list argptr) {
            T str;
            formatVa_to(str, fmt, argptr);
            this->set(str.c_str(), str.length());
        }

        /**
         * Converts the content of this builder to a linear string making a
         * deep copy
         *
         * @return A string holding a copy of the content of this builder
         */
        inline T to_string(void) const {
            T str;
            this->to_string(str);
            return str;
        }

        /**
         * Copies the content of this builder to the linear string 'str'
         *
         * @param str The string to receive the copy of the content of this
         *            builder
         *
         * @return A reference to 'str'
         */
        T& to_string(T& str) const;

        /**
         * remove all characters that are in the string 'chars' from the start 
         * of this builder. 'chars' must be zero-terminated.
         *
         * @param chars A string of characters to be removed from the begin of 
         *              this string.
         */
        void trim_begin(const typename T::value_type *chars);

        /**
         * remove all characters that are in the string 'chars' from the end 
         * of this builder. 'chars' must be zero-terminated.
         *
         * @param chars A string of characters to be removed from the begin of 
         *              this string.
         */
        void trim_end(const typename T::value_type *chars);

        /**
         * remove all characters that are in the string 'chars' from the start 
         * and end of this builder. 'chars' must be zero-terminated.
         *
         * @param chars A string of characters to be removed from the begin of 
         *              this string.
         */
        inline void trim(const typename T::value_type *chars) {
            // not optimal but ok
            this->trim_begin(chars);
            this->trim_end(chars);
        }

        /**
         * remove all whitespace characters from the start of this builder.
         */
        void trim_begin(void);

        /**
         * remove all whitespace characters from the end of this builder.
         */
        void trim_end(void);

        /**
         * remove all whitespace characters from the start and end of this
         * builder.
         */
        inline void trim(void) {
            // not optimal but ok
            this->trim_begin();
            this->trim_end();
        }

        /**
         * Answer the 'idt'-th character of the content of this builder
         *
         * @param idx The zero-based index of the character to return
         *
         * @return The requested character
         *
         * @throw index_out_of_range_exception for illegal values of 'idx'
         */
        const typename T::value_type& operator[](size_t idx);

        /**
         * Assignment operator, making a deep copy
         *
         * @param rhs The right hand operand, to be set
         *
         * @return A reference to this
         */
        inline string_builder& operator=(const string_builder& rhs) {
            this->set(rhs);
            return *this;
        }

        /**
         * Assignment operator, making a deep copy
         *
         * @param rhs The right hand operand, to be set
         *
         * @return A reference to this
         */
        inline string_builder& operator=(const T& rhs) {
            this->set(rhs);
            return *this;
        }

        /**
         * Assignment operator
         *
         * @param rhs The right hand operand, to be set
         *
         * @return A reference to this
         */
        inline string_builder& operator=(const typename T::value_type rhs) {
            this->set(rhs);
            return *this;
        }

        /**
         * Assignment operator, making a deep copy
         *
         * @param rhs The right hand operand, to be set
         *
         * @return A reference to this
         */
        inline string_builder& operator=(const typename T::value_type *rhs) {
            this->set(rhs);
            return *this;
        }

        /**
         * append operator, making a deep copy
         *
         * @param rhs The right hand operand, to be appended
         *
         * @return A reference to this
         */
        inline string_builder& operator+=(const string_builder& rhs) {
            this->append(rhs);
            return *this;
        }

        /**
         * append operator, making a deep copy
         *
         * @param rhs The right hand operand, to be appended
         *
         * @return A reference to this
         */
        inline string_builder& operator+=(const T& rhs) {
            this->append(rhs);
            return *this;
        }

        /**
         * append operator
         *
         * @param rhs The right hand operand, to be appended
         *
         * @return A reference to this
         */
        inline string_builder& operator+=(const typename T::value_type rhs) {
            this->append(rhs);
            return *this;
        }

        /**
         * append operator, making a deep copy
         *
         * @param rhs The right hand operand, to be appended
         *
         * @return A reference to this
         */
        inline string_builder& operator+=(const typename T::value_type *rhs) {
            this->append(rhs);
            return *this;
        }

        /**
         * Test for equality
         *
         * @param rhs The right hand operand
         *
         * @return True if 'this' is equal to 'rhs'
         */
        bool operator==(const string_builder& rhs) const;

        /**
         * Test for inequality
         *
         * @param rhs The right hand operand
         *
         * @return True if 'this' is not equal to 'rhs'
         */
        inline bool operator!=(const string_builder& rhs) const {
            return !this->operator==(rhs);
        }

    private:

        /**
         * "Rope" binary tree node class
         */
        class rope_node {
        public:

            /**
             * Concatenates two ropes
             *
             * @param l the left rope
             * @param r the right rope
             * @param rebalance if true the returned rope will be balanced
             *
             * @return The resulting rope
             */
            static rope_node *concat(rope_node *l, rope_node *r,
                    bool rebalance = true) {
                // This most likely results in an unbalanced rope
                // better implement the 'reverse' split
                rope_node *n = new rope_node();
                n->left = l;
                n->right = r;
                n->leftLen = (l == NULL) ? 0 : l->length();
                if (rebalance) n = balance(n);
                return n;
            }

            /**
             * Splits a rope
             *
             * @param n the input rope
             * @param i the split index
             * @param l receives the new left rope
             * @param r receives the new right rope
             * @param rebalance if true the returned ropes will be balanced
             */
            static void split(rope_node *& n, size_t i, rope_node *& l,
                    rope_node *& r, bool rebalance = true) {
                ASSERT(i >= 0);
                ASSERT(n != NULL);
                l = r = NULL;

                if (i == 0) {
                    l = NULL;
                    r = n;
                    n = NULL;
                    return;
                }

                // This is an recursive implementation because I did not
                // understand the explanation on wikipedia
                if (n->leftLen == i) {
                    if (n->left == NULL) {
                        l = n;
                        r = n->right;
                        n->right = NULL;
                        n = NULL;
                    } else {
                        l = n->left;
                        n->left = NULL;
                        r = n->right;
                        n->right = NULL;
                        safe_delete(n);
                    }

                } else if (n->leftLen > i) {
                    // split in left subtree
                    if (n->left == NULL) {
                        // split in string
                        l = new rope_node(n->str.substr(0, i));
                        n->str = n->str.substr(i);
                        n->leftLen = n->str.length();
                        r = n;
                        n = NULL;

                    } else {
                        rope_node *ll, *lr;
                        split(n->left, i, ll, lr, false);
                        n->left = lr;
                        n->leftLen = lr->length();
                        l = ll;
                        r = n;
                        n = NULL;
                    }

                } else {
                    // split in right
                    ASSERT(i > n->leftLen);
                    if (n->right == NULL) {
                        throw index_out_of_range_exception(0, 0, 0, "", 0);
                    }
                    rope_node *rl, *rr;
                    split(n->right, i - n->leftLen, rl, rr, false);
                    n->right = rl;
                    r = rr;
                    l = n;
                    n = NULL;
                }

                if (rebalance) {
                    l = balance(l);
                    r = balance(r);
                }

            }

            /**
             * Rebuild a rope to make it balanced
             *
             * @param n The input rope
             *
             * @return The balanced output rope
             */
            static rope_node *balance(rope_node *n) {
                // Trivial implementation:
                //  First linearlize all nodes
                //  then group to build hierarchy
                rope_node *l = NULL, *d = NULL;
                make_linear(l, d, n);
                while (l->right != NULL) {
                    l = make_tree(l);
                }
                return l;
            }


            /** Ctor */
            rope_node(void) : left(NULL), right(NULL), leftLen(0), str() {
                // intentionally empty
            }

            /** Ctor */
            rope_node(const T& str) : left(NULL), right(NULL),
                    leftLen(str.length()), str(str) {
                // intentionally empty
            }

            /** Ctor */
            rope_node(const typename T::value_type *str, size_t len)
                    : left(NULL), right(NULL), leftLen(len), str(str, len) {
                // intentionally empty
            }

            /** Dtor */
            ~rope_node(void) {
                safe_delete(this->left);
                safe_delete(this->right);
                this->leftLen = 0;
                this->str.clear();
            }

            /**
             * Clones this rope
             *
             * @return A clone of this rope
             */
            rope_node* clone(void) const {
                rope_node *r = new rope_node(this->str);
                r->left = (this->left == NULL) ? NULL : this->left->clone();
                r->right = (this->right == NULL) ? NULL
                    : this->right->clone();
                r->leftLen = this->leftLen;
                return r;
            }

            /**
             * finds the first occurance of 'c' after 'beginningAt'
             *
             * @param c The character to search for
             * @param beginningAt The search start position
             *
             * @return The position of the first occurance of c, or -1 if c
             *         was not found.
             */
            size_t find(const typename T::value_type c, size_t beginningAt) {
                // either left tree or string, never both
                ASSERT((this->left == NULL) || (this->str.empty()));

                if (beginningAt < this->leftLen) {
                    // also search left
                    if (this->left != NULL) {
                        size_t f = this->left->find(c, beginningAt);
                        if (f != -1) return f;
                    } else {
                        // search in string
                        size_t f = this->str.find(c, beginningAt);
                        if (f != T::npos) return f;
                    }
                }
                if (this->right != NULL) {
                    size_t f = this->right->find(c,
                        (beginningAt <= this->leftLen) ? 0 
                        : (beginningAt - this->leftLen));
                    if (f != -1) return f + this->leftLen;
                }
                return -1;

            }

            /**
             * Answer the 'idx'-th character of the rope
             *
             * @param idx The zero-based character to return
             *
             * @return The character
             *
             * @throws index_out_of_range_exception if idx is invalid
             */
            const typename T::value_type& index(size_t idx) const {
                // either left tree or string, never both
                ASSERT((this->left == NULL) || (this->str.empty()));
                const rope_node *r = this;
                while (r != NULL) {
                    if (idx < r->leftLen) {
                        if (r->left != NULL) {
                            r = r->left;
                        } else {
                            return r->str[idx];
                        }
                    } else {
                        idx -= r->leftLen;
                        r = r->right;
                    }
                }
                throw index_out_of_range_exception(idx, 0, this->length(),
                    __FILE__, __LINE__);
            }

            /**
             * Answer if this rope is empty
             *
             * @return True if this rope is empty
             */
            bool is_empty(void) const {
                return (this->leftLen == 0) && (
                    (this->right == NULL) || (this->right->is_empty()));
            }

            /**
             * Answer the length of the rope
             *
             * @return The length of the rope
             */
            size_t length(void) const {
                size_t l = this->leftLen;
                const rope_node *r = this->right;
                while (r != NULL) {
                    l += r->leftLen;
                    r = r->right;
                }
                return l;
            }

            /**
             * Reports (copies) the content of the rope to 'dst'
             *
             * @param dst The memory to receive the report copy
             */
            void report(typename T::value_type* dst) const {
                // either left tree or string, never both
                const rope_node *n = this;
                while (n != NULL) {
                    ASSERT((n->left == NULL) || (n->str.empty()));
                    if (n->left != NULL) {
                        n->left->report(dst);
                    } else {
                        ::memcpy(dst, n->str.c_str(),
                            n->leftLen * sizeof(typename T::value_type));
                    }
                    dst += n->leftLen;
                    n = n->right;
                }
            }

        private:

            /**
             * Converts the binary tree to a linear list only using the right
             * pointers
             *
             * @param f The first node of the list
             * @param l The last node of the list
             * @param n The current node of the tree
             */
            static void make_linear(rope_node *& f, rope_node *& l, rope_node *n) {
                ASSERT(n != NULL);
                rope_node *nl = n->left;
                rope_node *nr = n->right;
                n->left = NULL;
                n->right = NULL;
                if (n->str.empty()) {
                    delete n;
                } else {
                    n->leftLen = n->str.length();
                    if (f == NULL) {
                        f = l = n;
                    } else {
                        l->right = n;
                        l = n;
                    }
                }
                if (nl != NULL) make_linear(f, l, nl);
                if (nr != NULL) make_linear(f, l, nr);
            }

            /**
             * Constructs a binary tree from a list (using right points)
             *
             * @param list the 'right pointers' list
             *
             * @param the less flat right pointers list
             */
            static rope_node *make_tree(rope_node *list) {
                rope_node *f = NULL, *l = NULL, *t = NULL;

                while (list != NULL) {
                    if (f == NULL) {
                        f = l = new rope_node();
                    } else {
                        l->right = new rope_node();
                        l = l->right;
                    }
                    l->left = list;
                    t = list->right;
                    if (t == NULL) {
                        l->leftLen = l->left->length();
                        break;
                    }
                    list = t->right;
                    t->right = NULL;
                    l->leftLen = l->left->length();
                }

                return f;
            }

            /** The left subtree */
            rope_node *left;

            /** The right subtree */
            rope_node *right;

            /** The size of the left subtree and 'str' */
            size_t leftLen;

            /** The string stored in this tree */
            T str;

        };

        /** The root rope node */
        rope_node *root;

    };


    /*
     * string_builder::formatVa_to
     */
    template <class T>
    T& string_builder<T>::formatVa_to(T& str, const typename T::value_type *fmt,
            va_list argptr) {
        if (fmt == NULL) {
            str.clear();
        } else {            
            size_t len = the::text::string_utility::formatVa(
                NULL, 0, fmt, argptr);
            if (len <= 0) {
                str.clear();
            } else {
                str.resize(len, 0);
                the::text::string_utility::formatVa(
                    const_cast<typename T::value_type*>(str.c_str()),
                    len + 1, fmt, argptr);
            }
        }
        return str;
    }


    /*
     * string_builder::string_builder
     */
    template <class T>
    string_builder<T>::string_builder(void) : root(NULL) {
        // intentionally empty
    }


    /*
     * string_builder::string_builder
     */
    template <class T>
    string_builder<T>::string_builder(const string_builder<T>& src)
            : root(NULL) {
        this->set(src);
    }


    /*
     * string_builder::string_builder
     */
    template <class T>
    string_builder<T>::string_builder(const T& str) : root(NULL) {
        this->set(str.c_str(), str.length());
    }


    /*
     * string_builder::string_builder
     */
    template <class T>
    string_builder<T>::string_builder(const typename T::value_type c,
            size_t cnt) : root(NULL) {
        T str(cnt, c);
        this->set(str.c_str(), str.length());
    }


    /*
     * string_builder::string_builder
     */
    template <class T>
    string_builder<T>::string_builder(const typename T::value_type *str)
            : root(NULL) {
        this->set(str, static_cast<size_t>(
            std::char_traits<typename T::value_type>::length(str)));
    }


    /*
     * string_builder::string_builder
     */
    template <class T>
    string_builder<T>::string_builder(const typename T::value_type *str,
            size_t len) : root(NULL) {
        this->set(str, len);
    }


    /*
     * string_builder::~string_builder
     */
    template <class T>
    string_builder<T>::~string_builder(void) {
        safe_delete(this->root);
    }


    /*
     * string_builder::append
     */
    template <class T>
    void string_builder<T>::append(const string_builder<T>& src) {
        if (src.is_empty()) return;
        this->root = rope_node::concat(this->root, src.root->clone());
    }


    /*
     * string_builder::append
     */
    template <class T>
    void string_builder<T>::append(const typename T::value_type *str,
            size_t len) {
        this->root = rope_node::concat(this->root, new rope_node(str, len));
    }


    /*
     * string_builder::clear
     */
    template <class T>
    void string_builder<T>::clear(void) {
        safe_delete(this->root);
    }


    /*
     * string_builder::insert
     */
    template <class T>
    void string_builder<T>::insert(size_t pos, const string_builder<T>& src) {
        ASSERT(pos >= 0);
        if (src.is_empty()) return;
        if (this->root == NULL) {
            if (pos == 0) {
                this->set(src);
            } else { 
                throw index_out_of_range_exception(pos, 0, 0,
                    __FILE__, __LINE__);
            }
        } else {
            try {
                rope_node *l, *r;
                rope_node::split(this->root, pos, l, r, false);
                l = rope_node::concat(l, src.root->clone(), false);
                this->root = rope_node::concat(l, r);
            } catch(index_out_of_range_exception) {
                throw index_out_of_range_exception(pos, 0, this->length(),
                    __FILE__, __LINE__);
            }
        }
    }


    /*
     * string_builder::insert
     */
    template <class T>
    void string_builder<T>::insert(size_t pos,
            const typename T::value_type *str, size_t len) {
        ASSERT(pos >= 0);
        if ((str == NULL) || (len <= 0)) return;
        if (this->root == NULL) {
            if (pos == 0) {
                this->set(str, len);
            } else { 
                throw index_out_of_range_exception(pos, 0, 0, __FILE__, __LINE__);
            }
        } else {
            try {
                rope_node *l, *r;
                rope_node::split(this->root, pos, l, r, false);
                l = rope_node::concat(l, new rope_node(str, len), false);
                this->root = rope_node::concat(l, r);
            } catch(index_out_of_range_exception) {
                throw index_out_of_range_exception(pos, 0, this->length(),
                    __FILE__, __LINE__);
            }
        }
    }


    /*
     * string_builder::is_empty
     */
    template <class T>
    bool string_builder<T>::is_empty(void) const {
        return (this->root == NULL)
            || this->root->is_empty();
    }


    /*
     * string_builder::find
     */
    template <class T>
    size_t string_builder<T>::find(const typename T::value_type c,
            size_t beginningAt) {
        return (this->root == NULL) ? static_cast<size_t>(-1)
            : this->root->find(c, beginningAt);
    }


    /*
     * string_builder::find
     */
    template <class T>
    size_t string_builder<T>::find(const typename T::value_type *str,
            size_t strLen, size_t beginningAt) {
        if ((str == NULL) || (strlen <= 0)) return static_cast<size_t>(-1);
        if (this->is_empty()) return static_cast<size_t>(-1);

        // naive implementation an uses 'Index' O(log n) to iterate
        //  'strlen' times.

        size_t found;
        do {
            found = this->root->find(str[0], beginningAt);
            if (found != static_cast<size_t>(-1)) {
                // first char found, not test the rest
                bool all = true;
                for (size_t i = 1; i < strLen; i++) {
                    if (this->root->index(found + i) != str[i]) {
                        all = false;
                        break;
                    }
                }
                if (all) return found;
                beginningAt = found + 1;
            }
        } while (found != static_cast<size_t>(-1));

        return static_cast<size_t>(-1);
    }


    /*
     * string_builder::length
     */
    template <class T>
    size_t string_builder<T>::length(void) const {
        return (this->root == NULL) ? 0 : this->root->length();
    }


    /*
     * string_builder::prepend
     */
    template <class T>
    void string_builder<T>::prepend(const string_builder<T>& src) {
        if (src.is_empty()) return;
        this->root = rope_node::concat(src.root->clone(), this->root);
    }


    /*
     * string_builder::prepend
     */
    template <class T>
    void string_builder<T>::prepend(const typename T::value_type *str,
            size_t len) {
        if ((str == NULL) || (len <= 0)) return;
        this->root = rope_node::concat(new rope_node(str, len), this->root);
    }


    /*
     * string_builder::remove
     */
    template <class T>
    void string_builder<T>::remove(size_t pos, size_t len) {
        rope_node *l, *r, *rr;

        try {
            rope_node::split(this->root, pos, l, r, false);
        } catch(index_out_of_range_exception) {
            // pos not in range, so fail silently
            return;
        }

        this->root = l;

        try {
            rope_node::split(r, len, l, rr, false);
        } catch(index_out_of_range_exception) {
            // len not in range, so delete right
            delete r;
            return;
        }
        delete l;

        this->root = rope_node::concat(this->root, rr);
    }


    /*
     * string_builder::replace
     */
    template <class T>
    size_t string_builder<T>::replace(const typename T::value_type oldChar,
            const typename T::value_type newChar, size_t limit) {
        if (this->is_empty()) return 0;

        size_t cnt = 0;
        size_t i = 0;
        do {
            i = this->root->find(oldChar, i);
            if (i != static_cast<size_t>(-1)) {
                const_cast<typename T::value_type&>(
                    this->root->index(i)) = newChar;
                ++cnt;
                ++i;
                if (limit > 0) {
                    --limit;
                    if (limit == 0) i = static_cast<size_t>(-1);
                }
            }
        } while (i != static_cast<size_t>(-1));

        return cnt;
    }


    /*
     * string_builder::replace
     */
    template <class T>
    size_t string_builder<T>::replace(const typename T::value_type *oldStr,
            size_t oldStrLen, const typename T::value_type *newStr,
            size_t newStrLen, size_t limit) {
        if (this->is_empty()) return 0;
        if ((oldStr == NULL) || (oldStrLen == 0)) return 0;

        // super naive implementation!
        size_t cnt = 0;
        size_t i = 0;
        do {
            i = this->find(oldStr, oldStrLen, i);
            if (i != static_cast<size_t>(-1)) {
                rope_node *l, *r, *rr;
                rope_node::split(this->root, i, l, r, false);
                this->root = rope_node::concat(l,
                    new rope_node(newStr, newStrLen), false);
                rope_node::split(r, oldStrLen, l, rr, false);
                safe_delete(l);
                this->root = rope_node::concat(this->root, rr, false);
                ++cnt;
                if (limit > 0) {
                    --limit;
                    if (limit == 0) i = static_cast<size_t>(-1);
                }
            }
        } while (i != static_cast<size_t>(-1));

        if (cnt > 0) {
            this->root = rope_node::balance(this->root);
        }

        return cnt;
    }


    /*
     * string_builder::set
     */
    template <class T>
    void string_builder<T>::set(const string_builder<T>& src) {
        if (this == &src) return;
        delete this->root;
        this->root = src.root->clone();
    }


    /*
     * string_builder::set
     */
    template <class T>
    void string_builder<T>::set(const typename T::value_type *str,
            size_t len) {
        delete this->root;
        this->root = new rope_node(str, len);
    }


    /*
     * string_builder::to_string
     */
    template <class T>
    T& string_builder<T>::to_string(T& str) const {
        if (this->is_empty()) {
            str.clear();
            return str;
        }
        ASSERT(this->root != NULL);

        size_t len = this->root->length();
        str.resize(len, 0);
        this->root->report(const_cast<typename T::value_type*>(str.c_str()));

        return str;
    }


    /*
     * string_builder::trim_begin
     */
    template <class T>
    void string_builder<T>::trim_begin(const typename T::value_type *chars) {
        if (this->is_empty()) return;
        size_t len = this->length();
        size_t cc = static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(chars));
        for (size_t i = 0; i < len; ++i) {
            const typename T::value_type& c = this->root->index(i);
            bool inChars = false;
            for (size_t j = 0; j < cc; ++j) {
                if (chars[j] == c) {
                    inChars = true;
                    break;
                }
            }
            if (!inChars) {
                if (i > 0) {
                    this->remove(0, i);
                }
                return;
            }
        }
        this->clear();
    }


    /*
     * string_builder::trim_end
     */
    template <class T>
    void string_builder<T>::trim_end(const typename T::value_type *chars) {
        if (this->is_empty()) return;
        size_t len = this->length();
        size_t cc = static_cast<size_t>(
                std::char_traits<typename T::value_type>::length(chars));
        for (size_t i = len; i > 0;) {
            --i;
            const typename T::value_type& c = this->root->index(i);
            bool inChars = false;
            for (size_t j = 0; j < cc; ++j) {
                if (chars[j] == c) {
                    inChars = true;
                    break;
                }
            }
            if (!inChars) {
                if (i < len - 1) {
                    this->remove(i + 1);
                }
                return;
            }
        }
        this->clear();
    }


    /*
     * string_builder::trim_begin
     */
    template <class T>
    void string_builder<T>::trim_begin(void) {
        if (this->is_empty()) return;
        size_t len = this->length();
        for (size_t i = 0; i < len; ++i) {
            if (!the::text::char_utility::is_space(this->root->index(i))) {
                if (i > 0) {
                    this->remove(0, i);
                }
                return;
            }
        }
        this->clear();
    }


    /*
     * string_builder::trim_end
     */
    template <class T>
    void string_builder<T>::trim_end(void) {
        if (this->is_empty()) return;
        size_t len = this->length();
        for (size_t i = len; i > 0;) {
            --i;
            if (!the::text::char_utility::is_space(this->root->index(i))) {
                if (i < len - 1) {
                    this->remove(i + 1);
                }
                return;
            }
        }
        this->clear();
    }


    /*
     * string_builder::operator[]
     */
    template <class T>
    const typename T::value_type& string_builder<T>::operator[](size_t idx) {
        ASSERT(idx >= 0);
        if (this->root == NULL) {
            throw the::invalid_operation_exception("string_builder is empty",
                __FILE__, __LINE__);
        }
        try {
            return this->root->index(idx);
        } catch(index_out_of_range_exception) {
            throw index_out_of_range_exception(idx, 0, this->root->length(),
                __FILE__, __LINE__);
       }
    }


    /*
     * string_builder::operator==
     */
    template <class T>
    bool string_builder<T>::operator==(const string_builder<T>& rhs) const {
        // NOT THAT SIMPLE!
        // We need to test if both objects would report the same string
        // Should implement something faster
        T l, r;
        this->to_string(l);
        rhs.to_string(r);
        return l == r;
    }


    /** Typedef of ANSI string_builder */
    typedef string_builder<astring> astring_builder;

    /** Typedef of ANSI string_builder */
    typedef string_builder<wstring> wstring_builder;

    /** Typedef of ANSI string_builder */
    typedef string_builder<tstring> tstring_builder;

} /* end namesapce text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRINGBUILDER_H_INCLUDED */
