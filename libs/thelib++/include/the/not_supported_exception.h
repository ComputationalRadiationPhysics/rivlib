/*
 * the/not_supported_exception.h
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
 * not_supported_exception.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#ifndef THE_NOT_SUPPORTED_EXCEPTION_H_INCLUDED
#define THE_NOT_SUPPORTED_EXCEPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/exception.h"


namespace the {

    /**
     * This exception indicates that a function or method must not be called.
     *
     * @author Christoph Mueller
     */
    class not_supported_exception : public exception {

    public:

        /**
         * Ctor.
         *
         * @param funcName Name of the unsupported method or function.
         * @param file     The file the exception was thrown in.
         * @param line     The line the exception was thrown in.
         */
        not_supported_exception(const char *funcName, const char *file, 
            const int line);

        /**
         * Ctor.
         *
         * @param funcName Name of the unsupported method or function.
         * @param file     The file the exception was thrown in.
         * @param line     The line the exception was thrown in.
         */
        not_supported_exception(const wchar_t *funcName, const char *file, 
            const int line);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        not_supported_exception(const not_supported_exception& rhs);

        /** Dtor. */
        virtual ~not_supported_exception(void) throw();

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        virtual not_supported_exception& operator =(
            const not_supported_exception& rhs);
    };
}
 

/** Throws a not_supported_exception for the calling method. */
#define THROW_THE_NOT_SUPPORTED_EXCEPTION\
    THROW_THE_EXCEPTION(the::not_supported_exception, THE_FUNCTION)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_NOT_SUPPORTEDE_XCEPTION_H_INCLUDED */
