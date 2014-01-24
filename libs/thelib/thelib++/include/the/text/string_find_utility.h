/*
 * the/text/string_find_utility.h
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
#ifndef THE_TEXT_STRINGFINDUTILITY_H_INCLUDED
#define THE_TEXT_STRINGFINDUTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_utility"
#endif /* THE_STRING_H_INCLUDED */
#include "the/collections/array.h"
#include <utility>
#include "the/types.h"


namespace the {
namespace text {

    /*
     * Utility class to search in strings
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_find_utility {
    public:

        /** Constant NO_LIMIT */
        static const size_t NO_LIMIT;

        /**
         * Answer whether 'search' is contained in 'str' after 'beginningAt'.
         *
         * @param str         The string to search in.
         * @param search      The string to search for.
         * @param beginningAt The position to start the search.
         * @param matchCase   If true the matching will be case sensitive, if
         *                    false the matching will be case insensitive.
         *
         * @return true if 'str' contains 'search' after 'beginningAt',
                   false otherwise.
         */
        template<class T>
        static inline bool contains(const T& str,
                const T& search,
                const size_t beginningAt = 0,
                const bool matchCase = true) {
            return (string_find_utility::find(str, search, beginningAt,
                matchCase) != T::npos);
        }

        /**
         * Answer whether 'search' is contained in 'str' after 'beginningAt'.
         *
         * @param str         The string to search in.
         * @param search      The string to search for.
         * @param beginningAt The position to start the search.
         * @param matchCase   If true the matching will be case sensitive, if
         *                    false the matching will be case insensitive.
         *
         * @return true if 'str' contains 'search' after 'beginningAt',
                   false otherwise.
         */
        template<class T>
        static inline bool contains(const T& str,
                const typename T::value_type *search,
                const size_t beginningAt = 0,
                const bool matchCase = true) {
            return (string_find_utility::find(str, search, beginningAt,
                matchCase) != T::npos);
        }

        /**
         * Answer whether 'search' is contained in 'str' after 'beginningAt'.
         *
         * @param str         The string to search in.
         * @param search      The character to search for.
         * @param beginningAt The position to start the search.
         * @param matchCase   If true the matching will be case sensitive, if
         *                    false the matching will be case insensitive.
         *
         * @return true if 'str' contains 'search' after 'beginningAt',
                   false otherwise.
         */
        template<class T>
        static inline bool contains(const T& str,
                const typename T::value_type search,
                const size_t beginningAt = 0,
                const bool matchCase = true) {
            return (string_find_utility::find(str, search, beginningAt,
                matchCase) != T::npos);
        }

        /**
         * Answer whether 'search' is contained in 'str' after 'beginningAt'.
         *
         * @param str         The string to search in.
         * @param search      The character to search for.
         * @param beginningAt The position to start the search.
         * @param matchCase   If true the matching will be case sensitive, if
         *                    false the matching will be case insensitive.
         *
         * @return true if 'str' contains 'search' after 'beginningAt',
                   false otherwise.
         */
        template<class T>
        static inline bool contains(const T *str,
                const T search,
                const size_t beginningAt = 0,
                const bool matchCase = true) {
            return (string_find_utility::find(str, search, beginningAt,
                matchCase) != T::npos);
        }

        /**
         * Answer whether 'search' is contained in 'str' after 'beginningAt'.
         *
         * @param str         The string to search in.
         * @param search      The string to search for.
         * @param beginningAt The position to start the search.
         * @param matchCase   If true the matching will be case sensitive, if
         *                    false the matching will be case insensitive.
         *
         * @return true if 'str' contains 'search' after 'beginningAt',
                   false otherwise.
         */
        static inline bool contains(const char *str,
                const char *search,
                const size_t beginningAt = 0,
                const bool matchCase = true) {
            return (string_find_utility::find(str, search, beginningAt,
                matchCase) != std::string::npos);
        }

        /**
         * Searches the first occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return true if 'str' contains 'search' after 'beginningAt',
                   false otherwise.
         */
        static inline bool contains(const wchar_t *str, 
                const wchar_t *search,
                const size_t beginningAt = 0,
                const bool matchCase = true) {
            return (string_find_utility::find(str, search, beginningAt,
                matchCase) != std::wstring::npos);
        }

        /**
         * Counts the number of occurences of 'c' in 'str'
         *
         * @param str The string
         * @param c The character
         *
         * @return The number of occurences of 'c' in 'str'
         */
        template<class T>
        static size_t count(const T& str, const typename T::value_type c);

        /**
         * Counts the occurrences of 'c' in 'str'.
         *
         * @param str The string to be searched for 'c'. It is safe to pass 
         *            nullptr for 'str'.
         * @param c   The character to be searched in 'str'.
         *
         * @return The number of occurrences of 'c' in 'str'.
         */
        static size_t count(const char *str, const char c);

        /**
         * Counts the occurrences of 'c' in 'str'.
         *
         * @param str The string to be searched for 'c'. It is safe to pass 
         *            nullptr for 'str'.
         * @param c   The character to be searched in 'str'.
         *
         * @return The number of occurrences of 'c' in 'str'.
         */
        static size_t count(const wchar_t *str, const char c);

        /**
         * Searches the first occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The first position of 'search' within 'str' or T::npos
         */
        template<class T>
        static inline size_t find(const T& str, const T& search,
                size_t beginningAt = 0, bool matchCase = true) {
            return find(str.c_str(), search.c_str(), beginningAt, matchCase);
        }

        /**
         * Searches the first occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The first position of 'search' within 'str' or T::npos
         */
        template<class T>
        static inline size_t find(const T& str,
                const typename T::value_type *search, size_t beginningAt = 0,
                bool matchCase = true) {
            return find(str.c_str(), search, beginningAt, matchCase);
        }

        /**
         * Searches the first occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The first position of 'search' within 'str' or T::npos
         */
        template<class T>
        static inline size_t find(const T& str,
                const typename T::value_type search, size_t beginningAt = 0,
                bool matchCase = true) {
            typename T::value_type s[2];
            s[0] = search;
            s[1] = static_cast<typename T::value_type>(0);
            return find(str.c_str(), s, beginningAt, matchCase);
        }

        /**
         * Searches the first occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The first position of 'search' within 'str' or T::npos
         */
        static size_t find(const char *str, const char *search,
            size_t beginningAt = 0, bool matchCase = true);

        /**
         * Searches the first occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The first position of 'search' within 'str' or T::npos
         */
        static size_t find(const wchar_t *str, const wchar_t *search,
            size_t beginningAt = 0, bool matchCase = true);

        /**
         * Searches the last occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The last position of 'search' within 'str' or T::npos
         */
        template<class T>
        static inline size_t find_last(const T& str, const T& search,
                size_t beginningAt = T::npos, bool matchCase = true) {
            return find_last(str.c_str(), search.c_str(), beginningAt,
                matchCase);
        }

        /**
         * Searches the last occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The last position of 'search' within 'str' or T::npos
         */
        template<class T>
        static inline size_t find_last(const T& str,
                const typename T::value_type *search,
                size_t beginningAt = T::npos, bool matchCase = true) {
            return find_last(str.c_str(), search, beginningAt, matchCase);
        }

        /**
         * Searches the last occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The last position of 'search' within 'str' or T::npos
         */
        template<class T>
        static inline size_t find_last(const T& str,
                const typename T::value_type search,
                size_t beginningAt = T::npos, bool matchCase = true) {
            typename T::value_type s[2];
            s[0] = search;
            s[1] = static_cast<typename T::value_type>(0);
            return find_last(str, s, beginningAt, matchCase);
        }

        /**
         * Searches the last occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The last position of 'search' within 'str' or T::npos
         */
        static size_t find_last(const char *str, const char *search,
            size_t beginningAt = the::astring::npos, bool matchCase = true);

        /**
         * Searches the last occurance of 'search' in 'str'
         *
         * @param str The string to search in
         * @param search The string to search for
         * @param beginningAt The position to start the search
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return The last position of 'search' within 'str' or T::npos
         */
        static size_t find_last(const wchar_t *str, const wchar_t *search,
            size_t beginningAt = the::wstring::npos, bool matchCase = true);

        /**
         * Replaces all occurences of any first element of the 'map' array by
         * the corresponding second element, non-recursively. The elements in
         * 'map' are ordered by priority.
         *
         * @param str The string to replace in
         * @param map The map of replacement pairs
         *
         * @return The number of replacements
         */
        template<class T>
        static size_t multi_replace(T& str, the::collections::array<
            std::pair<T, T> > map);

        /**
         * Removes all occurences of 'sub' from 'str'
         *
         * @param str The string to be changed
         * @param sub The string to be removed
         *
         * @return A reference to 'str'
         */
        template<class T>
        static inline T& remove(T& str, const T& sub) {
            typename T::value_type s2[1];
            s2[0] = static_cast<typename T::value_type>(0);
            return replace(str, sub.c_str(), s2);
        }

        /**
         * Removes all occurences of 'sub' from 'str'
         *
         * @param str The string to be changed
         * @param sub The string to be removed
         *
         * @return A reference to 'str'
         */
        template<class T>
        static inline T& remove(T& str, const typename T::value_type *sub) {
            typename T::value_type s2[1];
            s2[0] = static_cast<typename T::value_type>(0);
            replace(str, sub, s2);
            return str;
        }


        /**
         * Removes all occurences of 'sub' from 'str'
         *
         * @param str The string to be changed
         * @param sub The string to be removed
         *
         * @return A reference to 'str'
         */
        template<class T>
        static inline T& remove(T& str, const typename T::value_type sub) {
            typename T::value_type s1[2];
            s1[0] = sub;
            s1[1] = static_cast<typename T::value_type>(0);
            typename T::value_type s2[1];
            s2[0] = static_cast<typename T::value_type>(0);
            return replace(str, s1, s2);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldChar The old char
         * @param newChar The new char
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        static size_t replace(T& str, const typename T::value_type oldChar,
            const typename T::value_type newChar, size_t limit = NO_LIMIT);

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldChar The old char
         * @param newStr The new string
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str,
                const typename T::value_type oldChar,
                const typename T::value_type *newStr,
                size_t limit = NO_LIMIT) {
            typename T::value_type s[2];
            s[0] = oldChar;
            s[1] = static_cast<typename T::value_type>(0);
            return replace(str, s, newStr, limit);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldChar The old char
         * @param newStr The new string
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str,
                const typename T::value_type oldChar, const T& newStr,
                size_t limit = NO_LIMIT) {
            typename T::value_type s[2];
            s[0] = oldChar;
            s[1] = static_cast<typename T::value_type>(0);
            return replace(str, s, newStr.c_str(), limit);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldStr The old string
         * @param newChar The new char
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str,
                const typename T::value_type *oldStr,
                const typename T::value_type newChar,
                size_t limit = NO_LIMIT) {
            typename T::value_type s[2];
            s[0] = newChar;
            s[1] = static_cast<typename T::value_type>(0);
            return replace(str, oldStr, s, limit);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldStr The old string
         * @param newStr The new string
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        static size_t replace(T& str, const typename T::value_type *oldStr,
            const typename T::value_type *newStr, size_t limit = NO_LIMIT);

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldStr The old string
         * @param newStr The new string
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str,
                const typename T::value_type *oldStr,
                const T& newStr, size_t limit = NO_LIMIT) {
            return replace(str, oldStr, newStr.c_str(), limit);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldStr The old string
         * @param newChar The new char
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str, const T& oldStr,
                const typename T::value_type newChar,
                size_t limit = NO_LIMIT) {
            typename T::value_type s[2];
            s[0] = newChar;
            s[1] = static_cast<typename T::value_type>(0);
            return replace(str, oldStr.c_str(), s, limit);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldStr The old string
         * @param newStr The new string
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str, const T& oldStr,
                const typename T::value_type *newStr,
                size_t limit = NO_LIMIT) {
            return replace(str, oldStr.c_str(), newStr, limit);
        }

        /**
         * Replaces the 'limit' first number of occurences of 'oldChar' in
         * 'str' by 'newChar'. If 'limit' is zero, all occurences are
         * replaced.
         *
         * @param str The string to be changed
         * @param oldStr The old string
         * @param newStr The new string
         * @param limit The number of occurences to be replaced or zero
         *
         * @return The number of replaced occurences
         */
        template<class T>
        inline static size_t replace(T& str, const T& oldStr,
                const T& newStr, size_t limit = NO_LIMIT) {
            return replace(str, oldStr.c_str(), newStr.c_str(), limit);
        }

    protected:

        /** forbidden Ctor */
        string_find_utility(void);

        /** forbidden Copy Ctor */
        string_find_utility(const string_find_utility& src);

        /** forbidden Dtor */
        ~string_find_utility(void);

    private:

    };


    /*
     * string_find_utility::Count
     */
    template<class T>
    size_t string_find_utility::count(const T& str,
            const typename T::value_type c) {
        size_t retval = 0;
        size_t offset = 0; //beginningAt;

        while ((offset = str.find(c, offset)) != T::npos) {
            retval++;
            offset++;
        }

        return retval;
    }


    /*
     * string_find_utility::multi_replace
     */
    template<class T>
    size_t string_find_utility::multi_replace(T& str, the::collections::array<
            std::pair<T, T> > map) {
        // naive implementation, because I don't want to handle entries with
        // different lengths in rabin-karp
        size_t mapCnt = map.size();
        size_t *np = new size_t[mapCnt];
        size_t acCnt = 0;
        size_t rplCnt = 0;
        bool first;

        for (size_t i = 0; i < mapCnt; ++i) {
            np[i] = str.find(map[i].first);
            if (np[i] != T::npos) ++acCnt;
        }

        while (acCnt > 0) {
            first = true;
            size_t dst;
            size_t id;
            for (size_t i = 0; i < mapCnt; ++i) {
                if (np[i] == T::npos) continue;
                if (first) {
                    id = i;
                    dst = np[i];
                    first = false;
                } else if (dst > np[i]) {
                    id = i;
                    dst = np[i];
                }
            }
            ASSERT(first == false);
            ASSERT((id >= 0) && (id < mapCnt));

            str.erase(dst, map[id].first.length());
            size_t sl = map[id].second.length();
            str.insert(dst, map[id].second);
            dst += sl;
            for (size_t i = 0; i < mapCnt; ++i) {
                if (np[i] == T::npos) continue;
                np[i] = str.find(map[i].first, dst);
                if (np[i] == T::npos) --acCnt;
            }
        }

        delete[] np;

        return rplCnt;
    }


    /*
     * string_find_utility::replace
     */
    template<class T>
    size_t string_find_utility::replace(T& str,
            const typename T::value_type oldChar,
            const typename T::value_type newChar, size_t limit) {
        size_t retval = 0;
        size_t len = str.length();
        
        for (size_t i = 0;
                (i < len) && ((limit == NO_LIMIT) || (retval < limit));
                i++) {
            if (str[i] == oldChar) {
                str[i] = newChar;
                retval++;
            }
        }

        return retval;
    }


    /*
     * string_find_utility::replace
     */
    template<class T>
    size_t string_find_utility::replace(T& str,
            const typename T::value_type *oldStr,
            const typename T::value_type *newStr, size_t limit) {
        size_t retval = 0; // Number of replacements
        size_t oldLen = std::char_traits<typename T::value_type>::length(oldStr);
            // Length of 'oldStr'
        size_t newLen = std::char_traits<typename T::value_type>::length(newStr);
            // Length of 'newStr'
        size_t nextStart = 0; // Next search start
        typename T::value_type *nstr = NULL;
        typename T::value_type *dst = NULL;

        if (oldLen < 1) {
            /* String to replace is empty, nothing to do. */
            return 0;
        }

        /* Count number of replacements. */
        while (((nextStart = str.find(oldStr, nextStart)) != T::npos)
                && ((limit == NO_LIMIT) || (retval < limit))) {
            retval++;
            nextStart += oldLen;
        }

        if (retval < 1) {
            /* String to replace was not found, nothing to do. */
            return 0;
        }
        /* Must replace at least one occurrence at this point. */

        /* Copy the data into new buffer 'str'. */
        nstr = new typename T::value_type[str.length() + retval * newLen
            - retval * oldLen + 1];

        nextStart = 0;
        retval = 0;
        const typename T::value_type *src = str.c_str();
        dst = nstr;
        while (((nextStart = str.find(oldStr, nextStart)) != T::npos)
                && ((limit == NO_LIMIT) || (retval < limit))) {
            while (src < str.c_str() + nextStart) {
                *dst++ = *src++;
            }
            ::memcpy(dst, newStr, newLen * sizeof(typename T::value_type));
            dst += newLen;
            src += oldLen;
            retval++;
            nextStart += oldLen;
        }
        while ((*dst++ = *src++) != 0);

        str.assign(nstr);
        delete[] nstr;

        return retval;
    }


} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_FIND_UTILITY_H_INCLUDED */
