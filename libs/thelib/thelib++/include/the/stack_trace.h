/*
 * the/stack_trace.h
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
 * StackTrace.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */
#ifndef THE_STACK_TRACE_H_INCLUDED
#define THE_STACK_TRACE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <memory>


namespace the {

    /** forward declaration */
    class THELIB_SYMBOLEXPORT stack_trace;

} /* end namespace the */

/**
 * Forces generation an export of template class
 * http://support.microsoft.com/default.aspx?scid=KB;EN-US;168958
 */
THELIB_SYMBOLEXTERN template class THELIB_SYMBOLEXPORT std::shared_ptr<the::stack_trace>;

namespace the {

    /**
     * The manager class for stack trace.
     *
     * Each module of your application must initialise the stack trace manager
     * object by calling 'the::stack_trace::initialise', however, you must
     * ensure that only a single manager object is created and is used by all
     * of your modules to get correct results.
     *
     * If your application is multi-threaded you must use
     * 'the::sys::threadsafe_stack_trace::initialise'. Otherwise the stack
     * returned may be wrong or corrupted.
     *
     * You should use the 'THE_STACK_TRACE' macro to push functions onto
     * the tracing stack.
     */
    class THELIB_SYMBOLEXPORT stack_trace {
    public:

        /**
         * Stack trace marker used to automatically push trace information
         * for functions. You should not create instances of this type on the
         * heap!
         */
        class THELIB_SYMBOLEXPORT marker {
        public:

            /**
             * Ctor.
             * Create this object to push 'functionName' on the trace stack.
             * It is automatically popped when this object is destroied.
             *
             * @param functionName The name to be pushed onto the trace stack.
             * @param file The file where is marker is placed (use '__FILE__')
             * @param line The line number where is marker is placed
             *             (use '__LINE__')
             */
            marker(const char* functionName, const char* file, const int line);

            /**
             * Dtor.
             */
            ~marker(void);

        private:

            /**
             * Forbidden copy ctor.
             *
             * @param src The object to clone from.
             */
            marker(const marker& src);

            /**
             * Forbidden assignment operator.
             *
             * @param rhs The right hand side operand.
             *
             * @return Reference to 'this'.
             */
            marker& operator=(const marker& rhs);

            /** The pushed position id */
            int id;

        };

        /**
         * Gets the whole current stack.
         *
         * If 'outStr' is NULL 'strSize' receives the required size in
         * characters to store the whole stack (including the terminating
         * zero).
         * If 'outStr' is not NULL 'strSize' specifies the size of the
         * buffer 'outStr' is pointing to. The method will only write that
         * amount of characters to the buffer. The content of 'strSize' will
         * be changed to the number of characters actually written (including
         * the terminating zero).
         *
         * @param outStr The buffer to recieve the stack.
         * @param strSize The size of the buffer.
         */
        static void get_stack_string(char *str, unsigned int &strSize);

        /**
         * Gets the whole current stack.
         *
         * If 'outStr' is NULL 'strSize' receives the required size in
         * characters to store the whole stack (including the terminating
         * zero).
         * If 'outStr' is not NULL 'strSize' specifies the size of the
         * buffer 'outStr' is pointing to. The method will only write that
         * amount of characters to the buffer. The content of 'strSize' will
         * be changed to the number of characters actually written (including
         * the terminating zero).
         *
         * @param outStr The buffer to recieve the stack.
         * @param strSize The size of the buffer.
         */
        static void get_stack_string(wchar_t *str, unsigned int &strSize);

        /**
         * Initialises the stack trace. You can specify the manager object for
         * the stack trace. If you do not specify the manager object, a new
         * instance will be created.
         *
         * It is usful to specify the manager object if your application
         * consists of multiple modules each linking against an own instance
         * of the vislib. In that case each modul must initialise the stack
         * trace by itself, but you should only use a single manager object.
         *
         * @param manager The manager object to be used for stack trace.
         * @param force If 'Initialise' has already been called for this
         *              module this flag controls whether the previous
         *              manager object is keept ('false') or if the new one
         *              (parameter 'manager') is used ('true') and the old
         *              one is removed.
         *
         * @return 'true' if the initialisation was successful and the object
         *         specified by 'manager' is now used to manage the stack
         *         trace; 'false' otherwise.
         */
        static bool initialise(std::shared_ptr<stack_trace> manager,
            bool force = false);

        // GCC 4.7.2 does not accept a nullptr as default parameter for 'manager'.
        static inline bool initialise(void) {
            return stack_trace::initialise(std::shared_ptr<stack_trace>(),
                false);
        }

        /**
         * Answer the manager object used by this modul.
         *
         * @return The manager object used by this modul.
         */
        static std::shared_ptr<stack_trace> Manager(void);

        /**
         * Dtor.
         *
         * Note: Do not call this dtor directly or directly destroy any
         * instance of this class. The 'SmartPtr' objects will handle the
         * deletion of the 'stack_trace' objects automatically.
         */
        virtual ~stack_trace(void);

        /** The marker object can access the stack */
        friend class marker;

    protected:

        /**
         * Private nested helper class building up the stack
         */
        typedef struct _stackelement_t {

            /** The function name */
            const char* func;

            /** The file name */
            const char* file;

            /** The line */
            int line;

            /** The next element on the stack */
            struct _stackelement_t *next;

        } stack_element;

        /** Ctor. */
        stack_trace(void);

        /**
         * Gets the whole current stack.
         *
         * If 'outStr' is NULL 'strSize' receives the required size in
         * characters to store the whole stack (including the terminating
         * zero).
         * If 'outStr' is not NULL 'strSize' specifies the size of the
         * buffer 'outStr' is pointing to. The method will only write that
         * amount of characters to the buffer. The content of 'strSize' will
         * be changed to the number of characters actually written (including
         * the terminating zero).
         *
         * @param outStr The buffer to recieve the stack.
         * @param strSize The size of the buffer.
         */
        virtual void get_stack_string_impl(char *str, unsigned int &strSize);

        /**
         * Gets the whole current stack.
         *
         * If 'outStr' is NULL 'strSize' receives the required size in
         * characters to store the whole stack (including the terminating
         * zero).
         * If 'outStr' is not NULL 'strSize' specifies the size of the
         * buffer 'outStr' is pointing to. The method will only write that
         * amount of characters to the buffer. The content of 'strSize' will
         * be changed to the number of characters actually written (including
         * the terminating zero).
         *
         * @param outStr The buffer to recieve the stack.
         * @param strSize The size of the buffer.
         */
        virtual void get_stack_string_impl(wchar_t *str,
            unsigned int &strSize);

        /**
         * Pops a position from the stack.
         *
         * @param id The id of the position to be popped
         *           (used for sanity checks).
         */
        virtual void pop(int id);

        /**
         * Pushes a position onto the stack.
         *
         * @param func The name of the function.
         * @param file The name of the file.
         * @param line The line in the file.
         *
         * @return The id of the pushed position.
         */
        virtual int push(const char* func, const char* file, const int line);

        /**
         * Gets the anchor element to the stack and starts using this stack.
         *
         * @return The anchor element to the stack.
         */
        virtual stack_element* start_use_stack(void);

        /**
         * Stops using the stack.
         *
         * @param stack The stack
         */
        virtual void stop_use_stack(stack_element* stack);

        /** The anchor pointer of the stack */
        stack_element *stack;

    private:

        /** The stack trace manager of this module */
        static std::shared_ptr<stack_trace> manager;

    };

} /* end namespace the */


#ifdef THE_DEBUG
#define THE_STACK_TRACE the::stack_trace::marker\
    __the_stack_trace_marker(THE_FUNCTION, __FILE__, __LINE__)

#else /* THE_DEBUG */
#define THE_STACK_TRACE

#endif /* THE_DEBUG */


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_STACK_TRACE_H_INCLUDED */
