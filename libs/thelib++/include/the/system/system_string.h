/*
 * include\the\system\system_string.h
 *
 * Copyright (C) 2012 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_SYSTEM_SYSTEM_STRING_H_INCLUDED
#define THE_SYSTEM_SYSTEM_STRING_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/string.h"
#include "the/tchar.h"

#include "the/text/string_converter.h"


namespace the {
namespace system {

    /**
     * Defines the character type that is internally used by the operating
     * system.
     *
     * Operating systems may use different character sets internally, e.g. 
     * Windows is using UNICODE down to the kernel level. The ANSI API is only
     * provided for backward compatibility with stone age technology and has a
     * negative performance impact because of the conversion that is required.
     * On the other hand, Linux uses ANSI internally. Hence, THElib must convert
     * all UNICODE input to ANSI in order to fulfil API call requests. To 
     * summarise, it is desirable to use the correct cahracter set in programmes
     * in order to prevent conversions. THElib provides predefined types and
     * functions to facilitate this.
     */
#if defined(THE_WINDOWS)
    typedef wchar_t system_char;
#elif defined(THE_LINUX)
    typedef char system_char;
#else /* defined(THE_WINDOWS) */
#error "system_char must be defined for this platform."
#endif /* defined(THE_WINDOWS) */

    /**
     * Defines the string type that is internally used by the operating
     * system.
     *
     * Operating systems may use different character sets internally, e.g. 
     * Windows is using UNICODE down to the kernel level. The ANSI API is only
     * provided for backward compatibility with stone age technology and has a
     * negative performance impact because of the conversion that is required.
     * On the other hand, Linux uses ANSI internally. Hence, THElib must convert
     * all UNICODE input to ANSI in order to fulfil API call requests. To 
     * summarise, it is desirable to use the correct cahracter set in programmes
     * in order to prevent conversions. THElib provides predefined types and
     * functions to facilitate this.
     */
#if defined(THE_WINDOWS)
    typedef wstring system_string;
#elif defined(THE_LINUX)
    typedef astring system_string;
#else /* defined(THE_WINDOWS) */
#error "system_string must be defined for this platform."
#endif /* defined(THE_WINDOWS) */

    /**
     * Makes a string constant to be defined in the character set internally
     * used by the operating system.
     *
     * Operating systems may use different character sets internally, e.g. 
     * Windows is using UNICODE down to the kernel level. The ANSI API is only
     * provided for backward compatibility with stone age technology and has a
     * negative performance impact because of the conversion that is required.
     * On the other hand, Linux uses ANSI internally. Hence, THElib must convert
     * all UNICODE input to ANSI in order to fulfil API call requests. To 
     * summarise, it is desirable to use the correct cahracter set in programmes
     * in order to prevent conversions. THElib provides predefined types and
     * functions to facilitate this.
     */
#if defined(THE_WINDOWS)
#define THE_SYSTEM_STRING_CONSTANT(str) L##str
#elif defined(THE_LINUX)
#define THE_SYSTEM_STRING_CONSTANT(str) str
#else /* defined(THE_WINDOWS) */
#error "THE_SYSTEM_STRING_CONSTANT must be defined for this platform."
#endif /* defined(THE_WINDOWS) */

#if defined(THE_WINDOWS)
#define THE_A2S(str) THE_A2W(str)
#define THE_T2S(str) THE_T2W(str)
#define THE_W2S(str) str

#define THE_S2A(str) THE_W2A(str)
#define THE_S2T(str) THE_W2T(str)
#define THE_S2W(str) str

#elif defined(THE_LINUX)
#define THE_A2S(str) str
#define THE_T2S(str) THE_T2A(str)
#define THE_W2S(str) THE_W2A(str)

#define THE_S2A(str) str
#define THE_S2T(str) THE_A2T(str)
#define THE_S2W(str) THE_A2W(str)
#else /* defined(THE_WINDOWS) */

#error "THE_A2S must be defined for this platform."
#error "THE_T2S must be defined for this platform."
#error "THE_W2S must be defined for this platform."

#error "THE_S2A must be defined for this platform."
#error "THE_S2T must be defined for this platform."
#error "THE_S2W must be defined for this platform."
#endif /* defined(THE_WINDOWS) */


} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_SYSTEM_STRING_H_INCLUDED */
