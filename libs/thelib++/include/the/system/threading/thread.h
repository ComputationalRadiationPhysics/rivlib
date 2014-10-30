/*
 * the/system/threading/thread.h
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
 * Thread.h  15.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */
#ifndef THE_SYSTEM_THREADING_THREAD_H_INCLUDED
#define THE_SYSTEM_THREADING_THREAD_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <cstdint>
#include <climits>
#include <ostream>
#include <string>

#ifdef THE_LINUX
#include <pthread.h>
#endif /* THE_LINUX */

#ifdef THE_WINDOWS
#include <windows.h>
#endif /* THE_WINDOWS */

#include "the/comparable.h"
#include "the/intrusive_ptr.h"
#include "the/not_copyable.h"
#include "the/not_implemented_exception.h"
#include "the/reference_counted.h"
#include "the/stack_trace.h"
#include "the/string.h"
#include "the/types.h"


namespace the {
namespace system {
namespace threading {

    /* Forward declarations. */
    class runnable;


    /**
     * Implements a platform-independent interface for system threads.
     *
     * On Linux, Threads are created joinable. Join() may only be called once. 
     * See remarks on this method.
     *
     * On Linux, the thread is detached in the destructor. Any native API calls,
     * in particular joining the thread, using the thread ID are illegal after 
     * the thread object has been destroyed.
     *
     * The thread class itself is not thread-safe. It it therefore erroneous to 
     * make concurrent calls to any of the thread objects' methods except for 
     * static ones.
     *
     * @author Christoph Mueller
     */
    class thread : public not_copyable {

    public:

        /** 
         * An opaque wrapper for a thread ID.
         */
        class id : comparable<id> {

        public:

            /** The ID type of the underlying thread implementation. */
#if defined(THE_WINDOWS)
            typedef DWORD native_id_type;
#elif defined(THE_LINUX)
            typedef pthread_t native_id_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::threading::thread::id must define native_id_type!"
#endif /* defined(THE_WINDOWS) */

            /** An empty ID instance. */
            static id empty;

            /**
             * Ctor.
             */
            id(void);

            /**
             * Ctor.
             *
             * @param nativeId The native thread ID represented by this object.
             */
            explicit id(const native_id_type& nativeId);

            /**
             * Clones 'id'.
             *
             * @param id The object to be cloned.
             */
            id(const id& id);

            /**
             * Dtor.
             */
            ~id(void);

            /**
             * Compare this thread ID and 'id'
             *
             * @param rhs The object to be compared.
             *
             * @return A negative number if this object is less than 'rhs', 
             *         zero if this object is equal to 'rhs',
             *         a positive number if this object is larger thatn 'rhs'.
             */
            virtual int compare(const id& rhs) const;

            /**
             * Convert the ID to an ANSI string.
             *
             * @return The string representation of the string.
             */
            astring to_astring(void) const;

            /**
             * Convert the ID to a unicode string.
             *
             * @return The string representation of the string.
             */
            wstring to_wstring(void) const;

            /**
             * Assignment.
             *
             * @param rhs The right hand side operand.
             *
             * @return *this.
             */
            id& operator =(const id& rhs);

            /**
             * Writes the ID into a stream.
             *
             * @param lhs The stream to write to.
             * @param rhs The ID that is to be written.
             *
             * @return 'lhs'
             */
            template<class C, class T>
            friend inline std::basic_ostream<C, T>& operator <<(
                    std::basic_ostream<C, T>& lhs, const id rhs) {
                lhs << rhs.nativeId;
                return lhs;
            }

        private:

            /** The native ID wrapped by this object. */
            native_id_type nativeId;
        }; /* end class id */

        /**
         * These values are used by the thread to inform the runnable about
         * the reason for it exiting.
         */
        typedef enum exit_reason_t {
            unknown = 0,        //< Reason is not known.
            exited,             //< Runnable exited normally.
            crashed,            //< Uncaught exception occurred.
            terminated,         //< Runnable was forcefully terminated.
            exit_reason_force_32_bits = 0x7FFFFFFF
        } exit_reason;

        /**
         * This enumeration defines possible ways for a runnable to react on
         * a termination request.
         */
        typedef enum termination_behaviour_t {
            unsupported = 0x0,  //< Termination is not supported.
            graceful = 0x01,    //< Runnable will exit as soon as possible.
            forceful = 0x02,    //< Runnable must be aborted.
            termination_behaviour_force_32_bits = 0x7FFFFFFF
        } termination_behaviour;

        /** The type of the native thread handle. */
#if defined(THE_WINDOWS)
        typedef DWORD native_exit_code_type;
#elif defined(THE_LINUX)
        typedef int native_exit_code_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::threading::thread must define native_exit_code_type!"
#endif /* defined(THE_WINDOWS) */

        /** The type of the native thread handle. */
#if defined(THE_WINDOWS)
        typedef HANDLE native_handle_type;
#elif defined(THE_LINUX)
        typedef pthread_t native_handle_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::threading::thread must define native_handle_type!"
#endif /* defined(THE_WINDOWS) */

        /**
         * Answer the ID of the calling thread.
         *
         * @eturn The ID of the calling thread.
         */
        static inline thread::id get_current_id(void) {
#if defined(THE_WINDOWS)
            return id(::GetCurrentThreadId());
#elif defined(THE_LINUX)
            return id(::pthread_self());
#else /* defined(THE_WINDOWS) */
            THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
        }

        // TODO: How to get exit code under Linux when using pthread_exit??
//        /**
//         * Exits the calling thread with the specified exit code.
//         *
//         * @param exitCode Exit code for the calling thread.
//         */
//        static inline void Exit(const DWORD exitCode) {
//#ifdef _WIN32
//            ::ExitThread(exitCode);
//#else /* _WIN32 */
//            ::pthread_exit(reinterpret_cast<void *>(exitCode));
//#endif /* _WIN32 */
//        }

        /**
         * Makes the calling thread sleep for 'millis' milliseconds.
         *
         * @param millis The milliseconds to block the calling thread.
         */
        static void sleep(const unsigned long millis);

        /**
         * Causes the calling thread to yield execution to another thread that 
         * is ready to run on the current processor.
         *
         * @throws system_exception If the operation could not be completed 
         *                          successfully (Linux only).
         */
        static void yield(void);

        /** 
         * Create a thread that executes the given runnable.
         *
         * Note that the runnable must not be nullptr and must not be executed
         * by another thread at the same time.
         *
         * @param runnable The runnable to run in a thread. The caller must
         *                 guarantee that the object designated by 'runnable'
         *                 exists at least as long as the thread is running.
         *                 The runnable must not be nullptr.
         */
        explicit thread(runnable *runnable);

        // TODO: Think about this.
        //thread(thread&& rhs);

        /**
         * Dtor. 
         *
         * Please note that this operation will abort the programme in case
         * the thread object controls a running thread, i.e. the object is
         * destroyed before the thread has exited or has been detached.
         */
        virtual ~thread(void);

        /**
         * Detaches this object from the actual thread, i.e. prevents further
         * control of the thread by means of this object.
         *
         * Note: It is safe to call this methods on threads that are not running
         * or have already been detached. In this case, the method does nothing.
         *
         * @throws the::system::system_exception In case the operation fails.
         */
        virtual void detach(void);

        /**
         * Answer the exit code of the thread. 
         *
         * @return The thread exit code.
         * 
         * @throws system_exception If the exit code could not be determined,
         *                          e.g. because the thread was not started.
         */
        native_exit_code_type get_exit_code(void) const;

        /**
         * Answer the ID of the thread.
         *
         * @return the ID of the thread.
         */
        id get_id(void) const;

        ///**
        // * Answer the native thread handle.
        // *
        // * @return The native handle of the thread.
        // */
        //native_handle_type get_native_handle(void) const {
        //    THE_STACK_TRACE;
        //    return this->handle;
        //}

        /**
         * Answer a pointer to the runnable executed by this thread.
         *
         * @return A pointer to the runnable. The return value might be nullptr,
         *         e.g. if the thread has been detached.
         */
        inline const runnable *get_runnable(void) const {
            THE_STACK_TRACE;
            return this->theRunnable;
        }

        /**
         * Answer whether the thread is currently running.
         *
         * @return true, if the thread is currently running, false otherwise.
         */
        virtual bool is_running(void) const;

        /**
         * Waits for the thread to finish. If the thread was not started, the
         * method returns immediately, i.e. it is safe to call this method on
         * threads that are not running. In this case, the method does nothing.
         *
         * On Linux, join() can only be called by one other thread.
         *
         * On Linux, join() detaches the thread. Any further call to join or
         * a forceful termination is illegal.
         *
         * @throws system_exception If waiting for the thread failed.
         */
        virtual void join(void);

        /**
         * Start the thread.
         *
         * A thread can only be started, if it is in the state NEW or
         * FINISHED.
         *
         * @param userData The user data that are passed to the new thread's
         *                 thread function or Run() method.
         *
         * @throws SystemException If the creation of the new thread failed.
         */
        virtual void start(void *userData = NULL);

        /**
         * Tries to terminate a thread.
         *
         * The behaviour of this method depends on the runnable that is executed
         * by the thread: the thread asks the runnable via its 
         * on_thread_terminating() method whether it supports termination or
         * not. The runnable has three possible ways to react: 
         * - It can return that it does not support termination. In this case, 
         *   this method will raise an exception.
         * - It can return that it will exit as soon as possible. In this case,
         *   the method does nothing and lets the runnable exit. Optionally, the
         *   user can wait for the thread to actually exit.
         * - It can return that it can be forcefully terminated. In this case,
         *   this method terminates/cancels the thread executing the runnable.
         *
         * @param doWait   If set true, the method will wait for the thread to
         *                 actually become signaled, i.e. exit.
         * @param reserved This parameter is reserved for future use and must
         *                 be zero.
         * @param exitCode This parameter is only relevant in case of forceful
         *                 termination of the thread. As the runnable cannot
         *                 set an exit code in this case, the value passed here
         *                 will be used. The default value is -1.
         *
         * @throws invalid_operation_exception If the runnable does not support
         *                                     being terminated.
         * @throws system_exception If a forceful termination was tried, but
         *                          failed.
         */
        virtual void terminate(const bool doWait = false,
            const unsigned int reserved = 0,
            const native_exit_code_type exitCode = -1);

    private:

#ifdef THE_LINUX
        /**
         * Objects of this class are used for each thread under Linux to store
         * contextual information that must be updated from the thread and from
         * the runnable. The reason for this object is that the data store for
         * the exit code also must exist if the thread has been detached,
         * because the executor function cannot know this.
         */
        class status_block : public the::reference_counted<status_block> {

        public:

            /**
             * Initialises a new instance.
             *
             * @param owner Pointer to the thread allocating the status block.
             */
            status_block(thread *owner);

            /**
             * Dtor.
             */
            virtual ~status_block(void);

            /**
             * Answer the exit code.
             *
             * @return The exit code.
             */
            inline native_exit_code_type exit_code(void) const {
                THE_STACK_TRACE;
                return static_cast<native_exit_code_type>(this->exitCode);
            }

            /**
             * Answer the exit reason.
             *
             * @return The exit reason.
             */
            inline thread::exit_reason exit_reason(void) const {
                THE_STACK_TRACE;
                return static_cast<thread::exit_reason>(
                    static_cast<int>(this->exitReason));
            }

            /**
             * This method updates the exit code and the exit reason in case the
             * thread is being cancelled.
             *
             * The exit reason will be set to exit_reason::unknown to indicate
             * that the thread was forcefully terminated.
             *
             * @param exitCode The exit code to be set.
             */
            void on_cancelling(const int exitCode) throw();

            /**
             * This method calls the termination callback as it should be done
             * in the pthread cleanup handler.
             */
            void on_cleanup(void) throw();

            /**
             * This method updates the exit code and the exit reason in case of
             * a normal thread exit.
             *
             * @param exitCode   The exit code of the runnable.
             * @param exitReason The exit reason (might be normal exit or
             *                   graceful termination).
             */
            void on_exited(const int exitCode,
                const thread::exit_reason exitReason) throw();

            /**
             * Answer the runnable of the thread.
             *
             * @return The runnable of the thread.
             */
            inline threading::runnable *runnable(void) {
                THE_STACK_TRACE;
                return this->theRunnable;
            }

        private:

            /** Stores the exit code of the runnable. */
            std::atomic<int> exitCode;

            /** Remembers the reason why the runnable exited. */
            std::atomic<int> exitReason;

            /** 
             * The runnable that is executed by the thread that the status block
             * stores contextual information for.
             */
            threading::runnable *theRunnable;

        }; /* end class status_block */
#endif /* THE_LINUX */

        /** 
         * The actual type of the parameter passed to thread::executor_func.
         * The void pointer will be casted to this type, and we have this 
         * typedef because the parameter passed on Windows and Linux differs.
         */
#if defined(THE_LINUX)
        typedef status_block *executor_param_type;
#else /* defined(THE_LINUX) */
        typedef runnable *executor_param_type;
#endif /* defined(THE_LINUX) */

#ifdef THE_LINUX
        /**
         * A cleanup handler for POSIX threads. We use this to determine whether
         * a thread is still running and for storing the exit code.
         *
         * The cancellation cleanup handler is popped from the cancellation 
         * cleanup stack and invoked with the argument arg when: (a) the thread 
         * exits (that is, calls pthread_exit()), (b) the thread acts upon a 
         * cancellation request, or (c) the thread calls pthread_cleanup_pop() 
         * with a non-zero execute argument.
         *
         * @param param A pointer to the thread status_block object.
         */
        static void cleanup_func(void *param);
#endif /* THE_LINUX */

        /**
         * The thread function that is passed to the system API when starting a
         * new thread.
         *
         * @param param The parameters for the thread which must be castable to
         *              thread::executor_param_type.
         * 
         * @return The thread's exit code.
         */
#if defined(THE_WINDOWS)
        static DWORD WINAPI executor_func(void *param);
#else /* defined(THE_WINDOWS) */
        static void *executor_func(void *param);
#endif /* defined(THE_WINDOWS)*/

#ifdef THE_LINUX
        /**
         * Stores the contextual information about the thread that must exists 
         * under Linux even if the thread is detached.
         */
        the::intrusive_ptr<status_block> statusBlock;
#endif /* THE_LINUX */

        /** The native handle used to control the thread. */
        native_handle_type handle;   // TODO: possible race condition!

        /** 
         * The Runnable to execute. 
         *
         * This member is considered immutable after construction of the thread 
         * object. Access is read-only while the thread is running.
         */
        runnable *theRunnable;
    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_THREAD_H_INCLUDED */
