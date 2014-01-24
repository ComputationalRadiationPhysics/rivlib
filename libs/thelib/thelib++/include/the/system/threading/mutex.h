/*
 * the/system/threading/mutex.h
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
 * Mutex.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_MUTEX_H_INCLUDED
#define THE_SYSTEM_THREADING_MUTEX_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <string>

#ifdef THE_WINDOWS
#include <windows.h>
#else /* THE_WINDOWS */
#include <pthread.h>
#endif /* THE_WINDOWS */

#include "the/not_copyable.h"
#include "the/string.h"

#include "the/system/threading/auto_lock.h"


namespace the {
namespace system {
namespace threading {

    /**
     * A platform independent mutex wrapper.
     *
     * Implementation notes: On Windows systems, this mutex can be used for
     * inter-process synchronisation tasks. If you just need to synchronise 
     * threads of a single process, consider using the critical section as it
     * is faster.
     *
     * @author Christoph Mueller
     */
    class mutex : not_copyable {

    public:

        /** A scoped lock type for mutexes. */
        typedef auto_lock<mutex> auto_lock_type;

        /**
         * The type of the native mutex handle.
         */
#if defined(THE_WINDOWS)
        typedef HANDLE native_handle_type;
#elif defined(THE_LINUX)
        typedef pthread_mutex_t native_handle_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::threading::mutex must define native_handle_type!"
#endif /* defined(THE_WINDOWS) */

        /**
         * Create a new mutex, which is initially not locked.
         */
        mutex(void);

        /**
         * Create a new, named mutex, which is initially not locked.
         *
         * On Windows, if 'name' matches the name of an existing named mutex 
         * object, the mutex requests the MUTEX_ALL_ACCESS access right.
         *
         * On Linux, naming mutex is not supported and this ctor is equivalent
         * to the one not specifying a name.
         *
         * @param name The name of the mutex.
         */
        mutex(const astring& name);

        /**
         * Create a new, named mutex, which is initially not locked.
         *
         * On Windows, if 'name' matches the name of an existing named mutex 
         * object, the mutex requests the MUTEX_ALL_ACCESS access right.
         *
         * On Linux, naming mutex is not supported and this ctor is equivalent
         * to the one not specifying a name.
         *
         * @param name The name of the mutex.
         */
        mutex(const wstring& name);

        /** 
         * Dtor.
         */
        virtual ~mutex(void);

        /**
         * Acquire a lock on the mutex for the calling thread. The method blocks
         * until the lock is acquired. 
         *
         * @throws the::system_exception If the lock could not be acquired.
         */
        void lock(void);

        /**
         * Try to acquire a lock on the mutex for the calling thread. If the 
         * mutex is already locked by another thread, the method will return
         * immediately and the return value is false. The method is therefore 
         * non-blocking.
         *
         * @return true, if the lock was acquired, false, if not.
         *
         * @throws the::system_exception If an error occurred when trying to 
         *                               acquire the lock.
         */
        bool try_lock(void);

        /**
         * Release the mutex.
         *
         * @throws the::system_exception If an error occurred when trying to 
         *                               acquire the lock.
         */
        void unlock(void);

    private:

        /** The OS handle for the mutex. */
        native_handle_type handle;
    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_MUTEX_H_INCLUDED */
