/*
 * the/system/threading/runnable_thread.h
 *
 * Copyright (c) 2013, TheLib Team (http://www.thelib.org/license)
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
 * RunnableThread.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 * Copyright (C) 2008 by Christoph Müller. Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_RUNNABLE_THREAD_H_INCLUDED
#define THE_SYSTEM_THREADING_RUNNABLE_THREAD_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/assert.h"
#include "the/invalid_operation_exception.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/system/system_string.h"

#include "the/system/threading/runnable.h"
#include "the/system/threading/thread.h"


namespace the {
namespace system {
namespace threading {


    /**
     * This template is for faciliating thread usage. It represents a
     * the::system::threading::thread that is its own
     * the::system::threading::runnable.
     *
     * There are some restrictions for instances of this class in contrast
     * to the::system::threading::thread: the most important is that
     * runnable_threads must not be destroyed while the (OS) thread executing
     * them is running. The reason for that is that the runnable_thread is its
     * own runnable and hence holds all resources that are required for 
     * execution. Hence, it must be ensured that the thread object lives and 
     * provides these resources exclusively to the running system thread as 
     * long as the system thread has not exited. The destructor of the thread
     * will abort the programme if this assumption is violated. However, if you
     * detach the thread, this check cannot be performed. The user is 
     * responsible for keeping the data alive as long as the OS thread is
     * running in this case.
     * A second restriction is that all runnables that can be executed in a
     * runnable_thread must have a default constructor.
     *
     * @tparam T The type of the::system::threading::runnable to be executed by
     *           the thread. The runnable must have a default constructor.
     */
    template<class T> class runnable_thread : public thread, public T {

    public:

        /** The type of the runnable that thread is executing. */
        typedef T runnable_type;

        /** Ctor. */
        runnable_thread(void);

        /** Dtor. */
        virtual ~runnable_thread(void);

    protected:

        /** Super class typedef. */
        typedef the::system::threading::thread base;

    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::threading::runnable_thread<T>::runnable_thread
 */
template<class T>
the::system::threading::runnable_thread<T>::runnable_thread(void)
#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif /* _MSC_VER */
        : base(this), runnable_type() {
#ifdef _MSC_VER
#pragma warning(default: 4355)
#endif /* _MSC_VER */
    THE_STACK_TRACE;
}


/*
 * the::system::threading::runnable_thread<T>::~runnable_thread
 */
template<class T>
the::system::threading::runnable_thread<T>::~runnable_thread(void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_RUNNABLE_THREAD_H_INCLUDED */
