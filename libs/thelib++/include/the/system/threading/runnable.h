/*
 * the/system/threading/runnable.h
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
 * Runnable.h  15.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_RUNNABLE_H_INCLUDED
#define THE_SYSTEM_THREADING_RUNNABLE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <atomic>

#include "the/system/threading/thread.h"


namespace the {
namespace system {
namespace threading {


    /**
     * Defines the interface for objects that can be run in threads.
     */
    class runnable {

    public:

        /** Dtor. */
        virtual ~runnable(void);

        /**
         * The thread will call this method immediately after it was started
         * in the context of the new thread and before run() is called.
         *
         * The default implementation does nothing.
         *
         * Note for implementors: This method must not throw exceptions.
         *
         * @param id The ID of the thread that is running the runnable.
         */
        virtual void on_thread_started(const thread::id& id) throw();

        /**
         * The thread will call this method immediately before it will be 
         * started. This is done in the context of the old thread. The
         * 'userData' parameter is the same as passed to on_thread_started().
         *
         * The default implementation does nothing.
         *
         * Note for implementors: This method must not throw exceptions.
         *
         * @param userData The user data that has been passed to the start()
         *                 method of the thread.
         */
        virtual void on_thread_starting(void *userData) throw();

        /**
         * This method will be called once the thread exited (after run() 
         * returned or after the thread was terminated).
         *
         * The default implementation does nothing.
         *
         * Note for implementors: This method must not throw exceptions.
         *
         * Note for implementors: It is undefined in which thread context this 
         * method will be called. Subclasses should not try to work with locks 
         * and make any assumption about the lock being (recursively) held in 
         * this method. On Windows, this method could be used to recover
         * abandoned mutexes after a forceful termination.
         *
         * Note for implementors: This method could be called within the thread
         * that executed the runnable. You should not assume that this OS thread
         * actually exited if this callback gets called!
         *
         * @param reason The reason why the thread actually exited. The runnable
         *               can use this information to determine what to do now.
         */
        virtual void on_thread_terminated(
            const thread::exit_reason reason) throw();

        /**
         * The thread will call this method if it was requested to terminate by
         * the user.
         *
         * By returning certain values, the runnable can signal how this 
         * termination request can be handled: The default implementation 
         * returns termination_behaviour::unsupported in order to indicate that
         * this runnable cannot be terminated. The thread will fail the 
         * termination request if this value is returned. Other possible values 
         * are termination_behaviour::graceful and 
         * termination_behaviour::forceful. The former indicated that the 
         * runnable will handle the termination request and return from its
         * run() method as soon as possible. The latter indicates that the 
         * runnable cannot interrupt its work, but that it is safe to forcefully
         * terminate the thread running it (by means of Terminate() on Windows
         * and pthread_cancel() on Linux). You should only return this value
         * if you are really sure that terminating the thread externally is
         * safe. Most importantly, it is NOT safe terminating threads that might
         * hold locks. Other resources that are not thread-bound (e.g. 
         * dynamically allocated memory) can and must be cleaned up in the
         * on_thread_terminated() method. You should, however, be aware of the
         * fact that on_thread_terminated() will run in a different thread 
         * context than run() and on_thread_terminating().
         *
         * Note for implementors: This method must not throw exceptions.
         *
         * Note for implementors: This method must never block and should return
         * as soon as possible!
         *
         * @return termination_behaviour::unsupported to indicate that the 
         *         thread cannot be terminated (the default returned by this
         *         implementation), termination_behaviour::graceful in order to
         *         indicate that the runnable handles the termination and will
         *         make run() return as soon as possible, 
         *         termination_behaviour::forceful to indicate that it is safe
         *         to terminate the thread forcefully.
         */
        virtual thread::termination_behaviour on_thread_terminating(
            void) throw();

        /**
         * Perform the work of a thread.
         *
         * @return The application dependent return code of the thread. This 
         *         must not be STILL_ACTIVE (259).
         */
        virtual int run(void) = 0;

    protected:

        /** 
         * Initialises a new instance.
         */
        runnable(void);

    private:

//#ifdef THE_LINUX
//        /** Stores the thread context (exit code etc.). */
//        the::intrusive_ptr<context> context;
//#endif /* THE_LINUX */
    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_RUNNABLE_H_INCLUDED */
