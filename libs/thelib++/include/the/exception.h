/*
 * the/exception.h
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
 * Exception.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#ifndef THE_EXCEPTION_H_INCLUDED
#define THE_EXCEPTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include <exception>
#include "the/tchar.h"
#include "the/types.h"


namespace the {

    /**
     * Superclass for all exceptions. The class provides basic functionality for
     * exception messages.
     *
     * @author Christoph Mueller
     */
    class exception : public std::exception {

    public:

        /**
         * Create a new exception. The ownership of the memory designated by
         * 'msg' and 'file' remains at the caller, the class creates a deep
         * copy.
         *
         * @param msg  A description of the exception.
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        exception(const char *msg, const char *file, const int line);

        /**
         * Create a new exception. The ownership of the memory designated by
         * 'msg' and 'file' remains at the caller, the class creates a deep
         * copy.
         *
         * @param msg  A description of the exception.
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        exception(const wchar_t *msg, const char *file, const int line);

        /**
         * Create a new exception. The ownership of the memory designated by
         * 'file' remains at the caller, the class creates a deep copy.
         *
         * @param file The file the exception was thrown in.
         * @param line The line the exception was thrown in.
         */
        exception(const char *file, const int line);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        exception(const exception& rhs);

        /** Dtor. */
        virtual ~exception(void) throw();

        /**
         * Answer the file the exception was thrown in. The onwnership of the
         * memory remains at the object.
         *
         * @return The file the exception was thrown in.
         */
        inline const char *get_file(void) const {
            return this->file;
        }

        /**
         * Answer the line the exception was thrown in.
         *
         * @return The line the exception was thrown in.
         */
        inline int get_line(void) const {
            return this->line;
        }

        /**
         * Answer the file the exception description text. The pointer returned
         * is valid until the next call to a GetMsg* method. The ownership of the
         * memory remains at the object.
         *
         * @return The exception message.
         */
        inline const TCHAR *get_msg(void) const {
#if defined(UNICODE) || defined(_UNICODE)
            return reinterpret_cast<const TCHAR *>(this->get_msg_wstr());
#else /* defined(UNICODE) || defined(_UNICODE) */
            return reinterpret_cast<const TCHAR *>(this->get_msg_astr());
#endif /* defined(UNICODE) || defined(_UNICODE) */
        }

        /**
         * Answer the file the exception description text. The pointer returned
         * is valid until the next call to a GetMsg* method. The ownership of the
         * memory remains at the object.
         *
         * @return The exception message.
         */
        virtual const char *get_msg_astr(void) const;

        /**
         * Answer the file the exception description text. The pointer returned
         * is valid until the next call to a GetMsg* method. The ownership of the
         * memory remains at the object.
         *
         * @return The exception message.
         */
        virtual const wchar_t *get_msg_wstr(void) const;

        /**
         * Answer the call stack trace from where the exception was thrown. If
         * no call stack is available an empty string is returned.
         *
         * @return The call stack trace from where the exception was thrown.
         */
        inline const char *get_stack(void) const {
            return (this->stack == NULL) ? "" : this->stack;
        }

        /**
         * Answer whether the exception knows the call stack trace from where
         * it was thrown.
         *
         * @return 'true' if a call stack trace is available,
         *         'false' otherwise.
         */
        inline bool has_stack(void) const {
            return this->stack != NULL;
        }

        /**
         * Answer the file the exception description text. The pointer returned
         * is valid until the next call to a GetMsg* method. The ownership of the
         * memory remains at the object.
         *
         * @return The exception message.
         */
        virtual const char * what() const throw() {
            return this->get_msg_astr();
        }

        /**
         * Assignment operator.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        virtual exception& operator =(const exception& rhs);

    protected:

        /**
         * Tries to fetch the current call stack trace.
         */
        void fetch_stack(void);

        /**
         * Set a new detail message.
         *
         * @param fmt The format string like in printf.
         * @param ... Additional parameters.
         */
        void format_msg(const char *fmt, ...);

        /**
         * Set a new detail message.
         *
         * @param fmt The format string like in printf.
         * @param ... Additional parameters.
         */
        void format_msg(const wchar_t *fmt, ...);

        /**
         * Set a new file.
         *
         * @param file The file name.
         */
        void set_file(const char *file);

        /**
         * Set a new detail message.
         *
         * @param msg The new exception detail message.
         */
        void set_msg(const char *msg) const;

        /**
         * Set a new detail message.
         *
         * @param msg The new exception detail message.
         */
        void set_msg(const wchar_t *msg) const;

    private:

        /** The file the exception was thrown in. */
        char *file;

        /** The line number the exception was thrown in. */
        int line;

        /** Remember whether 'msg' points to a Unicode or ANSI string. */
        mutable bool isMsgUnicode;

        /** The exception message. */
        mutable void *msg;

        /** The stack trace, if available */
        char *stack;

    };

} /* end namespace the */


/** 
 * Throws a THElib exception of the specified 'type' and appends the current
 * source location automatically.
 *
 * @param type The type of exception to be thrown.
 * @param ...  The parameters for the constructor of the exception except for
 *             the source location, which will be appended automatically.
 */
#define THROW_THE_EXCEPTION(type, ...)\
    throw type(__VA_ARGS__, __FILE__, __LINE__)

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_EXCEPTION_H_INCLUDED */
