/*
 * the/text/locale_utility.h
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

#ifndef THE_TEXT_LOCALEUTILITY_H_INCLUDED
#define THE_TEXT_LOCALEUTILITY_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <locale>
#include "the/assert.h"
#include "the/memory.h"
#include "the/types.h"


namespace the {
namespace text {

    /**
     * Utility class to manage std::locale function extensions
     */
    class locale_utility {
    public:

        /**
         * Access to the stored locale object
         *
         * @oaram forceRenew If true the stored locale object is renewed to
         *                   reflect the global locale
         *
         * @return The stored locale object
         */
        static inline std::locale& locale(bool forceRenew = false) {
            if (forceRenew) {
                loc = get_global_locale();
            }
            return loc;
        }

        /**
         * Creates a locale object with the system default locale, the global
         * locale also used by c functions
         *
         * @return A locale object with the global locale
         */
        static inline std::locale get_global_locale(void) {
            return std::locale("");
        }

        /**
         * Tests if the system uses a global UTF8 locale
         *
         * @param forceReevaluate If true 'TestIfUTF8Locale' will always be called
         *
         * @return True if the system uses a global UTF8 locale
         */
        static inline bool is_utf8_locale(bool forceReevaluate = false) {
            if ((isUtf8Locale < 0) || forceReevaluate) {
                isUtf8Locale = test_if_utf8_locale() ? 1 : 0;
            }
            return (isUtf8Locale != 0);
        }

        /**
         * Evaluates if the system uses a global UTF8 locale
         *
         * @return True if the system uses a global UTF8 locale
         */
        static bool test_if_utf8_locale(void);

    private:

        /** The object locale */
        static std::locale loc;

        /** Flag if a utf8 locale is used */
        static int isUtf8Locale;

        /**
         * This may be not a good idea for applications which sanely change
         * the C locale, but for now it is the best idea to activate the
         * system locale for an application at startup
         *
         * @return An empty string
         */
        static const char *init_system_locale();

        /** forbidden ctor */
        locale_utility(void);

        /** forbidden copy ctor */
        locale_utility(const locale_utility& src);

        /** forbidden dtor */
        ~locale_utility(void);

    };


} /* end namesapce text */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_TEXT_LOCALEUTILITY_H_INCLUDED */
