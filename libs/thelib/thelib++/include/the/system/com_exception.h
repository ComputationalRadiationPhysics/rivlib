/*
 * include\the\system\com_exception.h
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
/*
 * COMException.h
 *
 * Copyright (C) 2006 - 2012 by Visualisierungsinstitut Universitaet Stuttgart. 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_COM_EXCEPTION_H_INCLUDED
#define THE_SYSTEM_COM_EXCEPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <comdef.h>
#endif /* THE_WINDOWS */

#include "the/exception.h"
#include "the/string.h"


namespace the {
namespace system {

    /**
     * This exception represents a COM error code (HRESULT).
     */
    class com_exception : public the::exception {

    public:

        /** The type of a COM error code. */
#if defined(THE_WINDOWS)
        typedef HRESULT native_error_type;
#else /* defined(THE_WINDOWS) */
        typedef int native_error_type;
#endif /* defined(THE_WINDOWS) */

        /**
         * Return the error message for 'hr' into 'outMsg'.
         *
         * @param outMsg Receives the error message and will be returned.
         * @param hr     The error code to get the message for.
         *
         * @return 'outMsg'.
         */
        static astring& get_message(astring& outMsg, 
            const native_error_type hr);

        /**
         * Return the error message for 'hr' into 'outMsg'.
         *
         * @param outMsg Receives the error message and will be returned.
         * @param hr     The error code to get the message for.
         *
         * @return 'outMsg'.
         */
        static wstring& get_message(wstring& outMsg, 
            const native_error_type hr);

        /**
         * Initialises a new instance.
         *
         * @param hr   A COM error code.
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        com_exception(const native_error_type hr, const char *file, 
            const int line);

        /**
         * Initialises a new instance with a custom error message.
         *
         * @param hr   A COM error code.
         * @param msg  The custom error message.
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        com_exception(const native_error_type hr, const char *msg,
            const char *file, const int line);

        /**
         * Initialises a new instance with a custom error message.
         *
         * @param hr   A COM error code.
         * @param msg  The custom error message.
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        com_exception(const native_error_type hr, const wchar_t *msg,
            const char *file, const int line);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        com_exception(const com_exception& rhs);

        /** Dtor. */
        virtual ~com_exception(void) throw();

        /**
         * Gets the COM error code associated with the exception.
         *
         * @return The error code.
         */
        inline native_error_type get_error_code(void) const {
            return this->hr;
        }

        /**
         * Assign value of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        com_exception& operator =(const com_exception& rhs);

    protected:

        /** Super class typedef. */
        typedef the::exception base;

        /** The COM error code. */
        native_error_type hr;
    };

} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_COM_EXCEPTION_H_INCLUDED */
