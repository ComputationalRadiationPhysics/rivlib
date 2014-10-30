/*
 * the/unreferenced.h
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
 * unreferenced.h
 *
 * Copyright (C) 2009 by Visualisierungsinstitut der Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 * Copyright (C) 2009 by Christoph Müller. Alle Rechte vorbehalten.
 */

#ifndef THE_UNREFERENCED_H_INCLUDED
#define THE_UNREFERENCED_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */


/**
 * Define the parameter 'p' as unreferenced on purpose. Using this macro
 * prevents compiler warnings.
 *
 * @param p The parameter that is not used.
 */
#ifdef UNREFERENCED_PARAMETER
#define THE_UNREFERENCED_PARAMETER(p) UNREFERENCED_PARAMETER(p)
#else /* UNREFERENCED_PARAMETER */
#define THE_UNREFERENCED_PARAMETER(p) ((void) (p))
#endif /* UNREFERENCED_PARAMETER */


/**
 * Define the variable 'v' as unreferenced on purpose. Using this macro
 * prevents compiler warnings.
 *
 * @param v The local variable that is not used.
 */
#define THE_UNREFERENCED_LOCAL_VARIABLE(v) THE_UNREFERENCED_PARAMETER(v)


/**
 * Define the parameter 'p' as only referenced in the debug build. Using this
 * macro prevents compiler warnings.
 *
 * @param p The parameter that is only referenced in debug builds.
 */
#if (!defined(DEBUG) && !defined(_DEBUG))
#define THE_DBGONLY_REFERENCED_PARAMETER(p) THE_UNREFERENCED_PARAMETER(p)
#else /* (!defined(DEBUG) && !defined(_DEBUG)) */
#define THE_DBGONLY_REFERENCED_PARAMETER(p)
#endif /* (!defined(DEBUG) && !defined(_DEBUG)) */


/**
 * Define the variable 'v' as only referenced in the debug build. Using this 
 * macro prevents compiler warnings.
 *
 * @param v The local that is only referenced in debug builds.
 */
#define THE_DBGONLY_REFERENCED_LOCAL_VARIABLE(v) \
    THE_DBGONLY_REFERENCED_PARAMETER(v)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_UNREFERENCED_H_INCLUDED */
