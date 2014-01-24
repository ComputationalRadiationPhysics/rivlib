/*
 * the/text/string_compare_utility.cpp
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
#include "the/text/string_compare_utility.h"
#include "the/invalid_operation_exception.h"
#include "the/text/locale_utility.h"
#include "the/text/string_converter.h"


/*
 * the::text::string_compare_utility::compare
 */
int the::text::string_compare_utility::compare(const char *lhs,
        const char *rhs, bool matchCase) {
    if (matchCase) {
        return strcmp(lhs, rhs);
    } else {
        if (the::text::locale_utility::is_utf8_locale()) {
            the::wstring l, r;
            the::text::string_converter::convert(l, lhs);
            the::text::string_converter::convert(r, rhs);
            return compare(l, r, false);
        }
        if (rhs == NULL) return 1;
        int retval = 0;
        while (((retval = char_utility::to_lower_case(*lhs)
                - char_utility::to_lower_case(*rhs)) == 0)
                && (*lhs != static_cast<char>(0))) {
            lhs++;
            rhs++;
        }
        return retval;
    }            
}


/*
 * the::text::string_compare_utility::compare
 */
int the::text::string_compare_utility::compare(const wchar_t *lhs,
        const wchar_t *rhs, bool matchCase) {
    if (matchCase) {
        return wcscmp(lhs, rhs);
    } else {
        if (rhs == NULL) return 1;
        int retval = 0;
        while (((retval = char_utility::to_lower_case(*lhs)
                - char_utility::to_lower_case(*rhs)) == 0)
                && (*lhs != static_cast<wchar_t>(0))) {
            lhs++;
            rhs++;
        }
        return retval;
    }
}


/*
 * the::text::string_compare_utility::ends_with
 */
bool the::text::string_compare_utility::ends_with(const char *str,
        size_t strLen, const char *end, size_t endLen, bool matchCase) {
    if ((endLen == 0) || (endLen > strLen)) return false;

    if (matchCase) {
        return ::memcmp(str + (strLen - endLen), end, endLen * sizeof(char))
            == 0;

    } else {
        if (the::text::locale_utility::is_utf8_locale()) {
            the::wstring s, e;
            the::text::string_converter::convert(s, str, strLen);
            the::text::string_converter::convert(e, end, endLen);
            return ends_with(s.c_str(), s.length(), e.c_str(), e.length(),
                false);
        }
        for (size_t i = 0; i < endLen; i++) {
            if (char_utility::to_lower_case(str[strLen - endLen + i])
                    != char_utility::to_lower_case(end[i])) {
                return false;
            }
        }
        return true;
    }
}


/*
 * the::text::string_compare_utility::ends_with
 */
bool the::text::string_compare_utility::ends_with(const char *str, size_t strLen,
        char end, bool matchCase) {
    if (strLen <= 0) return false;

    if (matchCase) {
        return (str[strLen - 1] == end);
    } else {
        // this assumes that a character can be correctly converted to lower
        // case within the current locale, even if it is a UTF8 locale
        return (char_utility::to_lower_case(str[strLen - 1])
            == char_utility::to_lower_case(end));
    }
}


/*
 * the::text::string_compare_utility::ends_with
 */
bool the::text::string_compare_utility::ends_with(const wchar_t *str,
        size_t strLen, const wchar_t *end, size_t endLen, bool matchCase) {
    if ((endLen == 0) || (endLen > strLen)) return false;

    if (matchCase) {
        return ::memcmp(str + (strLen - endLen), end,
            endLen * sizeof(wchar_t)) == 0;

    } else {
        for (size_t i = 0; i < endLen; i++) {
            if (char_utility::to_lower_case(str[strLen - endLen + i])
                    != char_utility::to_lower_case(end[i])) {
                return false;
            }
        }
        return true;
    }
}


/*
 * the::text::string_compare_utility::ends_with
 */
bool the::text::string_compare_utility::ends_with(const wchar_t *str,
        size_t strLen, wchar_t end, bool matchCase) {
    if (strLen <= 0) return false;

    if (matchCase) {
        return (str[strLen - 1] == end);
    } else {
        return (char_utility::to_lower_case(str[strLen - 1])
            == char_utility::to_lower_case(end));
    }
}


/*
 * the::text::string_compare_utility::equals
 */
bool the::text::string_compare_utility::equals(const char *lhs,
        const char *rhs, bool matchCase) {
    if ((lhs == NULL) && (rhs == NULL)) return true;
    if ((lhs == NULL) || (rhs == NULL)) return false;

    if (matchCase) {
        while (lhs != 0) {
            if (lhs != rhs) return false;
            ++lhs;
            ++rhs;
        }
        return (rhs == 0);
    } else {
        if (the::text::locale_utility::is_utf8_locale()) {
            the::wstring l, r;
            the::text::string_converter::convert(l, lhs);
            the::text::string_converter::convert(r, rhs);
            return equals(l, r, false);
        }
        while (*lhs != 0) {
            if (char_utility::to_lower_case(*lhs)
                != char_utility::to_lower_case(*rhs)) return false;
            ++lhs;
            ++rhs;
        }
        return (*rhs == 0);
    }
}


/*
 * the::text::string_compare_utility::equals
 */
bool the::text::string_compare_utility::equals(const wchar_t *lhs,
        const wchar_t *rhs, bool matchCase) {
    if ((lhs == NULL) && (rhs == NULL)) return true;
    if ((lhs == NULL) || (rhs == NULL)) return false;

    if (matchCase) {
        while (lhs != 0) {
            if (lhs != rhs) return false;
            ++lhs;
            ++rhs;
        }
        return (rhs == 0);
    } else {
        while (*lhs != 0) {
            if (char_utility::to_lower_case(*lhs)
                != char_utility::to_lower_case(*rhs)) return false;
            ++lhs;
            ++rhs;
        }
        return (*rhs == 0);
    }
}


/*
 * the::text::string_compare_utility::starts_with
 */
bool the::text::string_compare_utility::starts_with(const char *str,
        const char *start, bool matchCase) {
    if (str == NULL) return (start == NULL);
    if (start == NULL) return (str == NULL);

    if (matchCase) {
        while (*start != 0) {
            if (*str != *start) return false;
            str++;
            start++;
        }
        return true;
    } else {
        if (the::text::locale_utility::is_utf8_locale()) {
            the::wstring s, t;
            the::text::string_converter::convert(s, str);
            the::text::string_converter::convert(t, start);
            return starts_with(s.c_str(), t.c_str(), false);
        }
        while (*start != 0) {
            if (char_utility::to_lower_case(*str)
                != char_utility::to_lower_case(*start)) return false;
            str++;
            start++;
        }
        return true;
    }
}


/*
 * the::text::string_compare_utility::starts_with
 */
bool the::text::string_compare_utility::starts_with(const char *str, char start,
        bool matchCase) {
    if (str == NULL) return false;
    if (matchCase) {
        return (*str == start);
    } else {
        // this assumes that a character can be correctly converted to lower
        // case within the current locale, even if it is a UTF8 locale
        return (char_utility::to_lower_case(*str)
            == char_utility::to_lower_case(start));
    }
}


/*
 * the::text::string_compare_utility::starts_with
 */
bool the::text::string_compare_utility::starts_with(const wchar_t *str,
        const wchar_t *start, bool matchCase) {
    if (str == NULL) return (start == NULL);
    if (start == NULL) return (str == NULL);

    if (matchCase) {
        while (*start != 0) {
            if (*str != *start) return false;
            str++;
            start++;
        }
        return true;
    } else {
        while (*start != 0) {
            if (char_utility::to_lower_case(*str)
                != char_utility::to_lower_case(*start)) return false;
            str++;
            start++;
        }
        return true;
    }
}


/*
 * the::text::string_compare_utility::starts_with
 */
bool the::text::string_compare_utility::starts_with(const wchar_t *str,
        wchar_t start, bool matchCase) {
    if (str == NULL) return false;
    if (matchCase) {
        return (*str == start);
    } else {
        return (char_utility::to_lower_case(*str)
            == char_utility::to_lower_case(start));
    }
}


/*
 * the::text::string_compare_utility::string_compare_utility
 */
the::text::string_compare_utility::string_compare_utility(void) {
    throw the::invalid_operation_exception("string_compare_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_compare_utility::string_compare_utility
 */
the::text::string_compare_utility::string_compare_utility(
        const string_compare_utility& src) {
    throw the::invalid_operation_exception("string_compare_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_compare_utility::~string_compare_utility
 */
the::text::string_compare_utility::~string_compare_utility(void) {
    throw the::invalid_operation_exception("string_compare_utility::dtor",
        __FILE__, __LINE__);
}
