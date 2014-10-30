/*
 * include\the\system\threading\thread_pool.h
 *
 * Copyright (C) 2013 TheLib Team (http://www.thelib.org/license)
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

#ifndef THE_SYSTEM_THREADING_THREAD_POOL_H_INCLUDED
#define THE_SYSTEM_THREADING_THREAD_POOL_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if 0 && (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0600))
#define WITH_THE_WINDOWS_VISTA_THREAD_POOL 1
#endif /* (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0600)) */

#include <atomic>

#ifdef THE_WINDOWS
#include <Windows.h>
#endif /* THE_WINDOWS */

#include "the/delegate.h"
#include "the/intrusive_ptr.h"
#include "the/not_copyable.h"
#include "the/reference_counted.h"

#include "the/collections/fast_forward_list.h"

#include "the/system/threading/critical_section.h"
#include "the/system/threading/event.h"
#include "the/system/threading/runnable.h"
#include "the/system/threading/runnable_thread.h"
#include "the/system/threading/semaphore.h"
#include "the/system/threading/thread.h"


namespace the {
namespace system {
namespace threading {

    /**
     * Implements a thread pool.
     *
     * On Windows Vista or later (_WIN32_WINNT >= 0x0600), the thread pool
     * implementation of the operating system is used. In all other cases, the
     * class provides an own implementation based on the thread class of
     * THElib.
     */
    class thread_pool : public the::not_copyable {

    public:

        /**
         * Represents a single piece of work that is to be run in a thread of
         * the thread pool.
         *
         * This class serves as a facade for different types of work items. The
         * thread pool e.g. allows running the::system::threading::runnable
         * instances. Work items are created via the factory methods of the
         * thread pool.
         */
        class work_item : public the::reference_counted<work_item>,
                public the::not_copyable {

        public:

            /** Dtor. */
            virtual ~work_item(void);

            /**
             * Wait until this work item has been processed.
             */
            virtual void wait(void);

        protected:

            /**
             * Initialises a new instance.
             */
            work_item(void);

            /**
             * Acutally executes the work item's work.
             *
             * Subclasses must overwrite this method in order to perform the
             * used-defined work.
             *
             * @return The exit code of the work item.
             */
            virtual int run(void) = 0;

        private:

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
            /**
             * The work callback for thread pools in Windows Vista or later.
             *
             * @param instance A TP_CALLBACK_INSTANCE structure that defines
             *                 the callback instance. Applications do not modify
             *                 the members of this structure.
             * @param context  The application-defined data. This must be a
             *                 pointer to the work item.
             * @param work     A TP_WORK structure that defines the work object
             *                 that generated the callback.
             */
            static VOID CALLBACK work_func(PTP_CALLBACK_INSTANCE instance,
                PVOID context, PTP_WORK work);
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

#ifndef WITH_THE_WINDOWS_VISTA_THREAD_POOL
            /** 
             * This event is signaled by the worker thread once it has been
             * processed. Other threads can wait for completion like we can do
             * it on Windows Vista or above using the PTP_WORK object.
             */
            event evtDone;
#endif /* !WITH_THE_WINDOWS_VISTA_THREAD_POOL */

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
            /** The system object for the work item. */
            PTP_WORK work;
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

            /**
             * Provide access to the resources of the work item to the thread
             * pool.
             */
            friend class the::system::threading::thread_pool;
        }; /* end class work_item */

        /** The pointer type that is externally used for all work items. */
        typedef the::intrusive_ptr<work_item> work_item_type;

        /**
         * Initialises a new instance.
         */
        thread_pool(void);

        /** Dtor. */
        virtual ~thread_pool(void);

        /**
         * Create a new work item that executes a the::delegate.
         *
         * @param delegate The delegate that is performing the actual work. The
         *                 caller must ensure that the delegate is not nullptr.
         *
         * @return The work item that can be submitted to the thread pool.
         *
         * @throws the::argument_null_exception If 'delegate' is nullptr.
         * @throws the::system::system_exception In case the creation of the
         *                                       work item failed.
         */
        work_item_type create_work_item(the::delegate<int, void> delegate);

        /**
         * Create a new work item that runs a
         * the::system::threading::runnable in a thread of the thread pool.
         *
         * @param runnable The runnable that is performing the actual work.
         *                 The caller must ensure that the runnable lives until
         *                 the work item has been processed or canceled.
         * @param userData User-defined data that are passed to the runnable.
         *                 As for the::system::threading::thread, these data
         *                 are passed to the runnable in the context of the
         *                 calling thread.
         *
         * @return The work item that can be submitted to the thread pool.
         *
         * @throws the::argument_null_exception If 'runnable' is nullptr.
         * @throws the::system::system_exception In case the creation of the
         *                                       work item failed.
         */
        work_item_type create_work_item(runnable *runnable,
            void *userData = nullptr);

        /**
         * Set the maximum number of threads that the pool is allowed to spawn.
         *
         * @param maxThreads The new maximum number of threads that the pool is
         *                   allowed to spawn.
         *
         * @throws the::system::system_exception In case of an error.
         */
        void set_max_threads(const int maxThreads);

        /**
         * Sets the minimum number of threads that the pool is allowed to spawn.
         *
         * @param minThreads The new minimal number olf threads that the pool
         *                   must provide.
         *
         * @throws the::system::system_exception In case of an error.
         */
        void set_min_threads(const int minThreads);

        /**
         * Submit a work item for execution in a thread of the pool.
         *
         * Access to submit() and wait() must be synchronised, because otherwise
         * a race condition between items being canceled and new items being
         * submitted might occur.
         *
         * @param workItem The work item to be queued.
         *
         * @throws the::argument_null_exception If 'workItem' is nullptr.
         * @throws the::invalid_operation_exception If the pool is currently
         *                                          being torn down.
         */
        void submit(work_item_type workItem);

        /**
         * Waits for all currently active work items to complete.
         *
         * Access to submit() and wait() must be synchronised, because otherwise
         * a race condition between items being canceled and new items being
         * submitted might occur.
         */
        void wait(/*const bool cancelPending*/);

    private:

        /**
         * A specialisation of the::system::thread::work_item for executing a
         * parameterless the::delegate in a thread pool.
         */
        class delegate_work_item : public work_item {

        public:

            /** The type of the delegate that can be executed. */
            typedef the::delegate<int, void> delegate_type;

            /**
             * Initialise a new instance.
             *
             * @param delegate The delegate defining the work. This must not be
             *                 a NULL-delegate.
             */
            delegate_work_item(delegate_type delegate);

            /** Dtor. */
            virtual ~delegate_work_item(void);

            /**
             * Acutally executes the work item's work.
             *
             * @return The exit code of the work item.
             */
            virtual int run(void);

        private:

            /** Super class typedef. */
            typedef the::system::threading::thread_pool::work_item base;

            /** The delegate that is defining the work. */
            delegate_type delegate;
        }; /* end class delegate_work_item */

        /**
         * A specialisation of the::system::threading::work_item for executing a
         * the::system::threading::runnable in a thread pool.
         */
        class runnable_work_item : public work_item {

        public:

            /**
             * Initialise a new instance.
             *
             * @param runnable The runnable defining the work. This must not be
             *                 a nullptr.
             */
            runnable_work_item(the::system::threading::runnable *runnable);

            /** Dtor. */
            virtual ~runnable_work_item(void);

            /**
             * Acutally executes the work item's work.
             *
             * This method performs the same sequence of operations as 
             * the::system::threading::thread::executor_func, i.e. it ensures
             * that is makes no diffence whether a runnable is executed in a
             * thread or a pool thread.
             *
             * @return The exit code of the work item.
             */
            virtual int run(void);

        private:

            /** Super class typedef. */
            typedef the::system::threading::thread_pool::work_item base;

            /** The runnable that is defining the work. */
            the::system::threading::runnable *runnable;
        }; /* end class runnable_work_item */

#ifndef WITH_THE_WINDOWS_VISTA_THREAD_POOL
        /**
         * This is the worker that executes work_items that have been queued in
         * the thread pool. The runnable runs until the 'semBlockWorker' member
         * of 'pool' is in signaled state, but the work item queue is empty at
         * the same time.
         */
        class worker : public the::system::threading::runnable {

        public:

            /** The thread type that is running the worker. */
            typedef the::system::threading::runnable_thread<worker> thread_type;

            /** Initialise a new instance. */
            worker(void);

            /** Dtor. */
            virtual ~worker(void);

            /**
             * Initialises the 'pool' attribute on startup.
             *
             * @param userData A pointer to the pool the thread is working for.
             */
            virtual void on_thread_starting(void *userData) throw();

            /**
             * This callback detaches the thread and deletes itself.
             *
             * We are using runnable_threads as workers which are allocated on
             * the heap using new. Once the thread exited, we detach it to
             * invalidate the handle, which in turn gives us the opportunity to
             * delete the object itself without violating the invariant that
             * no running thread object must be destroyed.
             *
             * @param reason The exit reason, which should always be
             *               thread::exit_reason::exited.
             */
            virtual void on_thread_terminated(
                const thread::exit_reason reason) throw();

            /**
             * Performs work from the thread pool's queue until the queue is
             * empty and the thread is woken at the same time.
             *
             * @return TODO
             */
            virtual int run(void);

        private:

            /** Super class typedef. */
            typedef the::system::threading::runnable base;

            /** The thread pool the thread is working for. */
            thread_pool *pool;
        }; /* end class worker */
#endif /* !WITH_THE_WINDOWS_VISTA_THREAD_POOL */

        /** Super class typedef. */
        typedef the::not_copyable base;

#ifndef WITH_THE_WINDOWS_VISTA_THREAD_POOL
        /** The type of the queue used to store the work items. */
        typedef the::collections::fast_forward_list<work_item_type,
            the::system::threading::critical_section> queue_type;
#endif /* !WITH_THE_WINDOWS_VISTA_THREAD_POOL */

//#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
//        /**
//         * The callback that is executed if CloseThreadpoolCleanupGroup() is
//         * called.
//         *
//         * @param objectContext  Optional application-defined data specified
//         *                       during creation of the object.
//         * @param cleanupContext Optional application-defined data specified
//         *                       using CloseThreadpoolCleanupGroupMembers().
//         */
//        static VOID CALLBACK cleanup_func(PVOID objectContext,
//            PVOID cleanupContext);
//#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

        /**
         * Ensures that all resources of the thread pool are being released. The
         * method returns once all work items have been processed.
         *
         * On Windows Vista or above, this reverts all effects of 'assert_init'.
         * On all other platforms, this method waits until all worker threads
         * have been exited.
         */
        void assert_cleanup(void);

        /**
         * Lazily initialises the thread pool.
         *
         * On Windows Vista or above, the pool and the clean up group are lazily
         * initialised. All methods that use the 'pool' member must call this 
         * method before accessing it.
         *
         * Rationale: We cannot create the pool in the constructor because this
         * operation might fail and thus raise an exception inthe constructor,
         * which should be avoided.
         *
         * On all other platforms, it is ensured that we have at least one
         * thread.
         *
         * @throws the::system::system_exception If 'pool' was nullptr, but could
         *                                       not be initialised.
         */
        void assert_init(void);

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
        /** The cleanup group that is used to track the callbacks. */
        PTP_CLEANUP_GROUP cleanupGroup;

        /**
         * The callback environment used to configure the behaviour of 'pool'.
         */
        TP_CALLBACK_ENVIRON env;

        /** The thread pool. */
        PTP_POOL pool;

#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

        /** The number of threads currently working on some work item. */
        std::atomic<int> cntActiveThreads;

        /** The total number of threads (active and idling). */
        std::atomic<int> cntTotalThreads;

        /**
         * This event is signaled if the queue is empty. In case there are items
         * being processed, it can be used to wait for those items being
         * completed.
         */
        event evtQueueEmpty;

        /** 
         * This flag determines whether it is possible to add new work items
         * to the queue.
         *
         * If the thread pool is being shut down, the queue is closed via this
         * flag in order to prevent a race condition between the shutdown
         * process and another thread qeueing new items.
         *
         * When accessing this attribute, 'queue' must be locked.
         */
        bool isQueueOpen;

        /** The maximum number of threads that the pool is allowed to spawn. */
        std::atomic<int> maxThreads;

        /** The queue of work items that are not yet processed. */
        queue_type queue;

        /** 
         * Semaphore for waiting while 'queue' is empty. The semaphore counter
         * is equivalent to the number of queued work items in case of normal
         * operation. In order to make the worker threads exit, the counter
         * is incremented to the number of threads such that each thread is
         * woken even if the queue is empty.
         */
        semaphore semQueueEmpty;
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_THREAD_POOL_H_INCLUDED */
