/*
 * include\the\system\threading\semaphore.h
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
 * Semaphore.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Müller. Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_SEMAPHORE_H_INCLUDED
#define THE_SYSTEM_THREADING_SEMAPHORE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#if defined(THE_WINDOWS)
#include <windows.h>
#elif defined(THE_LINUX)
#include <semaphore.h>
#endif /* defined(THE_WINDOWS) */

#include "the/not_copyable.h"

#include "the/system/system_string.h"


namespace the {
namespace system {
namespace threading {

    /**
     * A platform-independent semaphore.
     *
     * Named instances of these semaphores can be used for inter-process 
     * synchronisation tasks. These system-wide semaphores are destroyed once
     * the last object is destroyed.
     *
     * Note: Maximum count is not supported on Linux and ignored.
     *
     * @author Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de)
     */
    class semaphore : public the::not_copyable {

    public:

        /** The type used for the semaphore's counter. */
        typedef long counter_type;

        /** The handle for the OS semaphore. */
#if defined(THE_WINDOWS)
        typedef HANDLE native_handle_type;
#elif defined(THE_LINUX)
        typedef sem_t *native_handle_type;
#else /* defined(THE_WINDOWS) */
#error "the::system::threading::semaphore must define native_handle_type!"
#endif /* defined(THE_WINDOWS) */

        /** The type for specifying timeouts. */
        typedef unsigned int timeout_type;

        /** 
         * Use this value to wait infinitely for an event to become
         * signaled. 
         */
        static const timeout_type timeout_infinite;

        /**
         * Create a new semaphore.
         *
         * If no initial count it set, the semaphore is created with a count of 1,
         * i. e. the semaphore is initially in signaled state.
         *
         * @param initialCount The initial count for the semaphore object. This
         *                     value must be within [0, maxCount]. If the value
         *                     is not within this range, it will be clamped to
         *                     be valid.
         * @param maxCount     The maximum count for the semaphore object, which
         *                     must be greater than zero. If the value is less
         *                     than 1, it will be corrected to be 1.
         */
        semaphore(counter_type initialCount = 1, counter_type maxCount = 1);

        /** 
         * Open or create a new semaphore with the specified name. The ctor
         * first tries to open an existing semaphore and creates a new one, if
         * such a semaphore does not exist.
         *
         * If no initial count it set, the semaphore is created with a count of 1,
         * i. e. the semaphore is initially in signaled state.
         *
         * @param name         The name of the semaphore.
         * @param initialCount The initial count for the semaphore object. This
         *                     value must be within [0, maxCount]. If the value
         *                     is not within this range, it will be clamped to
         *                     be valid.
         * @param maxCount     The maximum count for the semaphore object, which
         *                     must be greater than zero. If the value is less
         *                     than 1, it will be corrected to be 1.
         * @param outIsNew     If not NULL, the ctor returns whether the
         *                     semaphore was created (true) or opened (false).
         */
        semaphore(const char *name, counter_type initialCount = 1,
            counter_type maxCount = 1, bool *outIsNew = nullptr);

        /** 
         * Open or create a new semaphore with the specified name. The ctor
         * first tries to open an existing semaphore and creates a new one, if
         * such a semaphore does not exist.
         *
         * If no initial count it set, the semaphore is created with a count of 1,
         * i. e. the semaphore is initially in signaled state.
         *
         * @param name         The name of the semaphore.
         * @param initialCount The initial count for the semaphore object. This
         *                     value must be within [0, maxCount]. If the value
         *                     is not within this range, it will be clamped to
         *                     be valid.
         * @param maxCount     The maximum count for the semaphore object, which
         *                     must be greater than zero. If the value is less
         *                     than 1, it will be corrected to be 1.
         * @param outIsNew     If not NULL, the ctor returns whether the
         *                     semaphore was created (true) or opened (false).
         */
        semaphore(const wchar_t *name, counter_type initialCount = 1,
            counter_type maxCount = 1, bool *outIsNew = nullptr);

        /**
         * Move 'rhs' to this object.
         *
         * @param rhs The object to be moved.
         */
        semaphore(semaphore&& rhs);

        /** Dtor. */
        virtual ~semaphore(void);

        /**
         * Acquire a lock on the semaphore. This method blocks until the lock is
         * acquired.
         *
         * The lock can be acquired, if the state of the semaphore is signaled,
         * i.e. the counter is greater than zero. If a lock has been
         * successfully acquired, the counter is decremented by one and if the
         * counter reaches zero, the state of the semaphore becomes nonsignaled.
         *
         * @throws the::system::system_exception If the lock could not be
         *                                       acquired.
         */
        void lock(void);

        /**
         * Try acquiring a lock on the semaphore for the calling thread for the
         * specified number of milliseconds. If the semaphore could not be locked
         * within the specified amount of time, the method returns and the return
         * value is false.
         *
         * @param timeout The number of milliseconds to wait for acquiring the
         *                semaphore. This parameter defaults to zero, i.e. the
         *                method will return immediately if the semaphore cannot
         *                be locked.
         *
         * @return true, if the lock was acquired, false, if not.
         *
         * @throws the::system::system_exception If the lock could not be
         *                                       acquired.
         */
        bool try_lock(const timeout_type timeout = 0);

        /**
         * Release the semaphore.
         *
         * The counter is incremented by one. The state of the semaphore becomes
         * signaled, as the counter becomes greater than zero, if the semaphore
         * was successfully released.
         *
         * @throw the::system::system_exception If the lock could not be
         *                                      released.
         */
        void unlock(void);

        /**
         * Move values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        semaphore& operator =(semaphore&& rhs);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

        /**
         * Check the initialisation parameters and ensure that
         *
         * - 'inOutMaxCount' is at least 1
         * - 'inOutInitialCount' is within [0, inOutMaxCount]
         */
        static void enforceParamAssertions(counter_type& inOutInitialCount,
            counter_type& inOutMaxCount);

#if defined(THE_LINUX)
        /** The default permissions assigned to the semaphore. */
        static const int DFT_PERMS;
#endif /* defined(THE_LINUX) */

        /** The native semaphore handle. */
        native_handle_type handle;

#if defined(THE_LINUX)
        /** The name of the semaphore if it is named. */
        the::system::system_string name;
#endif /* defined(THE_LINUX) */
    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_SEMAPHORE_H_INCLUDED */
