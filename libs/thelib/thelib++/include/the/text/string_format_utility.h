/*
 * the/text/string_format_utility.h
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
 * CharTraits.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 - 2006 by Christoph Mueller. All rights reserved.
 */
#ifndef THE_TEXT_STRING_FORMAT_UTILITY_H_INCLUDED
#define THE_TEXT_STRING_FORMAT_UTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#ifndef THE_STRING_H_INCLUDED
#error "the/string" must be included before including "the/string_converter"
#endif /* THE_STRING_H_INCLUDED */

#include <stdarg.h>
#include "the/types.h"

namespace the {
namespace text {

    /**
     * Helper methods for string formatting. Do not use! Use
     *  the::text::string_builder::format
     * instead
     */
    class string_format_utility {
    public:

        /**
         * Prints the formatted string 'fmt' to the buffer 'dst' having at
         * least 'cnt' characters.
         *
         * If 'dst' is a NULL pointer or 'cnt' is 0, the method just counts
         * how large 'dst' should be for 'fmt' and the specified parameters.
         *
         * The number of characters returned does not include the terminating
         * zero character.
         *
         * In case of an error, e. g. if 'dst' is too small or 'fmt' is a NULL
         * pointer, the method returns -1;
         *
         * If 'dst' is not NULL and 'cnt' is greater than zero, the method 
         * ensures that the resulting string in 'dst' is zero terminated, 
         * regardless of its content.
         *
         * @param dst    The buffer receiving the formatted string. This
         *               buffer must be able to hold at least 'cnt' characters
         *               or must be NULL.
         * @param cnt    The number of characters that fit into 'dst'.
         * @param fmt    The format string.
         * @param argptr The variable argument list.
         *
         * @return The number of characters written, not including the
         *         trailing zero, or -1 in case of an error like 'dst' being
         *         too small or 'fmt' being a NULL pointer.
         */
        static size_t formatVa(char *dst, size_t cnt, const char *fmt,
            va_list argPtr);

        /**
         * Prints the formatted string 'fmt' to the buffer 'dst' having at
         * least 'cnt' characters.
         *
         * If 'dst' is a NULL pointer or 'cnt' is 0, the method just counts
         * how large 'dst' should be for 'fmt' and the specified parameters.
         *
         * The number of characters returned does not include the terminating
         * zero character.
         *
         * In case of an error, e. g. if 'dst' is too small or 'fmt' is a NULL
         * pointer, the method returns -1;
         *
         * If 'dst' is not NULL and 'cnt' is greater than zero, the method 
         * ensures that the resulting string in 'dst' is zero terminated, 
         * regardless of its content.
         *
         * @param dst    The buffer receiving the formatted string. This
         *               buffer must be able to hold at least 'cnt' characters
         *               or must be NULL.
         * @param cnt    The number of characters that fit into 'dst'.
         * @param fmt    The format string.
         * @param argptr The variable argument list.
         *
         * @return The number of characters written, not including the
         *         trailing zero, or -1 in case of an error like 'dst' being
         *         too small or 'fmt' being a NULL pointer.
         */
        static size_t formatVa(wchar_t *dst, size_t cnt, const wchar_t *fmt,
            va_list argPtr);

    protected:

        /** forbidden Ctor */
        string_format_utility(void);

        /** forbidden Copy Ctor */
        string_format_utility(const string_format_utility& src);

        /** forbidden Dtor */
        ~string_format_utility(void);

    private:
    };

} /* end namespace text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_STRING_FORMAT_UTILITY_H_INCLUDED */
