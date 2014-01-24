/*
 * the/system/system_exception.h
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
 * SystemException.h  09.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_SYSTEM_EXCEPTION_H_INCLUDED
#define THE_SYSTEM_SYSTEM_EXCEPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/system/system_error.h"
#include "the/system/system_message.h"
#include "the/exception.h"


namespace the {
namespace system {

    /**
     * This exception class represents a system error. It is instantiated with 
     * a system error code and retrieves the error message from the system.
     *
     * @author Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de)
     */
    class system_exception : public exception {

    public:

        /** The type of an error code. */
        typedef system_error::native_error_type native_error_type;

        /**
         * Ctor.
         *
         * @param errorCode A system dependent error code.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        system_exception(const native_error_type errorCode, const char *file,
            const int line);

        /**
         * Ctor.
         *
         * @param errorCode A system dependent error code.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        system_exception(const system_error& errorCode, const char *file,
            const int line);

        /**
         * Create a new exception with the current error code retrieved using
         * ::GetLastError() on Windows or from ::errno on Linux.
         *
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        system_exception(const char *file, const int line);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        system_exception(const system_exception& rhs);

        /** Dtor. */
        virtual ~system_exception(void) throw();

        /**
         * Answer the system-dependent error code associated with this 
         * exception.
         *
         * @return The system error code.
         */
        inline system_error get_error(void) const {
            return this->sysMsg.get_error();
        }

        /**
         * Answer the human-readable exception description.
         *
         * @return The exception message.
         *
         * @throws the::system::system_exception If the message for the error 
         *                                       code could not be retrieved.
         */
        virtual const char *get_msg_astr(void) const;

        /**
         * Answer the human-readable exception description.
         *
         * @return The exception message.
         *
         * @throws the::system::system_exception If the message for the error 
         *                                       code could not be retrieved.
         */
        virtual const wchar_t *get_msg_wstr(void) const;

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        system_exception& operator =(const system_exception& rhs);

    protected:

        /** Super class typedef. */
        typedef exception base;

    private:

        /** 
         * The system message for the error code. This member is used for 
         * answering error messages rather than the buffer from the base class.
         */
        system_message sysMsg;
    };

} /* end namespace system */
} /* end namespace the */


/**
 * Throws a the::system::system_exception with the last system error code.
 */
#define THROW_THE_SYSTEM_EXCEPTION throw the::system::system_exception(\
    __FILE__, __LINE__)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_SYSTEM_EXCEPTION_H_INCLUDED */
