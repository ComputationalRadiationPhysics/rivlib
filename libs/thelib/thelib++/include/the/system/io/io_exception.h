/*
 * include\the\system\io\io_exception.h
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
 * IOException.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_IO_EXCEPTION_H_INCLUDED
#define THE_IO_EXCEPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/system/system_exception.h"


namespace the {
namespace system {
namespace io {

    /**
     * This exception indicates an I/O error.
     *
     * @author Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de)
     */
    class io_exception : public the::system::system_exception {

    public:

        /**
         * Initialises a new instance using the last error stored by
         * the system.
         *
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        io_exception(const char *file, const int line);

        /**
         * Initialises a new instance.
         *
         * @param errorCode A system dependent error code.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        io_exception(const native_error_type errorCode, const char *file,
            const int line);

        /**
         * Initialises a new instance.
         *
         * @param errorCode A system dependent error code.
         * @param msg       A detailed error message.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        io_exception(const native_error_type errorCode, const char *msg,
            const char *file, const int line);

        /**
         * Initialises a new instance.
         *
         * @param errorCode A system dependent error code.
         * @param msg       A detailed error message.
         * @param file      The file the exception was thrown in.
         * @param line      The line the exception was thrown in.
         */
        io_exception(const native_error_type errorCode, const wchar_t *msg,
            const char *file, const int line);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        io_exception(const io_exception& rhs);

        /** Dtor. */
        virtual ~io_exception(void) throw();

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        io_exception& operator =(const io_exception& rhs);

    protected:

        /** Super class typedef. */
        typedef the::system::system_exception base;

    };

} /* end namespace io */
} /* end namespace system */
} /* end namespace the */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_IO_EXCEPTION_H_INCLUDED */
