/*
 * src\system\threading\event.cpp
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
/*
 * Event.cpp
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#include "the/system/threading/event.h"

#include "the/assert.h"
#include "the/stack_trace.h"

#include "the/system/system_exception.h"

#include "the/system/threading/debug.h"
#include "the/system/threading/thread.h"


/*
 * the::system::threading::event::timeout_infinite
 */
const the::system::threading::event::timeout_type
the::system::threading::event::timeout_infinite
    = the::system::threading::semaphore::timeout_infinite;


/*
 * the::system::threading::event::event
 */
the::system::threading::event::event(const bool isManualReset,
        const bool isInitiallySignaled) : base()
#if defined(THE_LINUX)
        , isManualReset(isManualReset),
        sem(isInitiallySignaled ? 1 : 0, 1)
#endif /* defined(THE_LINUX) */
        {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    this->handle = ::CreateEventA(NULL, isManualReset ? TRUE : FALSE,
        isInitiallySignaled ? TRUE : FALSE, NULL);
    THE_ASSERT(this->handle != NULL);
#endif /* defined(THE_WINDOWS) */
}


/*
 *the::system::threading::event::event
 */
the::system::threading::event::event(const char *name,
        const bool isManualReset, const bool isInitiallySignaled,
        bool *outIsNew) : base()
#if defined(THE_LINUX)
        , isManualReset(isManualReset),
        sem(name, isInitiallySignaled ? 1 : 0, 1, outIsNew)
#endif /* defined(THE_LINUX) */
        {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (outIsNew != NULL) {
        *outIsNew = false;
    }

    /* Try to open existing event first. */
    if ((this->handle = ::OpenEventA(SYNCHRONIZE | EVENT_MODIFY_STATE,
            FALSE, name)) == NULL) {
        this->handle = ::CreateEventA(NULL, isManualReset ? TRUE : FALSE,
            isInitiallySignaled ? TRUE : FALSE, name);
        if (outIsNew != NULL) {
            *outIsNew = true;
        }
    }
    THE_ASSERT(this->handle != NULL);
#endif /* _WIN32 */
}


/*
 * the::system::threading::event::event
 */
the::system::threading::event::event(const wchar_t *name,
        const bool isManualReset, const bool isInitiallySignaled,
        bool *outIsNew) : base()
#if defined(THE_LINUX)
        , isManualReset(isManualReset),
        sem(name, isInitiallySignaled ? 1 : 0, 1, outIsNew)
#endif /* defined(THE_LINUX) */
        {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (outIsNew != NULL) {
        *outIsNew = false;
    }

    /* Try to open existing event first. */
    if ((this->handle = ::OpenEventW(SYNCHRONIZE | EVENT_MODIFY_STATE,
            FALSE, name)) == NULL) {
        this->handle = ::CreateEventW(NULL, isManualReset ? TRUE : FALSE,
            isInitiallySignaled ? TRUE : FALSE, name);
        if (outIsNew != NULL) {
            *outIsNew = true;
        }
    }
    THE_ASSERT(this->handle != NULL);
#endif /* _WIN32 */
}


/*
 * the::system::threading::event::~event
 */
the::system::threading::event::~event(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    ::CloseHandle(this->handle);
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::event::reset
 */
void the::system::threading::event::reset(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (::ResetEvent(this->handle) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#elif defined(THE_LINUX)
    THE_THREADING_TRACE_DEBUG("Entering %s...\n", THE_FUNCTION);
    this->sem.try_lock();
    THE_ASSERT(!this->sem.try_lock());

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::event::set
 */
void the::system::threading::event::set(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    if (::SetEvent(this->handle) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#elif defined(THE_LINUX)
    THE_THREADING_TRACE_DEBUG("Entering %s...\n", THE_FUNCTION);
    this->sem.try_lock();
    this->sem.unlock();

    // Force Linux scheduler not to behave like a complete idiot. Totally
    // strange behavour was reported by sgrottel without this. Furthermore,
    // thread::yield() does not seem to be sufficient...
    thread::sleep(1);

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::event::wait
 */
bool the::system::threading::event::wait(const timeout_type timeout) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    switch (::WaitForSingleObject(this->handle, timeout)) {
        case WAIT_OBJECT_0:
            /* falls through. */
        case WAIT_ABANDONED:
            return true;
            /* Unreachable. */

        case WAIT_TIMEOUT:
            return false;
            /* Unreachable. */

        default:
            THROW_THE_SYSTEM_EXCEPTION;
            /* Unreachable. */
    }

#elif defined(THE_LINUX)
    THE_THREADING_TRACE_DEBUG("Entering %s...\n", THE_FUNCTION);
    bool retval = false;

    if (timeout == timeout_infinite) {
        this->sem.lock();
        retval = true;

    } else {
        retval = this->sem.try_lock(timeout);
    }

    if (retval && this->isManualReset) {
        THE_THREADING_TRACE_DEBUG("%s signals again...\n", THE_FUNCTION);
        this->sem.unlock();
    }

    return retval;

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}
