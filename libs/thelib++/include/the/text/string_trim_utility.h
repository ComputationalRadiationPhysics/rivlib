/*
 * the/text/string_trim_utility.h
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
#ifndef THE_TEXT_STRING_TRIM_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_TRIM_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_converter"
#endif /* THE_STRING_H_INCLUDED */


namespace the {
namespace text {

    /**
     * Utility class to trim strings
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_trim_utility {
    public:

        /**
         * Remove all characters that are in the string 'chars' from the start 
         * of this string. 'chars' must be zero-terminated.
         *
         * @param str The string to be trimmed
         * @param chars A string of characters to be removed from the begin of 
         *              this string.
         */
        template<class T>
        static T& trim_begin(T& str, const typename T::value_type *chars);

        /**
         * Remove all characters that are in the string 'chars' from the end
         * of this string. 'chars' must be zero-terminated.
         *
         * @param str The string to be trimmed
         * @param chars A string of characters to be removed from the end of 
         *              this string.
         */
        template<class T>
        static T& trim_end(T& str, const typename T::value_type *chars);

        /**
         * Remove all characters that are in the string 'chars' from the start
         * and the end of this string. 'chars' must be zero-terminated.
         *
         * @param str The string to be trimmed
         * @param chars A string of characters to be removed from the begin and 
         *              end of this string.
         */
        template<class T>
        static T& trim(T& str, const typename T::value_type *chars);

        /**
         * Remove all whitespace characters from the start of this string.
         *
         * @param str The string to be trimmed
         */
        template<class T>
        static T& trim_begin(T& str);

        /**
         * Remove all whitespace characters from the end of this string. 
         *
         * @param str The string to be trimmed
         */
        template<class T>
        static T& trim_end(T& str);

        /**
         * Remove all whitespace characters from the start and the end of this
         * string.
         *
         * @param str The string to be trimmed
         */
        template<class T>
        static T& trim(T& str);

    protected:

        /** forbidden Ctor */
        string_trim_utility(void);

        /** forbidden Copy Ctor */
        string_trim_utility(const string_trim_utility& src);

        /** forbidden Dtor */
        ~string_trim_utility(void);

    private:

        /** Trim search predicate for characters */
        template<class T>
        class in_predicate {
        public:

            /**
             * Tests 'c'
             *
             * @param c the character to test
             * @param cs The characters forming the predicate
             * @param cnt The number of characters in 'cs'
             *
             * @return True if 'c' fullfills the predicate
             */
            static inline bool is(T& c, const T* cs, size_t cnt) {
                for (size_t i = 0; i < cnt; i++) {
                    if (c == cs[i]) return true;
                }
                return false;
            }
        };

        /** Trim search predicate for spaces */
        template<class T>
        class is_space_predicate {
        public:

            /**
             * Tests 'c'
             *
             * @param c the character to test
             * @param cs Not used
             * @param cnt Not used
             *
             * @return True if 'c' fullfills the predicate
             */
            static inline bool is(T& c, const T* cs, size_t cnt) {
                return the::text::char_utility::is_space(c);
            }

        };

        /**
         * Searches for the first character in 'str' which does not fullfil
         * the predicate
         *
         * @param T the string type
         * @param P the search predicate
         * @param str the string to search in
         * @param chars the search predicate characters
         * @param charsLen the length of 'chars' in characters
         *
         * @return The number of characters from the beginning of 'str' which
         *         fullfil the predicate
         */
        template<class T, class P> static size_t find_front(T& str,
                const typename T::value_type *chars, size_t charsLen) {
            size_t i = 0;
            for (; i < str.length(); ++i) {
                if (!P::is(str[i], chars, charsLen)) break;
            }
            return i;
        }

        /**
         * Searches for the last character in 'str' which does not fullfil
         * the predicate
         *
         * @param T the string type
         * @param P the search predicate
         * @param str the string to search in
         * @param chars the search predicate characters
         * @param charsLen the length of 'chars' in characters
         *
         * @return The position of the last character in 'str' which does not
         *         fullfil the predicate
         */
        template<class T, class P> static size_t find_back(T& str,
                const typename T::value_type *chars, size_t charsLen) {
            size_t i = str.length() - 1;
            for (; i != static_cast<size_t>(-1); --i) {
                if (!P::is(str[i], chars, charsLen)) break;
            }
            return i + 1;
        }

    };


    /*
     * string_trim_utility::trim_begin
     */
    template<class T>
    T& string_trim_utility::trim_begin(T& str,
            const typename T::value_type *chars) {
        size_t charsLen = static_cast<size_t>(
            std::char_traits<typename T::value_type>::length(chars));
        str.erase(0, find_front<T, in_predicate<typename T::value_type> >(
            str, chars, charsLen));

        return str;
    }


    /*
     * string_trim_utility::trim_end
     */
    template<class T>
    T& string_trim_utility::trim_end(T& str,
            const typename T::value_type *chars) {
        size_t charsLen = static_cast<size_t>(
            std::char_traits<typename T::value_type>::length(chars));
        str.erase(find_back<T, in_predicate<typename T::value_type> >(
            str, chars, charsLen));

        return str;
    }


    /*
     * string_trim_utility::trim
     */
    template<class T>
    T& string_trim_utility::trim(T& str,
            const typename T::value_type *chars) {
        size_t charsLen = static_cast<size_t>(
            std::char_traits<typename T::value_type>::length(chars));
        size_t start = find_front<T, in_predicate<typename T::value_type> >(
            str, chars, charsLen);

        if (start == str.length()) {
            str.clear();
        } else {
            str = str.substr(start, find_back<T,
                in_predicate<typename T::value_type> >(
                str, chars, charsLen) - start);
        }

        return str;
    }


    /*
     * string_trim_utility::trim_begin
     */
    template<class T>
    T& string_trim_utility::trim_begin(T& str) {
        str.erase(0,
            find_front<T, is_space_predicate<typename T::value_type> >(
            str, NULL, 0));
        return str;
    }


    /*
     * string_trim_utility::trim_end
     */
    template<class T>
    T& string_trim_utility::trim_end(T& str) {
        str.erase(
            find_back<T, is_space_predicate<typename T::value_type> >(
            str, NULL, 0));
        return str;
    }


    /*
     * string_trim_utility::trim
     */
    template<class T>
    T& string_trim_utility::trim(T& str) {
        size_t start =
            find_front<T, is_space_predicate<typename T::value_type> >(
            str, NULL, 0);

        if (start == str.length()) {
            str.clear();
        } else {
            str = str.substr(start, find_back<T,
                is_space_predicate<typename T::value_type> >(
                str, NULL, 0) - start);
        }

        return str;
    }

} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRINGTRIMUTILITY_H_INCLUDED */
