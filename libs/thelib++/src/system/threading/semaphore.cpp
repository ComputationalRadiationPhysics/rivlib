/*
 * src\system\threading\semaphore.cpp
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
 * Semaphore.cpp
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Müller. Alle Rechte vorbehalten.
 */

#include "the/system/threading/semaphore.h"

#include <limits>

#if defined(THE_LINUX)
#include <ctime>
#include <fcntl.h>  // For O_CREAT. Thank you for documenting that at sem_open.
#endif /* defined(THE_LINUX) */

#include "the/assert.h"
#include "the/memory.h"
#include "the/not_implemented_exception.h"
#include "the/stack_trace.h"

#include "the/system/system_error.h"
#include "the/system/system_exception.h"


/*
 * the::system::threading::semaphore::timeout_infinite
 */
const the::system::threading::semaphore::timeout_type
the::system::threading::semaphore::timeout_infinite =
#if defined(THE_WINDOWS)
    INFINITE;
#else /* defined(THE_WINDOWS) */
    std::numeric_limits<the::system::threading::semaphore::timeout_type>::max();
#endif /* defined(THE_WINDOWS) */


/*
 * the::system::threading::semaphore::semaphore
 */ 
the::system::threading::semaphore::semaphore(counter_type initialCount,
        counter_type maxCount) : base() {
    THE_STACK_TRACE;
    semaphore::enforceParamAssertions(initialCount, maxCount);

#if defined(THE_WINDOWS)
    this->handle = ::CreateSemaphore(NULL, initialCount, maxCount, NULL);
    THE_ASSERT(this->handle != NULL);

#elif defined(THE_LINUX)
    this->handle = new sem_t;
    ::sem_init(this->handle, 0, initialCount);

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::semaphore
 */
the::system::threading::semaphore::semaphore(const char *name,
        counter_type initialCount, counter_type maxCount, bool *outIsNew)
        : base() {
    THE_STACK_TRACE;
    semaphore::enforceParamAssertions(initialCount, maxCount);

    if (outIsNew != nullptr) {
        *outIsNew = false;
    }

#if defined(THE_WINDOWS)
    /* Try to open existing semaphore first. */
    if ((this->handle = ::OpenSemaphoreA(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE,
            FALSE, name)) == NULL) {
        this->handle = ::CreateSemaphoreA(NULL, initialCount, maxCount, name);
        if (outIsNew != NULL) {
            *outIsNew = true;
        }
    }
    THE_ASSERT(this->handle != NULL);

#elif false&&defined(THE_LINUX)
    // TODO
    if (name != NULL) {
        this->name = TranslateWinIpc2PosixName(name);
        VLTRACE(Trace::LEVEL_VL_INFO, "Open named POSIX semaphore \"%s\"\n", 
            this->name.PeekBuffer());
        if ((this->handle = ::sem_open(this->name.PeekBuffer(), 0, 0, 0)) 
                == SEM_FAILED) {
            this->handle = ::sem_open(this->name.PeekBuffer(), O_CREAT, 
                DFT_PERMS, initialCount);
            if (outIsNew != NULL) {
                *outIsNew = true;
            }
        }
        ASSERT(this->handle !=  SEM_FAILED);
    } else {
        this->handle = new sem_t;
        ::sem_init(this->handle, 0, initialCount); 
        if (outIsNew != NULL) {
            *outIsNew = true;
        }
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::semaphore
 */
the::system::threading::semaphore::semaphore(const wchar_t *name,
        counter_type initialCount, counter_type maxCount, bool *outIsNew)
        : base() {
    THE_STACK_TRACE;
    semaphore::enforceParamAssertions(initialCount, maxCount);

    if (outIsNew != nullptr) {
        *outIsNew = false;
    }

#if defined(THE_WINDOWS)
    /* Try to open existing semaphore first. */
    if ((this->handle = ::OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE,
            FALSE, name)) == NULL) {
        this->handle = ::CreateSemaphoreW(NULL, initialCount, maxCount, name);
        if (outIsNew != NULL) {
            *outIsNew = true;
        }
    }
    THE_ASSERT(this->handle != NULL);

#elif false&&defined(THE_LINUX)
    // TODO
    if (name != NULL) {
        this->name = TranslateWinIpc2PosixName(name);
        VLTRACE(Trace::LEVEL_VL_INFO, "Open named POSIX semaphore \"%ls\"\n", 
            this->name.PeekBuffer());
        if ((this->handle = ::sem_open(this->name.PeekBuffer(), 0, 0, 0)) 
                == SEM_FAILED) {
            this->handle = ::sem_open(this->name.PeekBuffer(), O_CREAT, 
                DFT_PERMS, initialCount);
            if (outIsNew != NULL) {
                *outIsNew = true;
            }
        }
        ASSERT(this->handle !=  SEM_FAILED);
    } else {
        this->handle = new sem_t;
        ::sem_init(this->handle, 0, initialCount);
        if (outIsNew != NULL) {
            *outIsNew = true;
        }
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::semaphore
 */
the::system::threading::semaphore::semaphore(semaphore&& rhs)
        : base(std::move(rhs)), handle(rhs.handle) {
    THE_STACK_TRACE;
    rhs.handle = NULL;

#if defined(THE_LINUX)
    this->name = rhs.name;
    rhs.name.clear();
#endif /* defined(THE_LINUX) */
}


/*
 * the::system::threading::semaphore::~semaphore(void)
 */
the::system::threading::semaphore::~semaphore(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    ::CloseHandle(this->handle);

#elif defined(THE_LINUX)
    if (this->name.empty()) {
        ::sem_destroy(this->handle);
        the::safe_delete(this->handle);
    } else {
        ::sem_unlink(this->name.c_str());
        ::sem_close(this->handle);
        // mueller: I assume that this->handle is owned by the system if it was
        // returned by sem_open, but the documentation says nothing about that.
        // I know, I should not be surprised about that ...
        this->name.clear();
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::lock
 */
void the::system::threading::semaphore::lock(void) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    switch (::WaitForSingleObject(this->handle, INFINITE)) {
        case WAIT_OBJECT_0:
            /* falls through. */
        case WAIT_ABANDONED:
            /* Does nothing. */
            break;

        case WAIT_TIMEOUT:
            /* Waiting infinitely should not timeout. */
            ASSERT(false);
            break;

        default:
            THROW_THE_SYSTEM_EXCEPTION;
    }

#elif defined(THE_LINUX)
    if (::sem_wait(this->handle) == -1) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::try_lock
 */
bool the::system::threading::semaphore::try_lock(const timeout_type timeout) {
    THE_STACK_TRACE;
#if defined(THE_WINDOWS)
    switch (::WaitForSingleObject(this->handle, timeout)) {
        case WAIT_OBJECT_0:
            /* falls through. */
        case WAIT_ABANDONED:
            return true;

        case WAIT_TIMEOUT:
            return false;

        default:
            THROW_THE_SYSTEM_EXCEPTION;
    }

#elif defined(THE_LINUX)
    system_error error;
    int status = 0;

    if (timeout == 0) {
        status = ::sem_trywait(this->handle);
    } else {
        struct timespec tsEnd;

        ::clock_gettime(CLOCK_REALTIME, &tsEnd);
        tsEnd.tv_sec += timeout / 1000;
        tsEnd.tv_nsec += (timeout % 1000) * 1000;

        status = ::sem_timedwait(this->handle, &tsEnd);
    }

    if (status != -1) {
        return true;
    } else {
        error = system_error::last();
        if ((error == EAGAIN) || (error == ETIMEDOUT)) {
            return false;
        } else {
            THROW_THE_SYSTEM_EXCEPTION;
        }
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::unlock
 */
void the::system::threading::semaphore::unlock(void) {
#if defined(THE_WINDOWS)
    if (::ReleaseSemaphore(this->handle, 1, NULL) == FALSE) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#elif defined(THE_LINUX)
    if (::sem_post(this->handle) == -1) {
        THROW_THE_SYSTEM_EXCEPTION;
    }

#else /* defined(THE_WINDOWS) */
    THROW_THE_NOT_IMPLEMENTED_EXCEPTION;
#endif /* defined(THE_WINDOWS) */
}


/*
 * the::system::threading::semaphore::operator =
 */
the::system::threading::semaphore&
the::system::threading::semaphore::operator =(semaphore&& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->handle = rhs.handle;
        rhs.handle = NULL;

#if defined(THE_LINUX)
        this->name = rhs.name;
        rhs.name.clear();
#endif /* defined(THE_LINUX) */
    }
    return *this;
}


/*
 * the::system::threading::semaphore::enforceParamAssertions
 */
void the::system::threading::semaphore::enforceParamAssertions(
        counter_type& inOutInitialCount, counter_type& inOutMaxCount) {
    THE_STACK_TRACE;
    if (inOutMaxCount <= 0) {
        inOutMaxCount = 1;
    }

    if (inOutInitialCount < 0) {
        inOutInitialCount = 0;
    } else if (inOutInitialCount > inOutMaxCount) {
        inOutInitialCount = inOutMaxCount;
    }

    THE_ASSERT(inOutMaxCount > 0);
    THE_ASSERT(inOutInitialCount >= 0);
    THE_ASSERT(inOutInitialCount <= inOutMaxCount);
}


#if defined(THE_LINUX)
/*
 * the::system::threading::semaphore::DFT_PERMS
 */
const int the::system::threading::semaphore::DFT_PERMS = 0666;
#endif /* defined(THE_LINUX) */
