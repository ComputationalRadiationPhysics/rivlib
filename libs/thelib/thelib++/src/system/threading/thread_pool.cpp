/*
 * src\system\threading\thread_pool.cpp
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

#include "the/system/threading/thread_pool.h"

#include <climits>

#include "the/argument_exception.h"
#include "the/argument_null_exception.h"
#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/invalid_operation_exception.h"

#include "the/system/system_exception.h"
#include "the/system/system_information.h"

#include "the/system/threading/auto_lock.h"
#include "the/system/threading/debug.h"


#include "the/not_implemented_exception.h"

#ifndef _WIN32
/**
 * Return code that marks a thread as still running. Make sure that the value 
 * is the same as on Windows.
 */
#define STILL_ACTIVE (259)
#endif /* !_WIN32 */


////////////////////////////////////////////////////////////////////////////////
#pragma region work_item

/*
 * the::system::threading::thread_pool::work_item::~work_item
 */
the::system::threading::thread_pool::work_item::~work_item(void) {
    THE_STACK_TRACE;
    // As we use CloseThreadpoolCleanupGroupMembers(), the work item must not
    // be released here. MSDN states: "Also, an application should not release
    // any of the objects individually by calling a function such as "
    // CloseThreadpoolWork, because the objects have already been released."
    // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682036(v=vs.85).aspx
    THE_THREADING_TRACE_DEBUG("Released thread pool work item 0x%p.\n", this);
}


/*
 * the::system::threading::thread_pool::work_item::wait
 */
void the::system::threading::thread_pool::work_item::wait(void) {
    THE_STACK_TRACE;
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    if (this->work != nullptr) {
        THE_THREADING_TRACE_INFO("The thread [%hs] is waiting for the work "
            "item 0x%p being completed in a thread pool...\n",
            thread::get_current_id().to_astring().c_str(), this);
        ::WaitForThreadpoolWorkCallbacks(this->work, FALSE);
    }
#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    this->evtDone.wait();
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


/*
 * the::system::threading::thread_pool::work_item::work_item
 */
the::system::threading::thread_pool::work_item::work_item(void)
        : the::reference_counted<work_item>(), the::not_copyable()
#ifndef WITH_THE_WINDOWS_VISTA_THREAD_POOL
        , evtDone(true)
#endif /* !WITH_THE_WINDOWS_VISTA_THREAD_POOL */
        {
    THE_STACK_TRACE;
    THE_THREADING_TRACE_DEBUG("Creating thread pool work item 0x%p.\n", this);

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    this->work = nullptr;
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
/*
 * the::system::threading::thread_pool::work_item::work_func
 */
VOID CALLBACK the::system::threading::thread_pool::work_item::work_func(
        PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work) {
    THE_STACK_TRACE;
    THE_ASSERT(context != nullptr);
    work_item *wi = static_cast<work_item *>(context);
    wi->run();
    wi->release();  // Release the reference of the pool.
}
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * the::system::threading::thread_pool::thread_pool
 */
the::system::threading::thread_pool::thread_pool(void) : base(),
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
        cleanupGroup(nullptr), pool(nullptr) {
#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
        cntActiveThreads(0), cntTotalThreads(0), evtQueueEmpty(true),
        isQueueOpen(true), maxThreads(INT_MAX), semQueueEmpty(0l, LONG_MAX) {
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    THE_STACK_TRACE;
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    ::InitializeThreadpoolEnvironment(&this->env);
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


/*
 * the::system::threading::thread_pool::~thread_pool
 */
the::system::threading::thread_pool::~thread_pool(void) {
    THE_STACK_TRACE;
    THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is in destructor.\n", this);
    this->assert_cleanup();
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    ::DestroyThreadpoolEnvironment(&this->env);
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}

/*
 * the::system::threading::thread_pool::create_work_item
 */
the::system::threading::thread_pool::work_item_type
the::system::threading::thread_pool::create_work_item(
        the::delegate<int, void> delegate) {
    THE_STACK_TRACE;

    if (!(delegate.is_target_set())) {
        THROW_THE_EXCEPTION(argument_null_exception, "delegate");
    }

    work_item_type retval(new delegate_work_item(delegate), false);

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    // Create the work item in the thread pool of the OS.
    retval->work = ::CreateThreadpoolWork(work_item::work_func,
        retval.operator-> (), &this->env);
    if (retval->work == nullptr) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
    THE_THREADING_TRACE_DEBUG("Native thread pool work item 0x%p has been "
        "created and wrapped in 0x%p.\n", retval->work, retval.operator ->());

#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    retval->evtDone.reset();
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

    return retval;
}


/*
 * the::system::threading::thread_pool::create_work_item
 */
the::system::threading::thread_pool::work_item_type
the::system::threading::thread_pool::create_work_item(runnable *runnable,
        void *userData) {
    THE_STACK_TRACE;

    if (runnable == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "runnable");
    }

    work_item_type retval(new runnable_work_item(runnable), false);

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    // Create the work item in the thread pool of the OS.
    retval->work = ::CreateThreadpoolWork(work_item::work_func,
        retval.operator-> (), &this->env);
    if (retval->work == nullptr) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
    THE_THREADING_TRACE_DEBUG("Native thread pool work item 0x%p has been "
        "created and wrapped in 0x%p.\n", retval->work, retval.operator ->());

#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    retval->evtDone.reset();
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

    // Call the initialisation hook, just like the "normal" thread.
    runnable->on_thread_starting(userData);

    return retval;
}

/*
 * the::system::threading::thread_pool::set_max_threads
 */
void the::system::threading::thread_pool::set_max_threads(
        const int maxThreads) {
    THE_STACK_TRACE;
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    this->assert_init();
    ::SetThreadpoolThreadMaximum(this->pool, maxThreads);
#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    this->maxThreads = maxThreads;
    // TODO: Must stop threads in this case.
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


/*
 * the::system::threading::thread_pool::set_min_threads
 */
void the::system::threading::thread_pool::set_min_threads(
        const int minThreads) {
    THE_STACK_TRACE;

    if (minThreads < 0) {
        THROW_THE_EXCEPTION(argument_exception, "minThreads");
    }

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    this->assert_init();
    if (!::SetThreadpoolThreadMinimum(this->pool, minThreads)) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    for (; this->cntTotalThreads < minThreads; ++this->cntTotalThreads) {
        (new worker::thread_type())->start(this);
    }
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


/*
 * the::system::threading::thread_pool::submit
 */
void the::system::threading::thread_pool::submit(
        work_item_type workItem) {
    THE_STACK_TRACE;

    if (workItem == nullptr) {
        THROW_THE_EXCEPTION(argument_null_exception, "workItem");
    }

    this->assert_init();

    THE_THREADING_TRACE_DEBUG("Enqueueing work item 0x%p...\n", 
        workItem.operator-> ());
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    // Add an additional reference for the pool, because we have passed a dumb
    // pointer to the 'CreateThreadpoolWork'. The exectutor function must
    // decrement the counter once the work item was processed or canceled.
    workItem->add_ref();

    // Actually submit the work item to the pool.
    ::SubmitThreadpoolWork(workItem->work);

#else  /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    auto_lock<queue_type> l(this->queue);
    if (this->isQueueOpen) {
        this->queue.add(workItem);      // Add item.
        this->evtQueueEmpty.reset();    // Signal unfinished work.
        this->semQueueEmpty.unlock();   // Wake at least one worker.
#ifndef _WIN32
        // Linux has the most crap scheduler I have ever seen ...
        thread::yield();
#endif /* _WIN32 */

        // TODO: Must spawn new threads (until max reached) if there is congestion
    } else {
        THROW_THE_EXCEPTION(invalid_operation_exception, "The work item "
            "cannot be queued as the thread pool is being closed.");
    }
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


/*
 * the::system::threading::thread_pool::wait
 */
void the::system::threading::thread_pool::wait(void) {
    THE_STACK_TRACE;
    THE_THREADING_TRACE_INFO("The thread [%hs] is waiting for thread pool "
        "0x%p to complete all of its work items...\n", 
        thread::get_current_id().to_astring().c_str(), this);

#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    if (this->cleanupGroup != nullptr) {
        THE_THREADING_TRACE_DEBUG("Closing cleanup group 0x%p...",
            this->cleanupGroup);
        ::CloseThreadpoolCleanupGroupMembers(this->cleanupGroup, FALSE, this);
    }

#else  /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    this->evtQueueEmpty.wait();
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */

    THE_THREADING_TRACE_INFO("The thread [%hs] is signaled as thread pool "
        "0x%p completed all of its work items...\n",
        thread::get_current_id().to_astring().c_str(), this);
}


////////////////////////////////////////////////////////////////////////////////
#pragma region delegate_work_item

/*
 * the::system::threading::thread_pool::delegate_work_item::delegate_work_item
 */
the::system::threading::thread_pool::delegate_work_item::delegate_work_item(
        delegate_work_item::delegate_type delegate)
        : base(), delegate(delegate) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread_pool::delegate_work_item::~delegate_work_item
 */
the::system::threading::thread_pool::delegate_work_item::~delegate_work_item(
        void) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread_pool::delegate_work_item::run
 */
int the::system::threading::thread_pool::delegate_work_item::run(void) {
    THE_STACK_TRACE;
    THE_THREADING_TRACE_INFO("The thread pool thread [%hs] is processing the "
        "work item 0x%p containing a delegate...\n",
        thread::get_current_id().to_astring().c_str(), this);
    int retval = this->delegate();
    return retval;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
#pragma region runnable_work_item

/*
 * the::system::threading::thread_pool::runnable_work_item::runnable_work_item
 */
the::system::threading::thread_pool::runnable_work_item::runnable_work_item(
        the::system::threading::runnable *runnable)
        : base(), runnable(runnable) {
    THE_STACK_TRACE;
    THE_ASSERT(this->runnable != nullptr);
}


/*
 * the::system::threading::thread_pool::runnable_work_item::~runnable_work_item
 */
the::system::threading::thread_pool::runnable_work_item::~runnable_work_item(
        void) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread_pool::runnable_work_item::run
 */
int the::system::threading::thread_pool::runnable_work_item::run(void) {
    THE_STACK_TRACE;

    thread::exit_reason exitReason = thread::exit_reason::unknown;
    int retval = 0;
    threading::runnable *r = this->runnable;
    THE_ASSERT(r != nullptr);

    THE_THREADING_TRACE_INFO("The thread pool thread [%hs] is processing the "
        "work item 0x%p with runnable 0x%p...\n",
        thread::get_current_id().to_astring().c_str(), this, this->runnable);

    /* Inform the runnable that the thread is now running. */
    r->on_thread_started(thread::get_current_id());

    try {
        retval = r->run();
        THE_ASSERT(retval != STILL_ACTIVE); // Should not use STILL_ACTIVE!
        exitReason = thread::exit_reason::exited;
    } catch (...) {
        THE_THREADING_TRACE_WARN("A runnable in the thread pool thread [%hs] "
            "crashed while processing work item 0x%p due to an uncaught "
            "exception at top level.\n", 
            thread::get_current_id().to_astring().c_str(), this);
        exitReason = thread::exit_reason::crashed;
    }

    /* Call the termination callback. */
    r->on_thread_terminated(exitReason);

    return retval;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
#pragma region worker
#ifndef WITH_THE_WINDOWS_VISTA_THREAD_POOL

/*
 * the::system::threading::thread_pool::worker::worker
 */
the::system::threading::thread_pool::worker::worker(void)
        : base(), pool(nullptr) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread_pool::worker::~worker
 */
the::system::threading::thread_pool::worker::~worker(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread_pool::worker::on_thread_starting
 */
void the::system::threading::thread_pool::worker::on_thread_starting(
        void *userData) throw() {
    THE_STACK_TRACE;
    THE_ASSERT(userData != nullptr);
    this->pool = static_cast<thread_pool *>(userData);
}


/*
 * the::system::threading::thread_pool::worker::on_thread_terminated
 */
void the::system::threading::thread_pool::worker::on_thread_terminated(
        const thread::exit_reason reason) throw() {
    THE_STACK_TRACE;
    THE_ASSERT(reason == thread::exit_reason::exited);      // Anything else is strange...
    thread_type *that = dynamic_cast<thread_type *>(this);  // runnable_thread.
    THE_THREADING_TRACE_DEBUG("Thread pool worker thread 0x%p is cleaned up in "
        "thread [%hs]...\n", that,
        thread::get_current_id().to_astring().c_str());
    that->detach();
    delete this;
}


/*
 * the::system::threading::thread_pool::worker::run
 */
int the::system::threading::thread_pool::worker::run(void) {
    THE_STACK_TRACE;

    event& evtQueueEmpty = this->pool->evtQueueEmpty;       // Shortcut.
    int retval = 0;                                         // Return value.
    thread_pool::queue_type& queue = this->pool->queue;     // Shortcut.
    semaphore& semQueueEmpty = this->pool->semQueueEmpty;   // Shortcut.
    work_item_type workItem = nullptr;                      // Active item.

    THE_THREADING_TRACE_DEBUG("Thread pool worker thread 0x%p [%hs] has been "
        "started.\n", this, thread::get_current_id().to_astring().c_str());

    do {
        /* Wait for work. */
        THE_THREADING_TRACE_DEBUG("Thread pool worker thread [%hs] is waiting "
            "for work...\n", thread::get_current_id().to_astring().c_str());
        semQueueEmpty.lock();

        /* Acquire locks. */
        queue.lock();

        /*
         * We use an empty queue as trigger for a thread to leave: If we wake a
         * thread and it does not find any work to do, it should exit.
         */
        THE_THREADING_TRACE_DEBUG("Thread pool worker thread [%hs] is checking "
            "the queue...\n", thread::get_current_id().to_astring().c_str());
        if (!queue.is_empty()) {
            workItem = queue.first();
            queue.remove_first();
            ++this->pool->cntActiveThreads;
            THE_THREADING_TRACE_DEBUG("Thread pool worker thread [%hs] "
                "dequeued work item 0x%p; queue now contains %u additional "
                "items; %d threads are currently active.\n",
                thread::get_current_id().to_astring().c_str(),
                workItem.operator-> (), queue.size(),
                static_cast<int>(this->pool->cntActiveThreads));
        } else {
            workItem = nullptr;
        }

        /* Release locks while working. */
        queue.unlock();

        /* Do the work. */
        if (workItem != nullptr) {
            THE_THREADING_TRACE_INFO("Thread pool worker thread [%hs] is "
                "processing work item 0x%p...\n",
                thread::get_current_id().to_astring().c_str(),
                workItem.operator-> ());
            workItem->run();
            THE_THREADING_TRACE_INFO("The thread pool thread [%hs] completed "
                "the work item 0x%p.\n", 
                thread::get_current_id().to_astring().c_str(),
                workItem.operator ->(), retval, retval);
            workItem->evtDone.set();
        }

        /* Check whether we must signal listeners. */
        if ((--this->pool->cntActiveThreads == 0)   // SFX. Must be first!
                && queue.is_empty()) {
            THE_THREADING_TRACE_DEBUG("Signaling that queue is empty and "
                "thread has work...\n");
            evtQueueEmpty.set();
        }

    } while (workItem != nullptr);

    THE_THREADING_TRACE_DEBUG("Thread pool worker thread [%hs] is exiting...\n",
        thread::get_current_id().to_astring().c_str());
    --this->pool->cntTotalThreads;

    // Note: Actual cleanup must be done in termination handler, because 
    // the whole stuff will explode otherwise.

    return 0;
}


#endif /* !WITH_THE_WINDOWS_VISTA_THREAD_POOL */
#pragma endregion
////////////////////////////////////////////////////////////////////////////////

//#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
///*
// * the::system::threading::thread_pool::cleanup_func
// */
//VOID CALLBACK the::system::threading::thread_pool::cleanup_func(
//        PVOID objectContext, PVOID cleanupContext) {
//    THE_STACK_TRACE;
//    thread_pool *that = static_cast<thread_pool *>(cleanupContext);
//}
//#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */


/*
 * the::system::threading::thread_pool::assert_cleanup
 */
void the::system::threading::thread_pool::assert_cleanup(void) {
    THE_STACK_TRACE;
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    if (this->cleanupGroup != nullptr) {
        THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is waiting for all "
            "outstanding work items as part of final cleanup...\n", this);
        // The cleanup group must have no members when we try to close it.
        // Therefore, we close all cleanup group members and wait for them to
        // finish. We cannot cancel the outstanding requests because this would
        // cause a memory leak (outstanding work_item object would not be
        // released properly).
        // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682033(v=vs.85).aspx
        ::CloseThreadpoolCleanupGroupMembers(this->cleanupGroup, FALSE, this);
        ::CloseThreadpoolCleanupGroup(this->cleanupGroup);
        this->cleanupGroup = nullptr;
    }

    if (this->pool != nullptr) {
        THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is being destroyed...\n",
            this);
        ::CloseThreadpool(this->pool);
        this->pool = nullptr;
    }

#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is about to be cleaned up...\n",
        this);
    this->queue.lock();
    this->isQueueOpen = false;  // Prevent further items being queued.
    this->queue.clear();        // Signal exit to workers via empty queue.
    this->queue.unlock();       // Allow workers to check the queue.

    this->wait();               // Wait for active items to complete.

    THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is waking %d worker threads "
        "threads as part of final cleanup...\n", this,
        static_cast<int>(this->cntTotalThreads));
    for (int i = 0; i < this->cntTotalThreads; ++i) {
        this->semQueueEmpty.unlock();
    }

    THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is waiting for %d worker "
        "threads to exit as part of final cleanup...\n", this,
        static_cast<int>(this->cntTotalThreads));
    this->wait();
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}


/*
 * the::system::threading::thread_pool::assert_init
 */
void the::system::threading::thread_pool::assert_init(void) {
    THE_STACK_TRACE;
#ifdef WITH_THE_WINDOWS_VISTA_THREAD_POOL
    if (this->pool == nullptr) {
        THE_THREADING_TRACE_DEBUG("Thread pool 0x%p is being initialised "
            "lazily...\n", this);
        THE_ASSERT(this->cleanupGroup == nullptr);

        if ((this->pool = ::CreateThreadpool(nullptr)) == nullptr) {
            this->assert_cleanup();
            THROW_THE_SYSTEM_EXCEPTION;
        }

        if ((this->cleanupGroup = ::CreateThreadpoolCleanupGroup())
                == nullptr) {
            this->assert_cleanup();
            THROW_THE_SYSTEM_EXCEPTION;
        }

        //THE_ASSERT(this->cleanupGroup != nullptr);
        //::SetThreadpoolCallbackCleanupGroup(&this->env, this->cleanupGroup,
        //    thread_pool::cleanup_func);
    }
    THE_ASSERT(this->pool != nullptr);
    THE_ASSERT(this->cleanupGroup != nullptr);

#else /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
    if (this->cntTotalThreads < 1) {
        int cntProcessors = system_information::processors();
        THE_THREADING_TRACE_DEBUG("Initial %d worker threads for pool 0x%p are "
            "being spawned...\n", cntProcessors, this);
        this->set_min_threads(cntProcessors);
    }
#endif /* WITH_THE_WINDOWS_VISTA_THREAD_POOL */
}
