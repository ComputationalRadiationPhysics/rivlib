/*
 * the/system/threading/critical_section.h
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
 * CriticalSection.h
 *
 * Copyright (C) 2006 by Universitaet Stuttgart (VIS). Alle Rechte vorbehalten.
 * Copyright (C) 2005 by Christoph Mueller (christoph.mueller@vis.uni-stuttgart.de). Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_CRITICAL_SECTION_H_INCLUDED
#define THE_SYSTEM_THREADING_CRITICAL_SECTION_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#ifdef THE_WINDOWS
#include <windows.h>
#else /* THE_WINDOWS */
#include "the/system/threading/mutex.h"
#endif /* THE_WINDOWS */

#include "the/not_copyable.h"

#include "the/system/threading/auto_lock.h"


namespace the {
namespace system {
namespace threading {

    /**
     * Implements a critical section.
     *
     * Implementation notes: On Windows, the implementation uses a critical 
     * section, which cannot be used for inter-process synchronisation. Only
     * threads of a single process can be synchronised using this class. Use
     * Mutex or Semaphore, if you need inter-process synchronisation or a
     * TryLock() method on systems less than Windows NT 4. Note, 
     * that critical sections are faster than Mutexes or Semaphores.
     *
     * You must compile your program with _WIN32_WINNT defined as 0x0400 or 
     * later to use TryLock on the critical section. TryLock() will always fail
     * otherwise.
     *
     * On Linux systems, the critical section is emulated using a system mutex.
     *
     * @author Christoph Mueller
     */
    class critical_section : not_copyable {

    public:

        /** A scoped lock type for critical sections. */
        typedef auto_lock<critical_section> auto_lock_type;

        /**
         * Creates a new critical section, which is initially not locked.
         */
        critical_section(void);

        /**
         * Dtor. 
         */
        virtual ~critical_section(void);

        /**
         * Enter the crititcal section for the calling thread. The method blocks
         * until the lock is acquired. 
         *
         * @throws the::system_exception If the lock could not be acquired.
         */
        void lock(void);

        /**
         * Try to enter the critical section. If another thread is already in 
         * the critical section, the method will return immediately and the r
         * eturn value is false. The method is therefore non-blocking.
         *
         * NOTE: This method will always return fail on Windows systems prior
         * to Windows NT 4. Only programmes that are compiled with _WIN32_WINNT
         * defined as 0x0400 support this method.
         *
         * @return true, if the lock was acquired, false, if not.
         *
         * @throws the::not_supported_exception On system prior to Windows NT 4.
         * @throws the::system_exception If an error occurred when trying to 
         *                               acquire the lock.
         */
        bool try_lock(void);

        /**
         * Leave the critical section.
         *
         * @throw the::system_exception If the lock could not be released.
         */
        void unlock(void);

    private:

#ifdef THE_WINDOWS
        /** The OS critical section. */
        CRITICAL_SECTION critSect;

#else /* THE_WINDOWS */
        /** The mutex used for protecting the critical section. */
        mutex theMutex;

#endif /* THE_WINDOWS */
    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_CRITICAL_SECTION_H_INCLUDED */
