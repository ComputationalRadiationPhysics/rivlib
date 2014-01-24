/*
 * the/text/string_compare_utility.h
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
#ifndef THE_TEXT_STRING_COMPARE_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_COMPARE_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_utility"
#endif /* THE_STRING_H_INCLUDED */


namespace the {
namespace text {

    /**
     * Utility class to compare strings
     *
     * @remark Do not use directly. Use 'string_utility' instead
     */
    class string_compare_utility {
    public:

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return -1 if ('lhs' < 'rhs'),
         *          0 if ('lhs' == 'rhs'), and
         *         +1 if ('lhs' > 'rhs')
         */
        template<class T>
        static inline int compare(const T& lhs, const T& rhs,
                bool matchCase = true) {
            return compare(lhs.c_str(), rhs.c_str(), matchCase);
        }

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return -1 if ('lhs' < 'rhs'),
         *          0 if ('lhs' == 'rhs'), and
         *         +1 if ('lhs' > 'rhs')
         */
        template<class T>
        static inline int compare(const typename T::value_type *lhs,
                const T& rhs, bool matchCase = true) {
            return compare(lhs, rhs.c_str(), matchCase);
        }

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return -1 if ('lhs' < 'rhs'),
         *          0 if ('lhs' == 'rhs'), and
         *         +1 if ('lhs' > 'rhs')
         */
        template<class T>
        static int compare(const T& lhs, const typename T::value_type *rhs,
                bool matchCase = true) {
            return compare(lhs.c_str(), rhs, matchCase);
        }

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return -1 if ('lhs' < 'rhs'),
         *          0 if ('lhs' == 'rhs'), and
         *         +1 if ('lhs' > 'rhs')
         */
        static int compare(const char *lhs, const char *rhs,
            bool matchCase = true);

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return -1 if ('lhs' < 'rhs'),
         *          0 if ('lhs' == 'rhs'), and
         *         +1 if ('lhs' > 'rhs')
         */
        static int compare(const wchar_t *lhs, const wchar_t *rhs,
            bool matchCase = true);

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param end The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        template<class T>
        static bool ends_with(const T& str, const T& end,
                bool matchCase = true) {
            return ends_with(str.c_str(), str.length(), end.c_str(),
                end.length(), matchCase);
        }

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param end The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        template<class T>
        static inline bool ends_with(const T& str,
                const typename T::value_type *end, bool matchCase = true) {
            size_t endLen = 0;
            if (end != NULL) while (end[endLen] != 0) endLen++;
            return ends_with(str.c_str(), str.length(), end, endLen, matchCase);
        }

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param end The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        template<class T>
        static inline bool ends_with(const T& str,
                const typename T::value_type end, bool matchCase = true) {
            return ends_with(str.c_str(), str.length(), end, matchCase);
        }

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param strLen The length of 'str' in characters
         * @param end The test string
         * @param endLen The length of 'end' in characters
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        static bool ends_with(const char *str, size_t strLen,
            const char *end, size_t endLen, bool matchCase = true);

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param strLen The length of 'str' in characters
         * @param end The test character
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        static bool ends_with(const char *str, size_t strLen,
            char end, bool matchCase = true);

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param strLen The length of 'str' in characters
         * @param end The test string
         * @param endLen The length of 'end' in characters
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        static bool ends_with(const wchar_t *str, size_t strLen,
            const wchar_t *end, size_t endLen, bool matchCase = true);

        /**
         * Tests if 'str' ends with 'end'
         *
         * @param str The string
         * @param strLen The length of 'str' in characters
         * @param end The test character
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' ends with 'end', false otherwise
         */
        static bool ends_with(const wchar_t *str, size_t strLen,
            wchar_t end, bool matchCase = true);

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'lhs' equals 'rhs', false otherwise
         */
        template<class T>
        static bool equals(const T& lhs, const T& rhs,
                bool matchCase = true) {
            return equals(lhs.c_str(), rhs.c_str(), matchCase);
        }

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'lhs' equals 'rhs', false otherwise
         */
        template<class T>
        static bool equals(const typename T::value_type *lhs, const T& rhs,
                bool matchCase = true) {
            return equals(lhs, rhs.c_str(), matchCase);
        }

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'lhs' equals 'rhs', false otherwise
         */
        template<class T>
        static bool equals(const T& lhs, const typename T::value_type *rhs,
                bool matchCase = true) {
            return equals(lhs.c_str(), rhs, matchCase);
        }

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'lhs' equals 'rhs', false otherwise
         */
        static bool equals(const char *lhs, const char *rhs,
            bool matchCase = true);

        /**
         * Compares 'lhs' and 'rhs' optionally ignoring the case
         *
         * @param lhs The first string
         * @param rhs The second string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'lhs' equals 'rhs', false otherwise
         */
        static bool equals(const wchar_t *lhs, const wchar_t *rhs,
            bool matchCase = true);

        /**
         * Answer if a string is empty
         *
         * @param str The string to be tested
         *
         * @return True if str has a length of zero
         */
        template<class T>
        static bool is_empty(const T& str);

        /**
         * Answer if a string is empty
         *
         * @param str The string to be tested
         *
         * @return True if str has a length of zero
         */
        template<class T>
        static bool is_empty(const T *str);

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        template<class T>
        static inline bool starts_with(const T& str, const T& start,
                bool matchCase = true) {
            return starts_with(str.c_str(), start.c_str(), matchCase);
        }

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        template<class T>
        static inline bool starts_with(const T& str, 
                const typename T::value_type *start, bool matchCase = true) {
            return starts_with(str.c_str(), start, matchCase);
        }

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        template<class T>
        static inline bool starts_with(const T& str,
                const typename T::value_type start, bool matchCase = true) {
            return starts_with(str.c_str(), start, matchCase);
        }

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        static bool starts_with(const char *str, const char *start,
            bool matchCase = true);

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        static bool starts_with(const char *str, char start,
            bool matchCase = true);

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        static bool starts_with(const wchar_t *str, const wchar_t *start,
            bool matchCase = true);

        /**
         * Tests if 'str' starts with 'start'
         *
         * @param str The string
         * @param start The test string
         * @param matchCase If true the matching will be case sensitive, if
         *                  false the matching will be case insensitive.
         *
         * @return True if 'str' starts with 'start', false otherwise
         */
        static bool starts_with(const wchar_t *str, wchar_t start,
            bool matchCase = true);

    protected:

        /** forbidden Ctor */
        string_compare_utility(void);

        /** forbidden Copy Ctor */
        string_compare_utility(const string_compare_utility& src);

        /** forbidden Dtor */
        ~string_compare_utility(void);

    private:

    };


    /*
     * string_compare_utility::is_empty
     */
    template<class T>
    bool string_compare_utility::is_empty(const T& str) {
        return str.empty() != 0;
    }


    /*
     * string_compare_utility::is_empty
     */
    template<class T>
    bool string_compare_utility::is_empty(const T* str) {
        return (str == NULL) || (str[0] == 0);
    }


} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_COMPARE_UTILITY_H_INCLUDED */
