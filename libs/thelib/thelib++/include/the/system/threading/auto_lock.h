/*
 * the/system/threading/auto_lock.h
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
 * AutoLock.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_AUTO_LOCK_H_INCLUDED
#define THE_SYSTEM_THREADING_AUTO_LOCK_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/not_copyable.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace threading {


    /**
     * This class provides a simplifies mechanism for using synchronisation
     * objects. It acquires the lock passed in the constructor on construction
     * and releases it in the dtor.
     *
     * The template parameter must be a lock object that provides the methods
     * lock() for acquiring the lock and unlock() for releaseing it.
     */
    template<class T> class auto_lock : not_copyable {

    public:

        /**
         * Acqures 'lock' and stores it for releasing it in dtor.
         *
         * WARNING: This ctor might throw an exception, if the lock cannot be
         * acquired even when waiting infinitely. You should not go on if this
         * happens.
         *
         * @throws system_exception If the lock could not be acquired.
         */
        auto_lock(T& lock);

        /**
         * The dtor releases 'lock'.
         */
        ~auto_lock(void);

    private:

        /** The synchronisation object that is hold by the lock. */
        T& lock;

    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::threading::auto_lock<T>::auto_lock
 */
template<class T>
the::system::threading::auto_lock<T>::auto_lock(T& lock) : lock(lock) {
    THE_STACK_TRACE;
    this->lock.lock();
}


/*
 * the::system::threading::auto_lock<T>::~auto_lock
 */
template<class T>
the::system::threading::auto_lock<T>::~auto_lock(void) {
    THE_STACK_TRACE;
    this->lock.unlock();
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_AUTO_LOCK_H_INCLUDED */
