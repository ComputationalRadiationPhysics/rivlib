/*
 * the/text/string_converter.cpp
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

#include <string>

#include "the/assert.h"
#include "the/invalid_operation_exception.h"

#include "the/text/locale_utility.h"
#include "the/text/string_converter.h"


/*
 * the::text::string_converter::convert
 */
the::astring& the::text::string_converter::convert(the::astring& dst,
        const wchar_t *src, const size_t len, const char defChar) {
    THE_STACK_TRACE;

    if ((src == NULL) || (len <= 0)) {
        dst.clear();
        return dst;
    }

    if (the::text::locale_utility::is_utf8_locale()) {
#ifdef THE_WINDOWS
        size_t l;
        errno_t err = ::wcstombs_s(&l, NULL, 0, src, len);
        if ((err != 0) || (l <= 0)) {
            throw exception("String conversion from wide to ANSI failed",
                __FILE__, __LINE__);
        }

        dst.resize(l, 0);
        VERIFY((::wcstombs_s(&l, const_cast<char *>(dst.data()), l, src, len)
            == 0));

#else /* THE_WINDOWS */
        size_t l = ::wcstombs(NULL, src, len);
        if (l <= 0) {
            throw exception("String conversion from wide to ANSI failed",
                __FILE__, __LINE__);
        }

        dst.resize(l, 0);
        VERIFY(::wcstombs(const_cast<char *>(dst.data()), src, l) == l);

#endif /* THE_WINDOWS */

    } else {

        dst.resize(len, 0);
        char *d = const_cast<char*>(dst.data());
        std::use_facet<std::ctype<wchar_t> >(
            the::text::locale_utility::locale()).narrow(src, src + len,
            defChar, d);

    }

    return dst;
}


/*
 * the::text::string_converter::convert
 */
the::wstring& the::text::string_converter::convert(the::wstring& dst,
        const char *src, const size_t len) {
    THE_STACK_TRACE;

    if ((src == NULL) || (len <= 0)) {
        dst.clear();
        return dst;
    }

    if (the::text::locale_utility::is_utf8_locale()) {
#ifdef THE_WINDOWS
        size_t l;
        errno_t err = ::mbstowcs_s(&l, NULL, 0, src, len);
        if ((err != 0) || (l <= 0)) {
            throw exception("String conversion from ANSI to wide failed",
                __FILE__, __LINE__);
        }

        dst.resize(l, 0);
        VERIFY((::mbstowcs_s(&l, const_cast<wchar_t*>(dst.data()), l, src, len)
            == 0));

#else /* THE_WINDOWS */
        size_t l = ::mbstowcs(NULL, src, len);
        if (l <= 0) {
            throw exception("String conversion from ANSI to wide failed",
                __FILE__, __LINE__);
        }

        dst.resize(l, 0);
        VERIFY(::mbstowcs(const_cast<wchar_t*>(dst.data()), src, len) == l);

#endif /* THE_WINDOWS */

    } else {

        dst.resize(len, 0);
        wchar_t *d = const_cast<wchar_t*>(dst.data());
        std::use_facet<std::ctype<wchar_t> >(
            the::text::locale_utility::locale()).widen(src, src + len, d);

    }

    return dst;
}


/*
 * the::text::string_converter::to_a
 */
char the::text::string_converter::to_a(wchar_t c) {
    THE_STACK_TRACE;
    return std::use_facet<std::ctype<wchar_t> >
        (the::text::locale_utility::locale()).narrow(c, '\000');
}


/*
 * the::text::string_converter::to_w
 */
wchar_t the::text::string_converter::to_w(char c) {
    THE_STACK_TRACE;
    return std::use_facet<std::ctype<wchar_t> >
        (the::text::locale_utility::locale()).widen(c);
}
