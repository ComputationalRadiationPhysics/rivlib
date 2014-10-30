/*
 * the/text/locale_utility.cpp
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

#include "the/text/locale_utility.h"
#include "the/invalid_operation_exception.h"
#include "the/config.h"
#if !defined(THE_WINDOWS) && !defined(NO_LANGINFO_H)
#include <langinfo.h>
#endif /* !THE_WINDOWS && !NO_LANGINFO_H */


/*
 * the::text::locale_utility::test_if_utf8_locale
 */
bool the::text::locale_utility::test_if_utf8_locale(void) {
#ifdef THE_WINDOWS
    // windows does not something terrible like this
    return false;
//#elif _LIN32
#else
    // linux needs to be tested
    // Code based on:
    // http://www.cl.cam.ac.uk/~mgk25/unicode.html#c
    char *str;

    // Test 1:
    // on any X/Open compliant systems, where <langinfo.h> is available
#ifndef NO_LANGINFO_H
    str = nl_langinfo(CODESET);
    if ((str != NULL) && (*str != 0)) {
        if (::strcasestr(str, "UTF-8")) {
            return true;
        } else {
            return false;
        }
    }
#endif /* !NO_LANGINFO_H */

    // If you are really concerned that calling nl_langinfo() might not be
    // portable enough, there is also Markus Kuhn’s portable public domain
    // nl_langinfo(CODESET) emulator for systems that do not have the real
    // thing (and another one from Bruno Haible), and you can use the
    // norm_charmap() function to standardize the output of the
    // nl_langinfo(CODESET) on different platforms.

    /*
    // (Bruno Haible suggested) ugli test if nl_langinfo is available.
    #serial AM1

    dnl From Bruno Haible.

    AC_DEFUN([AM_LANGINFO_CODESET],
    [
      AC_CACHE_CHECK([for nl_langinfo and CODESET], am_cv_langinfo_codeset,
        [AC_TRY_LINK([#include <langinfo.h>],
          [char* cs = nl_langinfo(CODESET);],
          am_cv_langinfo_codeset=yes,
          am_cv_langinfo_codeset=no)
        ])
      if test $am_cv_langinfo_codeset = yes; then
        AC_DEFINE(HAVE_LANGINFO_CODESET, 1,
          [Define if you have <langinfo.h> and nl_langinfo(CODESET).])
      fi
    ])
    */

    // Alternative Test:
    // only works if 'UTF-8' is a part of the locale name (which is not always
    // the case ???)
    if (((str = ::getenv("LC_ALL")) && *str) ||
            ((str = ::getenv("LC_CTYPE")) && *str) ||
            ((str = ::getenv("LANG")) && *str)) {
        if (::strcasestr(str, "UTF-8")) {
            return true;
        }
    }

    return false;

//#else
//    // non-linux is not supported ATM, so let's assume it is ok.
//    // TODO: Problem for possible MacOS port
//    return false;
#endif
}


/*
 * the::text::locale_utility::loc
 */
std::locale the::text::locale_utility::loc(
    the::text::locale_utility::init_system_locale());


/*
 * the::text::locale_utility::isUtf8Locale
 */ 
int the::text::locale_utility::isUtf8Locale = -1;


/*
 * the::text::locale_utility::init_system_locale
 */
const char *the::text::locale_utility::init_system_locale() {
    std::locale::global(std::locale(""));
    return "";
}


/*
 * the::text::locale_utility::locale_utility
 */
the::text::locale_utility::locale_utility(void) {
    throw the::invalid_operation_exception("locale_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::locale_utility::locale_utility
 */
the::text::locale_utility::locale_utility(
        const the::text::locale_utility& src) {
    throw the::invalid_operation_exception("locale_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::locale_utility::~locale_utility
 */
the::text::locale_utility::~locale_utility(void) {
    throw the::invalid_operation_exception("locale_utility::dtor",
        __FILE__, __LINE__);
}
