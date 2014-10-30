/*
 * the/text/string_case_utility.cpp
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
#include "the/text/string_case_utility.h"
#include "the/config.h"
#ifndef THE_WINDOWS
#include <algorithm>
#endif /* THE_WINDOWS */
#include "the/invalid_operation_exception.h"
#ifndef THE_WINDOWS
#include "the/text/locale_utility.h"
#include "the/text/string_converter.h"
#endif /* THE_WINDOWS */

#ifndef THE_WINDOWS

/*
 * the::text::string_case_utility::to_lower_case
 */
the::astring& the::text::string_case_utility::to_lower_case(the::astring& str) {
    if (the::text::locale_utility::is_utf8_locale()) {
        // Now, this is just great what Linux did here!
        the::wstring wStr;
        the::text::string_converter::convert(wStr, str);
        to_lower_case(wStr);
        the::text::string_converter::convert(str, wStr);

    } else {
        size_t len = str.length();
        for (size_t i = 0U; i < len; i++) {
            str[i] = char_utility::to_lower_case(str[i]);
        }
    }
    return str;
}


/*
 * the::text::string_case_utility::to_upper_case
 */
the::astring& the::text::string_case_utility::to_upper_case(the::astring& str) {
    if (the::text::locale_utility::is_utf8_locale()) {
        // Now, this is just great what Linux did here!
        the::wstring wStr;
        the::text::string_converter::convert(wStr, str);
        to_upper_case(wStr);
        the::text::string_converter::convert(str, wStr);

    } else {
        size_t len = str.length();
        for (size_t i = 0U; i < len; i++) {
            str[i] = char_utility::to_upper_case(str[i]);
        }
    }
    return str;
}


/*
 * the::text::string_case_utility::to_lower_case
 */
void the::text::string_case_utility::to_lower_case(char *dst, size_t cnt,
        const char *src) {
    if (the::text::locale_utility::is_utf8_locale()) {
        // Now, this is just great what Linux did here!
        the::wstring wStr;
        the::text::string_converter::convert(wStr, src);
        to_lower_case(wStr);
        the::astring aStr;
        the::text::string_converter::convert(aStr, wStr);
        if (aStr.length() < cnt) cnt = aStr.length() + 1;
        ::memcpy(dst, aStr.c_str(), cnt);

    } else {
        for (size_t i = 0; i < cnt; i++) {
            if (src[i] == 0) {
                dst[i] = 0;
                break;
            }
            dst[i] = char_utility::to_lower_case(src[i]);
        }

    }

}


/*
 * the::text::string_case_utility::to_upper_case
 */
void the::text::string_case_utility::to_upper_case(char *dst, size_t cnt,
        const char *src) {
    if (the::text::locale_utility::is_utf8_locale()) {
        // Now, this is just great what Linux did here!
        the::wstring wStr;
        the::text::string_converter::convert(wStr, src);
        to_upper_case(wStr);
        the::astring aStr;
        the::text::string_converter::convert(aStr, wStr);
        if (aStr.length() < cnt) cnt = aStr.length() + 1;
        ::memcpy(dst, aStr.c_str(), cnt);

    } else {
        for (size_t i = 0; i < cnt; i++) {
            if (src[i] == 0) {
                dst[i] = 0;
                break;
            }
            dst[i] = char_utility::to_upper_case(src[i]);
        }

    }

}

#endif


/*
 * the::text::string_case_utility::string_case_utility
 */
the::text::string_case_utility::string_case_utility(void) {
    throw the::invalid_operation_exception("string_case_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_case_utility::string_case_utility
 */
the::text::string_case_utility::string_case_utility(
        const string_case_utility& src) {
    throw the::invalid_operation_exception("string_case_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_case_utility::~string_case_utility
 */
the::text::string_case_utility::~string_case_utility(void) {
    throw the::invalid_operation_exception("string_case_utility::dtor",
        __FILE__, __LINE__);
}
