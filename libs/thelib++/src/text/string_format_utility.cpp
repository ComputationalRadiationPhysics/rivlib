/*
 * the/text/string_format_utility.cpp
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
/*
 * CharTraits.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */
#include "the/string.h"
#include "the/text/string_format_utility.h"
#include "the/config.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"


/*
 * the::text::string_format_utility::formatVa
 */
size_t the::text::string_format_utility::formatVa(char *dst, size_t cnt,
        const char *fmt, va_list argptr) {
    size_t retval = -1;
    va_list lap;

#ifdef THE_WINDOWS
    if ((dst == NULL) || (cnt <= 0)) {
        /* Answer the prospective size of the string. */
        va_copy(lap, argptr);
        retval = ::_vscprintf(fmt, lap);
        va_end(lap);

    } else {
        va_copy(lap, argptr);
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
        retval = ::_vsnprintf_s(dst, cnt, cnt, fmt, lap);
#else /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
        retval = ::_vsnprintf(dst, cnt, fmt, lap);
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
        va_end(lap);
    } /* end if ((dst == NULL) || (cnt <= 0)) */

#else /* THE_WINDOWS */
    va_copy(lap, argptr);
    retval = ::vsnprintf(dst, cnt, fmt, lap);
    va_end(lap);

    if ((dst != NULL) && (cnt > 0) && (retval > cnt - 1)) {
        retval = -1;
    }
#endif /* THE_WINDOWS */ 

    /* Ensure string being terminated. */
    if ((dst != NULL) && (cnt > 0)) {
        dst[cnt - 1] = 0;
    }
    return retval;
}


/*
 * the::text::string_format_utility::formatVa
 */
size_t the::text::string_format_utility::formatVa(wchar_t *dst, size_t cnt,
        const wchar_t *fmt, va_list argptr) {
    size_t retval = -1;
    va_list lap;

#ifdef THE_WINDOWS
    if ((dst == NULL) || (cnt <= 0)) {
        /* Answer the prospective size of the string. */
        va_copy(lap, argptr);
        retval = ::_vscwprintf(fmt, lap);
        va_end(lap);

    } else {
        va_copy(lap, argptr);
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
        retval = ::_vsnwprintf_s(dst, cnt, cnt, fmt, lap);
#else /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
        retval = ::_vsnwprintf(dst, cnt, fmt, lap);
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
        va_end(lap);
    } /* end if ((dst == NULL) || (cnt <= 0)) */

#else /* THE_WINDOWS */
    // Yes, you can trust your eyes: The char and wide char implementations
    // under Linux have a completely different semantics. vswprintf cannot 
    // be used for determining the required size as vsprintf can.
    size_t bufferSize, bufferGrow;
    wchar_t *buffer = NULL;

    if ((dst == NULL) || (cnt <= 0)) {
        /* Just count. */
        bufferSize = static_cast<size_t>(1.1 * static_cast<float>(
            ::wcslen(fmt)) + 1);
        bufferGrow = static_cast<size_t>(0.5 * static_cast<float>(
            bufferSize));
        buffer = new wchar_t[bufferSize];

        va_copy(lap, argptr);
        while ((retval = ::vswprintf(buffer, bufferSize, fmt, lap)) 
                == -1) {
            va_end(lap);
            va_copy(lap, argptr);
            safe_array_delete(buffer);
            bufferSize += bufferGrow;
            buffer = new wchar_t[bufferSize];
        }
        va_end(lap);

        retval = ::wcslen(buffer);
        safe_array_delete(buffer);
            
    } else {
        /* Format the string. */
        va_copy(lap, argptr);
        retval = ::vswprintf(dst, cnt, fmt, lap);
        va_end(lap);
    }
#endif /* THE_WINDOWS */

    /* Ensure string being terminated. */
    if ((dst != NULL) && (cnt > 0)) {
        dst[cnt - 1] = 0;
    }
    return retval;
}


/*
 * the::text::string_format_utility::string_format_utility
 */
the::text::string_format_utility::string_format_utility(void) {
    throw the::invalid_operation_exception("string_format_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_format_utility::string_format_utility
 */
the::text::string_format_utility::string_format_utility(
        const string_format_utility& src) {
    throw the::invalid_operation_exception("StringFormatUtility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_format_utility::~string_format_utility
 */
the::text::string_format_utility::~string_format_utility(void) {
    throw the::invalid_operation_exception("string_format_utility::dtor",
        __FILE__, __LINE__);
}
