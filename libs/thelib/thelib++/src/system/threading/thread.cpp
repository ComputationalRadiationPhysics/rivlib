/*
 * the/system/threading/thread.cpp
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
 * Thread.cpp  15.08.2006 (mueller)
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 */

#include "the/system/threading/thread.h"

#include <cstdio>
#include <iostream>
#include <sstream>

#ifdef THE_LINUX
#include <unistd.h>
#endif /* THE_LINUX */

#include "the/argument_null_exception.h"
#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/memory.h"
#include "the/trace.h"

#include "the/system/system_error.h"
#include "the/system/system_exception.h"
#include "the/system/system_string.h"

#include "the/text/string_builder.h"

#include "the/system/threading/debug.h"
#include "the/system/threading/runnable.h"

//#include "DynamicFunctionPointer.h"


/** Converts a thread::id object to an ANSI C-style string. */
#define THE_THREAD_ID_STR(id) (id).to_astring().c_str()

/** Represents the ID of the calling thread as ANSI C-style string. */
#define THE_THREADING_CURRRENT_ID_STR\
    (THE_THREAD_ID_STR(the::system::threading::thread::get_current_id()))


#ifndef _WIN32
/**
 * Return code that marks a thread as still running. Make sure that the value 
 * is the same as on Windows.
 */
#define STILL_ACTIVE (259)
#endif /* !_WIN32 */


////////////////////////////////////////////////////////////////////////////////
#pragma region id

/*
 * the::system::threading::thread::id::empty
 */
the::system::threading::thread::id the::system::threading::thread::id::empty;


/*
 * the::system::threading::thread::id::id
 */
the::system::threading::thread::id::id(void) : nativeId(0) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread::id::id
 */
the::system::threading::thread::id::id(const native_id_type& nativeId) 
        : nativeId(nativeId) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread::id::id
 */
the::system::threading::thread::id::id(const id& id) : nativeId(id.nativeId) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread::id::~id
 */
the::system::threading::thread::id::~id(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread::id::compare
 */
int the::system::threading::thread::id::compare(const id& rhs) const {
    THE_STACK_TRACE;
    return (this->nativeId - rhs.nativeId);
}


/*
 * the::system::threading::thread::id::to_astring
 */
the::astring the::system::threading::thread::id::to_astring(void) const {
    THE_STACK_TRACE;
    std::ostringstream str;
    //str.imbue(std::locale(""));
    str << this->nativeId;
    return str.str();
}


/*
 * the::system::threading::thread::id::to_wstring
 */
the::wstring the::system::threading::thread::id::to_wstring(void) const {
    THE_STACK_TRACE;
    std::wostringstream str;
    //str.imbue(std::locale(""));
    str << this->nativeId;
    return str.str();
}


/*
 * the::system::threading::thread::id::operator =
 */
the::system::threading::thread::id&
the::system::threading::thread::id::operator =(const id& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->nativeId = rhs.nativeId;
    }
    return *this;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////


/*
 * the::system::threading::thread::sleep
 */
void the::system::threading::thread::sleep(const unsigned long millis) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    ::Sleep(millis);

#elif THE_LINUX
    if (millis >= 1000) {
        /* At least one second to sleep. Use ::sleep() for full seconds. */
        ::sleep(millis / 1000);
    }

    ::usleep((millis % 1000) * 1000);

#else /* THE_WINDOWS */
#error "thread::sleep() is missing."
#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::thread::yield
 */
void the::system::threading::thread::yield(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
#if (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400))
    ::SwitchToThread();
#else /* (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)) */
    // TODO
    ::Sleep(0);
    //DynamicFunctionPointer<BOOL (*)(void)> stt("kernel32", "SwitchToThread");
    //if (stt.IsValid()) {
    //    stt();
    //} else {
    //    ::Sleep(0);
    //}
#endif /* (defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0400)) */

#elif THE_LINUX
    if (::sched_yield() != 0) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#else /* THE_WINDOWS */
#error "thread::yield() is missing."
#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::thread::thread
 */
the::system::threading::thread::thread(threading::runnable *run)
        : theRunnable(run) {
    THE_STACK_TRACE;
    THE_ASSERT(this->theRunnable != nullptr);

#ifdef THE_WINDOWS
    this->handle = NULL;
#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::thread::~thread
 */
the::system::threading::thread::~thread(void) {
    THE_STACK_TRACE;
    if (this->is_running()) {
        THE_THREADING_TRACE_ERROR("A thread object of a running thread is "
            "about to be destroyed. This is illegal. Please ensure that a "
            "thread has exited (e.g. by joining it) or has been detached "
            "before destroying the thread object.", nullptr);
        std::abort();
    }
}


/*
 * the::system::threading::thread::detach
 */
void the::system::threading::thread::detach(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    if (::CloseHandle(this->handle)) {
        this->handle = NULL;
        this->theRunnable = nullptr;
    } else {
        THROW_THE_SYSTEM_EXCEPTION;
    }
    
#elif THE_LINUX
    if (::pthread_detach(this->handle) == 0) {
        this->handle = 0;
        this->theRunnable = nullptr;
        this->statusBlock = nullptr;
    } else {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#else /* THE_WINDOWS */
#error "thread::detach() is missing."
#endif /* THE_WINDOWS */

    // The invariant of detaching is that the thread object is unused, i.e.
    // not representing a valid thread.
    THE_ASSERT(!this->is_running());
}


/*
 * the::system::threading::thread::get_exit_code
 */
the::system::threading::thread::native_exit_code_type
the::system::threading::thread::get_exit_code(void) const {
    THE_STACK_TRACE;

#ifdef THE_WINDOWS
    DWORD retval = 0;
    if (::GetExitCodeThread(this->handle, &retval) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }
    return retval;

#elif THE_LINUX
    if (this->is_running()) {
        return static_cast<native_exit_code_type>(STILL_ACTIVE);
    } else if (this->statusBlock.is_null()) {
        throw system_exception(ENOTSUP, __FILE__, __LINE__);
    } else {
        return this->statusBlock->exit_code();
    }

#else /* THE_WINDOWS */
#error "thread::get_exit_code() is missing."
#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::thread::get_id
 */
the::system::threading::thread::id
the::system::threading::thread::get_id(void) const {
    THE_STACK_TRACE;

#ifdef THE_WINDOWS
    return id(::GetThreadId(this->handle));

#elif THE_LINUX
    return id(this->handle);

#else /* THE_WINDOWS */
#error "thread::get_id() is missing."
#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::thread::is_running
 */
bool the::system::threading::thread::is_running(void) const {
    THE_STACK_TRACE;
    bool retval = false;

#ifdef THE_WINDOWS
    // Note: Win32 handles an invalid handle as signaled.
    retval = ((this->handle != NULL) 
        && (::WaitForSingleObject(this->handle, 0) != WAIT_OBJECT_0));

#elif THE_LINUX
    retval = (this->statusBlock.is_not_null()
        && (this->statusBlock->exit_reason() == unknown));

#else /* THE_WINDOWS */
#error "thread::is_running() is missing."
#endif /* THE_WINDOWS */

    return retval;
}


/*
 * the::system::threading::thread::join
 */
void the::system::threading::thread::join(void) {
    THE_STACK_TRACE;

#ifdef THE_WINDOWS
    if (this->handle != NULL) {
        if (::WaitForSingleObject(this->handle, INFINITE) == WAIT_FAILED) {
            THROW_THE_SYSTEM_EXCEPTION;
        }
    }

#elif THE_LINUX
    if (this->is_running()) {
        if (::pthread_join(this->handle, NULL) != 0) {
            THROW_THE_SYSTEM_EXCEPTION;
        }
    }

#else /* THE_WINDOWS */
#error "thread::join() is missing."
#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::thread::start
 */
void the::system::threading::thread::start(void *userData) {
    THE_STACK_TRACE;
    id::native_id_type tid = 0;

    /* Sanity checks. */
    if (this->is_running()) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("A ")
            THE_SYSTEM_STRING_CONSTANT("thread that is already running ")
            THE_SYSTEM_STRING_CONSTANT("cannot be re-started until it ")
            THE_SYSTEM_STRING_CONSTANT("exited."), __FILE__, __LINE__);
    }
    if (this->theRunnable == NULL) {
        throw invalid_operation_exception(THE_SYSTEM_STRING_CONSTANT("A ")
            THE_SYSTEM_STRING_CONSTANT("thread without a valid runnable ")
            THE_SYSTEM_STRING_CONSTANT("cannot be started."), __FILE__,
            __LINE__);
    }

    /* Inform the runnable that we are about to start a thread. */
    this->theRunnable->on_thread_starting(userData);

#ifdef THE_WINDOWS
    /* Close possible old handle. */
    if (this->handle != NULL) {
        ::CloseHandle(this->handle);
    }

    if ((this->handle = ::CreateThread(NULL, 0, thread::executor_func,
            static_cast<void *>(this->theRunnable), 0, &tid)) == NULL) {
        system_error error = system_error::last();
        THE_THREADING_TRACE_ERROR("The call to CreateThread() failed with "
            "error code %d.\n", error.native_error());
        throw system_exception(error, __FILE__, __LINE__);
    }

#elif THE_LINUX
    // Allocate status block first as this could raise std::bad_alloc.
    this->statusBlock = the::intrusive_ptr<status_block>(
        new status_block(this), false);

    pthread_attr_t attribs;
    ::pthread_attr_init(&attribs);
    ::pthread_attr_setscope(&attribs, PTHREAD_SCOPE_SYSTEM);
    ::pthread_attr_setdetachstate(&attribs, PTHREAD_CREATE_JOINABLE);

    if (::pthread_create(&this->handle, &attribs, thread::executor_func,
            static_cast<void *>(this->statusBlock.operator ->())) != 0) {
        system_error error = system_error::last();
        THE_THREADING_TRACE_ERROR("The call to pthread_create() failed with "
            "error code %d.\n", error.native_error());
        //this->theRunnable->exitReason = crashed;
        ::pthread_attr_destroy(&attribs);
        throw system_exception(error, __FILE__, __LINE__);
    }
    tid = static_cast<id::native_id_type>(this->handle);
    // TODO: Could this produce a memory leak if an exception is thrown in
    // the code above?
    ::pthread_attr_destroy(&attribs);

#else /* THE_WINDOWS */
#error "thread::start() is missing."
#endif /* THE_WINDOWS */

    THE_TRACE(THE_TRCCHL_THELIB_COMMON, THE_TRCLVL_INFO, "Thread [%u] "
        "was created.\n", tid);
}


/*
 * the::system::threading::thread::terminate
 */
void the::system::threading::thread::terminate(const bool doWait,
        const unsigned int reserved, const native_exit_code_type exitCode) {
    THE_STACK_TRACE;
    THE_ASSERT(this->theRunnable != nullptr);

    switch (this->theRunnable->on_thread_terminating()) {

        case graceful:
            THE_THREADING_TRACE_INFO("Thread [%hs] acknowledged graceful "
                "termination.\n", THE_THREAD_ID_STR(this->get_id()));

            if (doWait) {
                THE_THREADING_TRACE_INFO("Waiting for thread [%hs] to "
                    "complete graceful termination.\n",
                    THE_THREAD_ID_STR(this->get_id()));
                this->join();
            }
            break;

        case forceful:
            THE_THREADING_TRACE_INFO("Thread [%hs] will be terminated.\n",
                THE_THREAD_ID_STR(this->get_id()));
#ifdef THE_WINDOWS
            if (::TerminateThread(this->handle, exitCode) == FALSE) {
                THROW_THE_SYSTEM_EXCEPTION;
            } else {
                this->join();   // Prevent race condition wrt. 'exitReason'.
                this->theRunnable->on_thread_terminated(terminated);
            }

#elif THE_LINUX
            // Note: pthread cancellation is asynchronous, so we cannot call the
            // termination callback here, but let the cancellation handler do
            // this.
            THE_ASSERT(!this->statusBlock.is_null());
            this->statusBlock->on_cancelling(exitCode);
            if (::pthread_cancel(this->handle) != 0) {
                THROW_THE_SYSTEM_EXCEPTION;
            }

#else /* THE_WINDOWS */
#error "thread::terminate() is missing."
#endif /* THE_WINDOWS */
            break;

        case unsupported:
        default:
            THROW_THE_EXCEPTION(invalid_operation_exception,
                THE_SYSTEM_STRING_CONSTANT("The runnable does not support ")
                THE_SYSTEM_STRING_CONSTANT("termination."));
            break;
    }
}


#ifdef THE_LINUX
////////////////////////////////////////////////////////////////////////////////
#pragma region status_block

/*
 * the::system::threading::thread::status_block::status_block
 */
the::system::threading::thread::status_block::status_block(thread *owner)
        : exitCode(0), exitReason(thread::exit_reason::unknown) {
    THE_STACK_TRACE;
    static_assert(sizeof(thread::exit_reason) == sizeof(int), "Enumeration is "
        "not mapped to an int.");
    THE_ASSERT(owner != nullptr);
    this->theRunnable = owner->theRunnable;
}


/*
 * the::system::threading::thread::status_block::~status_block
 */
the::system::threading::thread::status_block::~status_block(void) {
    THE_STACK_TRACE;
}


/*
 * the::system::threading::thread::status_block::on_cancelling
 */
void the::system::threading::thread::status_block::on_cancelling(
        const int exitCode) throw() {
    THE_STACK_TRACE;
    THE_THREADING_TRACE_DEBUG("Status block marked cancelled from thread "
        "[%hs].\n", THE_THREADING_CURRRENT_ID_STR);
    this->exitCode = exitCode;

    // Mark the thread as forcefully terminated in case it was running, which
    // is indicated by an unknown 'exitReason'.
    if (this->exitReason == thread::exit_reason::unknown) {
        this->exitReason = thread::exit_reason::terminated;
    }
}


/*
 * the::system::threading::thread::status_block::on_cleanup
 */
void the::system::threading::thread::status_block::on_cleanup(void) throw() {
    THE_STACK_TRACE;
    THE_ASSERT(this->theRunnable != nullptr);
    THE_THREADING_TRACE_DEBUG("Status block cleanup callback of runnable 0x%p "
        "called from thread [%hs].\n", this->theRunnable,
        THE_THREADING_CURRRENT_ID_STR);
    this->theRunnable->on_thread_terminated(this->exit_reason());
}


/*
 * the::system::threading::thread::status_block::on_exited
 */
void the::system::threading::thread::status_block::on_exited(const int exitCode,
        const thread::exit_reason exitReason) throw() {
    THE_STACK_TRACE;
    THE_THREADING_TRACE_DEBUG("Status block marked exited from thread [%hs].\n",
        THE_THREADING_CURRRENT_ID_STR);
    this->exitCode = exitCode;
    this->exitReason = exitReason;
}

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
#endif /* THE_LINUX */


#ifdef THE_LINUX
/*
 * the::system::threading::thread::cleanup_func
 */
void the::system::threading::thread::cleanup_func(void *param) {
    THE_STACK_TRACE;
    THE_ASSERT(param != nullptr);
    executor_param_type p = static_cast<executor_param_type>(param);
    p->on_cleanup();
}
#endif /* THE_LINUX */


/*
 * the::system::threading::thread::executor_func
 */
#ifdef THE_WINDOWS
DWORD WINAPI the::system::threading::thread::executor_func(void *param) {
#else /* THE_WINDOWS */
void *the::system::threading::thread::executor_func(void *param) {
#endif /* THE_WINDOWS */
    THE_STACK_TRACE;
    THE_ASSERT(param != nullptr);

    exit_reason exitReason = exit_reason::unknown;
    int retval = 0;
#if defined(THE_WINDOWS)
    runnable *r = static_cast<executor_param_type>(param);

#elif defined(THE_LINUX)
    // Create a reference-counted local copy of the parameter.
    the::intrusive_ptr<status_block> p(static_cast<executor_param_type>(param));

    // Get the runnable.
    runnable *r = p->runnable();
    THE_THREADING_TRACE_DEBUG("thread::executor_func in [%hs] is executing "
        "runnable 0x%p...\n", THE_THREADING_CURRRENT_ID_STR, r);

    // Install a cancellation cleanup handler in order to determine whether the
    // thread is still running.
    pthread_cleanup_push(thread::cleanup_func, p.operator ->());
#endif /* THE_LINUX */

    /* Inform the runnable that the thread is now running. */
    THE_ASSERT(r != nullptr);
    r->on_thread_started(thread::get_current_id());

    try {
        retval = r->run();
        THE_ASSERT(retval != STILL_ACTIVE); // Should not use STILL_ACTIVE!
        exitReason = exit_reason::exited;
    } catch (...) {
        THE_THREADING_TRACE_WARN("Thread [%hs] crashed due to an uncaught "
            "exception at top level.\n",
            thread::get_current_id().to_astring().c_str());
        exitReason = exit_reason::crashed;
    }

#ifdef THE_WINDOWS
    // Windows will call the termination callback directly.
    r->on_thread_terminated(exitReason);

#elif THE_LINUX
    // Uninstall the cancellation cleanup handler and call it in order to run
    // the same code for normal exit and cancellation. This will call the 
    // termination callback on the runnable.
    p->on_exited(retval, exitReason);
    pthread_cleanup_pop(1);
#endif /* THE_WINDOWS */

    THE_THREADING_TRACE_INFO("Thread [%hs] has exited with code %d (0x%x).\n",
        thread::get_current_id().to_astring().c_str(), retval, retval);

#ifdef THE_WINDOWS
    return static_cast<native_exit_code_type>(retval);
#elif THE_LINUX
    return reinterpret_cast<void *>(retval);
#else /* THE_WINDOWS */
#error "thread::executor_func() is missing."
#endif /* _WIN32 */
}
