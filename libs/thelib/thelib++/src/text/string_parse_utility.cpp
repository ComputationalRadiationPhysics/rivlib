/*
 * the/text/string_parse_utility.cpp
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
#include "the/text/string_parse_utility.h"
#include "the/config.h"
#include "the/argument_null_exception.h"
#include "the/format_exception.h"
#include "the/invalid_operation_exception.h"
#include "the/string.h"


/*
 * the::text::string_parse_utility::parse_bool
 */
bool the::text::string_parse_utility::parse_bool(const char *str) {
    if (str == NULL) {
        throw argument_null_exception("str", __FILE__, __LINE__);
    }

    if (
#ifdef THE_WINDOWS
            (_stricmp("true", str) == 0) || (_stricmp("t", str) == 0) || 
            (_stricmp("yes", str) == 0) || (_stricmp("y", str) == 0) || 
            (_stricmp("on", str) == 0)
#else /* THE_WINDOWS */
            (strcasecmp("true", str) == 0) || (strcasecmp("t", str) == 0) ||
            (strcasecmp("yes", str) == 0) || (strcasecmp("y", str) == 0) || 
            (strcasecmp("on", str) == 0)
#endif /* THE_WINDOWS */
            ) {
        return true;
    }

    if (
#ifdef THE_WINDOWS
            (_stricmp("false", str) == 0) || (_stricmp("f", str) == 0) || 
            (_stricmp("no", str) == 0) || (_stricmp("n", str) == 0) || 
            (_stricmp("off", str) == 0)
#else /* THE_WINDOWS */
            (strcasecmp("false", str) == 0) || (strcasecmp("f", str) == 0) || 
            (strcasecmp("no", str) == 0) || (strcasecmp("n", str) == 0) || 
            (strcasecmp("off", str) == 0)
#endif /* THE_WINDOWS */
            ) {
        return false;
    }

    try {
        int i = parse_int(str);
        return (i != 0);
    } catch (...) {
    }

    throw format_exception("Cannot convert String to Boolean", __FILE__, 
        __LINE__);
}


/*
 * the::text::string_parse_utility::parse_bool
 */
bool the::text::string_parse_utility::parse_bool(const wchar_t *str) {
    if (str == NULL) {
        throw argument_null_exception("str", __FILE__, __LINE__);
    }

    if (
#ifdef THE_WINDOWS
            (_wcsicmp(L"true", str) == 0) || (_wcsicmp(L"t", str) == 0) || 
            (_wcsicmp(L"yes", str) == 0) || (_wcsicmp(L"y", str) == 0) || 
            (_wcsicmp(L"on", str) == 0)
#else /* THE_WINDOWS */
            (wcscasecmp(L"true", str) == 0) || (wcscasecmp(L"t", str) == 0) || 
            (wcscasecmp(L"yes", str) == 0) || (wcscasecmp(L"y", str) == 0) || 
            (wcscasecmp(L"on", str) == 0)
#endif /* THE_WINDOWS */
            ) {
        return true;
    }

    if (
#ifdef THE_WINDOWS
            (_wcsicmp(L"false", str) == 0) || (_wcsicmp(L"f", str) == 0) || 
            (_wcsicmp(L"no", str) == 0) || (_wcsicmp(L"n", str) == 0) || 
            (_wcsicmp(L"off", str) == 0)
#else /* THE_WINDOWS */
            (wcscasecmp(L"false", str) == 0) || (wcscasecmp(L"f", str) == 0) || 
            (wcscasecmp(L"no", str) == 0) || (wcscasecmp(L"n", str) == 0) || 
            (wcscasecmp(L"off", str) == 0)
#endif /* THE_WINDOWS */
            ) {
        return false;
    }

    try {
        int i = parse_int(str);
        return (i != 0);
    } catch (...) {
    }

    throw format_exception("Cannot convert String to Boolean", __FILE__, 
        __LINE__);
}


/*
 * the::text::string_parse_utility::parse_double
 */
double the::text::string_parse_utility::parse_double(const char *str) {
    double retval;
    if (str == NULL) {
        throw argument_null_exception("str", __FILE__, __LINE__);
    }
    
    if (
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
            sscanf_s
#else  /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            sscanf
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            (str, "%lf", &retval) != 1) {
        throw format_exception("Cannot convert String to Double", __FILE__, 
            __LINE__);
    }

    return retval;
}


/*
 * the::text::string_parse_utility::parse_double
 */
double the::text::string_parse_utility::parse_double(const wchar_t *str) {
    double retval;
    if (str == NULL) {
        throw argument_null_exception("str", __FILE__, __LINE__);
    }

    if (
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
            swscanf_s
#else  /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            swscanf
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            (str, L"%lf", &retval) != 1) {
        throw format_exception("Cannot convert String to Double", __FILE__, 
            __LINE__);
    }

    return retval;
}


/*
 * the::text::string_parse_utility::parse_int
 */
int the::text::string_parse_utility::parse_int(const char *str) {
    int retval;
    if (str == NULL) {
        throw argument_null_exception("str", __FILE__, __LINE__);
    }

    if (
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
            sscanf_s
#else  /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            sscanf
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            (str, "%d", &retval) != 1) {
        throw format_exception("Cannot convert String to Integer", __FILE__, 
            __LINE__);
    }
    
    return retval;
}


/*
 * the::text::string_parse_utility::parse_int
 */
int the::text::string_parse_utility::parse_int(const wchar_t *str) {
    int retval;
    if (str == NULL) {
        throw argument_null_exception("str", __FILE__, __LINE__);
    }

    if (
#if (defined(_MSC_VER) && (_MSC_VER >= 1400))
            swscanf_s
#else  /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            swscanf
#endif /* (defined(_MSC_VER) && (_MSC_VER >= 1400)) */
            (str, L"%d", &retval) != 1) {
        throw format_exception("Cannot convert String to Integer", __FILE__, 
            __LINE__);
    }
    
    return retval;
}


/*
 * the::text::string_parse_utility::string_parse_utility
 */
the::text::string_parse_utility::string_parse_utility(void) {
    throw the::invalid_operation_exception("string_parse_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_parse_utility::string_parse_utility
 */
the::text::string_parse_utility::string_parse_utility(
        const string_parse_utility& src) {
    throw the::invalid_operation_exception("string_parse_utility::ctor",
        __FILE__, __LINE__);
}


/*
 * the::text::string_parse_utility::~string_parse_utility
 */
the::text::string_parse_utility::~string_parse_utility(void) {
    throw the::invalid_operation_exception("string_parse_utility::dtor",
        __FILE__, __LINE__);
}
