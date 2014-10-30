/*
 * the/text/string_find_utility.cpp
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

#include "the/string.h"
#include "the/text/string_find_utility.h"
#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/stack_trace.h"


/*
 * the::text::string_find_utility::NO_LIMIT
 */
const size_t the::text::string_find_utility::NO_LIMIT = -1;


namespace the {
namespace text {
namespace _internal {


    /** Utility class for case conversion */
    template<class T> class no_case_conv {
    public:

        /** Returns 'c', optionally with converted casing
         *
         * @param c The character
         *
         * @retrun 'c'
         */
        static THE_FORCE_INLINE const T& do_it(const T& c) {
            return c;
        }

    };


    /** Utility class for case conversion */
    template<class T> class lower_case_conv {
    public:

        /** Returns 'c', optionally with converted casing
         *
         * @param c The character
         *
         * @retrun 'c'
         */
        static THE_FORCE_INLINE T do_it(const T& c) {
            return the::text::char_utility::to_lower_case(c);
        }

    };


    /**
     * Searches for the first occurance of 'search' in 'str' after
     * 'beginningAt'
     *
     * @param T The character type
     * @param C The case conversion utility class
     * @param str The string to search in
     * @param search The search string
     * @param searchLen The length of 'search' in character
     * @param beginningAt The position to search from
     *
     * @return The found position or '-1' if search was not found
     */
    template<class T, class C> size_t str_find_first(const T *str,
            const T *search, size_t searchLen, size_t beginningAt) {
        ASSERT(str != NULL);
        ASSERT(search != NULL);
        ASSERT(searchLen > 0);

        // move to the beginning
        size_t i = 0, j;
        while (i < beginningAt) {
            if (str[i] == 0) return -1; // string depleted even before start
            ++i;
        }

        bool found = false;
        while (!found) {
            found = true;
            for (j = 0; j < searchLen; j++) {
                if (str[i + j] == 0) return -1; // string depleted
                if (C::do_it(str[i + j]) != C::do_it(search[j])) {
                    found = false; // difference found
                    break;
                }
            }
            ++i;
        }

        return i - 1;
    }


    /**
     * Searches for the first occurance of 'search' in 'str' after
     * 'beginningAt'
     *
     * @param T The character type
     * @param C The case conversion utility class
     * @param str The string to search in
     * @param strLen The length of 'str'
     * @param search The search string
     * @param searchLen The length of 'search' in character
     * @param beginningAt The position to search from
     *
     * @return The found position or '-1' if search was not found
     */
    template<class T, class C> size_t str_find_last(const T *str,
            size_t strLen, const T *search, size_t searchLen,
            size_t beginningAt) {
        ASSERT(str != NULL);
        ASSERT(strLen != 0);
        ASSERT(search != NULL);
        ASSERT(searchLen > 0);

        size_t i = (((beginningAt >= 0) && (beginningAt < strLen))
            ? beginningAt : strLen) - searchLen;
        size_t j;

        bool found = false;
        while (!found) {
            found = true;
            for (j = 0; j < searchLen; j++) {
                if (str[i + j] == 0) return -1; // string depleted
                if (C::do_it(str[i + j]) != C::do_it(search[j])) {
                    found = false; // difference found
                    break;
                }
            }
            --i;
        }

        return i + 1;
    }


} /* end namespace _internal */
} /* end namespace text */
} /* end namespace the */


/*
 * the::text::string_find_utility::count
 */
size_t the::text::string_find_utility::count(const char *str, const char c) {
    THE_STACK_TRACE;
    size_t retval = 0;
    const char *s = str;

    while (s != nullptr) {
        if (*s == c) {
            ++retval;
        }
        ++s;
    }

    return retval;
}


/*
 * the::text::string_find_utility::count
 */
size_t the::text::string_find_utility::count(const wchar_t *str, const char c) {
    THE_STACK_TRACE;
    size_t retval = 0;
    const wchar_t *s = str;

    while (s != nullptr) {
        if (*s == c) {
            ++retval;
        }
        ++s;
    }

    return retval;
}


/*
 * the::text::string_find_utility::find
 */
size_t the::text::string_find_utility::find(const char *str, const char *search,
        size_t beginningAt, bool matchCase) {
    if ((search == NULL) || (str == NULL)) return astring::npos;
    size_t searchLen = static_cast<size_t>(
        std::char_traits<char>::length(search));
    if (searchLen == 0) return astring::npos;
    size_t retval = the::astring::npos;

    if (matchCase) {
        retval = _internal::str_find_first<char,
            _internal::no_case_conv<char> >(
            str, search, searchLen, beginningAt);
    } else {
        if (the::text::locale_utility::is_utf8_locale()) {
            the::wstring h, n;
            the::text::string_converter::convert(h, str);
            the::text::string_converter::convert(n, search, searchLen);
            retval = _internal::str_find_first<wchar_t,
                _internal::lower_case_conv<wchar_t> >(
                h.c_str(), n.c_str(), n.length(), beginningAt);
        } else {
            retval = _internal::str_find_first<char,
                _internal::lower_case_conv<char> >(
                str, search, searchLen, beginningAt);
        }
    }

    if (retval == static_cast<size_t>(-1)) {
        retval = the::astring::npos;
    }
    return retval;
}


/*
 * the::text::string_find_utility::find
 */
size_t the::text::string_find_utility::find(const wchar_t *str,
        const wchar_t *search, size_t beginningAt, bool matchCase) {
    if ((search == NULL) || (str == NULL)) return wstring::npos;
    size_t searchLen = static_cast<size_t>(
        std::char_traits<wchar_t>::length(search));
    if (searchLen == 0) return wstring::npos;
    size_t retval = the::wstring::npos;

    if (matchCase) {
        retval = _internal::str_find_first<wchar_t,
            _internal::no_case_conv<wchar_t> >(
            str, search, searchLen, beginningAt);
    } else {
        retval = _internal::str_find_first<wchar_t,
            _internal::lower_case_conv<wchar_t> >(
            str, search, searchLen, beginningAt);
    }

    if (retval == static_cast<size_t>(-1)) {
        retval = the::wstring::npos;
    }
    return retval;
}


/*
 * the::text::string_find_utility::find_last
 */
size_t the::text::string_find_utility::find_last(const char *str,
        const char *search, size_t beginningAt, bool matchCase) {
    if ((search == NULL) || (str == NULL)) return astring::npos;
    size_t strLen = static_cast<size_t>(
        std::char_traits<char>::length(str));
    if (strLen == 0) return astring::npos;
    size_t searchLen = static_cast<size_t>(
        std::char_traits<char>::length(search));
    if ((searchLen == 0) || (searchLen > strLen)) return astring::npos;
    size_t retval = the::astring::npos;

    if (matchCase) {
        retval = _internal::str_find_last<char,
            _internal::no_case_conv<char> >(
            str, strLen, search, searchLen, beginningAt);
    } else {
        if (the::text::locale_utility::is_utf8_locale()) {
            the::wstring h, n;
            the::text::string_converter::convert(h, str, strLen);
            the::text::string_converter::convert(n, search, searchLen);
            retval = _internal::str_find_last<wchar_t,
                _internal::lower_case_conv<wchar_t> >(
                h.c_str(), h.length(), n.c_str(), n.length(), beginningAt);
        } else {
            retval = _internal::str_find_last<char,
                _internal::lower_case_conv<char> >(
                str, strLen, search, searchLen, beginningAt);
        }
    }

    if (retval == static_cast<size_t>(-1)) {
        retval = the::astring::npos;
    }
    return retval;
}


/*
 * the::text::string_find_utility::find_last
 */
size_t the::text::string_find_utility::find_last(const wchar_t *str,
        const wchar_t *search, size_t beginningAt, bool matchCase) {
    if ((search == NULL) || (str == NULL)) return wstring::npos;
    size_t strLen = static_cast<size_t>(
        std::char_traits<wchar_t>::length(str));
    if (strLen == 0) return wstring::npos;
    size_t searchLen = static_cast<size_t>(
        std::char_traits<wchar_t>::length(search));
    if ((searchLen == 0) || (searchLen > strLen)) return wstring::npos;
    size_t retval = the::wstring::npos;

    if (matchCase) {
        retval = _internal::str_find_last<wchar_t,
            _internal::no_case_conv<wchar_t> >(
            str, strLen, search, searchLen, beginningAt);
    } else {
        retval = _internal::str_find_last<wchar_t,
            _internal::lower_case_conv<wchar_t> >(
            str, strLen, search, searchLen, beginningAt);
    }

    if (retval == static_cast<size_t>(-1)) {
        retval = the::wstring::npos;
    }
    return retval;
}


/*
 * the::text::string_find_utility::string_find_utility
 */
the::text::string_find_utility::string_find_utility(void) {
    throw the::invalid_operation_exception("string_find_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_find_utility::string_find_utility
 */
the::text::string_find_utility::string_find_utility(
    const string_find_utility& src) {
    throw the::invalid_operation_exception("string_find_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_find_utility::~string_find_utility
 */
the::text::string_find_utility::~string_find_utility(void) {
    throw the::invalid_operation_exception("string_find_utility::dtor",
        __FILE__, __LINE__);
}
