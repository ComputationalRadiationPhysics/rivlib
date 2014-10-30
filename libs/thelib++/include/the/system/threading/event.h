/*
 * include\the\system\threading\event.h
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
 * Event.h
 *
 * Copyright (C) 2006 - 2007 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 */

#ifndef THE_SYSTEM_THREADING_EVENT_H_INCLUDED
#define THE_SYSTEM_THREADING_EVENT_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/system/threading/semaphore.h"

#include "the/not_copyable.h"


namespace the {
namespace system {
namespace threading {

    /**
     * This class implements a synchronisation object whose state can be 
     * explicitly set. There are two states, signaled and non-signaled. If
     * the event is in signaled state, threads calling the Wait function
     * can proceed, in non-signaled state the threads are blocked.
     *
     * The event can be used as auto-reset and manual-reset event. An
     * auto-reset event resets its state to non-signaled after one thread
     * has been released. A manual-reset event stays signaled until it is
     * explicitly reset.
     */
    class event : public the::not_copyable {

    public:

        /** The type for specifying timeouts. */
        typedef the::system::threading::semaphore::timeout_type timeout_type;

        /** 
         * Use this value to wait infinitely for an event to become
         * signaled. 
         */
        static const timeout_type timeout_infinite;

        /** 
         * Create a new event.
         *
         * The default ctor creates an auto-reset event, which is initially
         * non-signaled.
         *
         * @param isManualReset       Make the event a manual-reset event. If 
         *                            the flag is not set, an auto-reset event 
         *                            is created.
         * @param isInitiallySignaled Specifies whether the event is initially
         *                            signaled.
         */
        event(const bool isManualReset = false,
            const bool isInitiallySignaled = false);

        /**
         * Opens an existing event with the specified name 'name' or creates a 
         * new named event if no such event exists.
         *
         * Windows: If an existing event is opened, the 'isManualReset' and 
         * 'isInitiallySignaled' parameters are ignored as these are defined by
         * the original creator of the event.
         *
         * Linux: The user must ensure that the 'isManualReset' flag is set
         * correctly when opening an existing event to match the value of the
         * original creator. The behaviour of the event is undefined if this
         * condition is not met.
         *
         * The default ctor creates an auto-reset event, which is initially
         * non-signaled.
         *
         * @param name                The name of the event.
         * @param isManualReset       Make the event a manual-reset event. If 
         *                            the flag is not set, an auto-reset event 
         *                            is created.
         * @param isInitiallySignaled Specifies whether the event is initially
         *                            signaled.
         * @param outIsNew            If not nullptr, the ctor returns whether
         *                            the semaphore was created (true) or opened
         *                            (false).
         */
        event(const char *name, const bool isManualReset = false,
            const bool isInitiallySignaled = false, bool *outIsNew = nullptr);

        /**
         * Opens an existing event with the specified name 'name' or creates a 
         * new named event if no such event exists.
         *
         * Windows: If an existing event is opened, the 'isManualReset' and 
         * 'isInitiallySignaled' parameters are ignored as these are defined by
         * the original creator of the event.
         *
         * Linux: The user must ensure that the 'isManualReset' flag is set
         * correctly when opening an existing event to match the value of the
         * original creator. The behaviour of the event is undefined if this
         * condition is not met.
         *
         * The default ctor creates an auto-reset event, which is initially
         * non-signaled.
         *
         * @param name                The name of the event.
         * @param isManualReset       Make the event a manual-reset event. If 
         *                            the flag is not set, an auto-reset event 
         *                            is created.
         * @param isInitiallySignaled Specifies whether the event is initially
         *                            signaled.
         * @param outIsNew            If not nullptr, the ctor returns whether
         *                            the semaphore was created (true) or opened
         *                            (false).
         */
        event(const wchar_t *name, const bool isManualReset = false,
            const bool isInitiallySignaled = false, bool *outIsNew = nullptr);

        /** Dtor. */
        virtual ~event(void);

        /**
         * Resets the event to non-signaled state.
         * 
         * It is safe to reset an already non-signaled event. The operation has 
         * no effect in this case.
         *
         * @throws SystemException If the operation failed.
         */
        void reset(void);

        /**
         * Sets the event to signaled state.
         *
         * It is safe to set an already signaled event. The operation has no
         * effect in this case.
         *
         * @throws SystemException If the operation failed.
         */
        void set(void);

        /**
         * Wait for the event to become signaled.
         *
         * @param timeout The timeout for the wait operation in milliseconds.
         *
         * @return true, if the event was signaled, false, if the operation
         *         timed out.
         * 
         * @throws SystemException If the operation failed.
         */
        bool wait(const timeout_type timeout = timeout_infinite);

    protected:

        /** Super class typedef. */
        typedef the::not_copyable base;

#if defined(THE_WINDOWS)
        /** Handle to the event object. */
        HANDLE handle;

#else /* defined(THE_WINDOWS) */
        /** Remember whether the event is a manual reset event. */
        bool isManualReset;

        /** The semaphore used to emulate the event. */
        semaphore sem;

#endif /* defined(THE_WINDOWS) */

    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_EVENT_H_INCLUDED */
