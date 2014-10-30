/*
 * the/system/threading/mutex.cpp
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
 * Mutex.cpp
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#include "the/system/threading/mutex.h"

#include "the/system/system_exception.h"
#include "the/assert.h"
#include "the/config.h"
#include "the/stack_trace.h"


/*
 * the::system::threading::mutex::mutex
 */ 
the::system::threading::mutex::mutex(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    this->handle = ::CreateMutex(NULL, FALSE, NULL);
    ASSERT(this->handle != NULL);

#else /* THE_WINDOWS */
    this->handle = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::mutex::mutex
 */
the::system::threading::mutex::mutex(const astring& name) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    this->handle = ::CreateMutexA(NULL, FALSE, name.c_str());
    ASSERT(this->handle != NULL);

#else /* THE_WINDOWS */
    this->handle = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::mutex::mutex
 */
the::system::threading::mutex::mutex(const wstring& name) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    this->handle = ::CreateMutexW(NULL, FALSE, name.c_str());
    ASSERT(this->handle != NULL);

#else /* THE_WINDOWS */
    this->handle = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::mutex::~mutex
 */
the::system::threading::mutex::~mutex(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    ::CloseHandle(this->handle);

#else /* THE_WINDOWS */
    if (::pthread_mutex_destroy(&this->handle) == EBUSY) {
        this->unlock();
        ::pthread_mutex_destroy(&this->handle);
    }

#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::mutex::lock
 */
void the::system::threading::mutex::lock(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
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
            /* An actual error occurred. */
            throw system_exception(__FILE__, __LINE__);
    }

#else /* THE_WINDOWS */
    int returncode = ::pthread_mutex_lock(&this->handle);
    if (returncode != 0) {
        throw system_exception(returncode, __FILE__, __LINE__);
    }

#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::mutex::try_lock
 */
bool the::system::threading::mutex::try_lock(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    switch (::WaitForSingleObject(this->handle, 0)) {

        case WAIT_OBJECT_0:
            /* falls through. */
        case WAIT_ABANDONED:
            return true;

        case WAIT_TIMEOUT:
            return false;

        default:
            /* An actual error occurred. */
            throw system_exception(__FILE__, __LINE__);
    }

#else /* THE_WINDOWS */
    int returncode = ::pthread_mutex_trylock(&this->handle);

    switch (returncode) {
        case 0:
            return true;

        case EBUSY:
            return false;

        default:
            throw system_exception(returncode, __FILE__, __LINE__);
    }

#endif /* THE_WINDOWS */
}


/*
 * the::system::threading::mutex::unlock
 */
void the::system::threading::mutex::unlock(void) {
    THE_STACK_TRACE;
#ifdef THE_WINDOWS
    if (::ReleaseMutex(this->handle) != TRUE) {
        throw system_exception(__FILE__, __LINE__);
    }

#else /* THE_WINDOWS */
    int returncode = ::pthread_mutex_unlock(&this->handle);
    if (returncode != 0) {
        throw system_exception(returncode, __FILE__, __LINE__);
    }

#endif /* THE_WINDOWS */
}
