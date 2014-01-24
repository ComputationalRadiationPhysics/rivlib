/*
 * include\the\event_args.h
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

#ifndef THE_EVENT_ARGS_H_INCLUDED
#define THE_EVENT_ARGS_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/event_args.h"
#include "the/observable.h"
#include "the/stack_trace.h"

#include "the/system/threading/critical_section.h"
#include "the/system/threading/no_lock.h"



namespace the {

    /**
     * Base class for event arguments being produced by a the::observable.
     *
     * The intended use is to derive customised classes from this one, e.g.
     * class my_event_args : public event_args<my_event_args> in order to
     * transport meaningful information. Classes that provide these events
     * should be derived from my_event_args::observable then, which adds a
     * list of thread-safe listeners to the producer of the events.
     *
     * Classes that provide events of this type should inherit publicly from
     * my_event_args::observable_type. There is also a
     * my_event_args::unsafe_observable_type which can be used to derive an
     * observable that does not provide thread-safety.
     *
     * @tparam T The class inheriting from this one.
     * @tparam L The type of lock used for the definition of the thread-safe
     *           observable. This parameter defaults to a critical_section. It
     *           is unreasonable passing a non-lock here.
     */
    template<class T, class L =  system::threading::critical_section>
    class event_args {

    public:

        /** Type of lock used for the thread-safe obserable. */
        typedef L lock_type;

        /** Type of non-lock. */
        typedef system::threading::no_lock no_lock_type;

        /**
         * Type of the delegate handling events raised by a derived class
         * of event_args::observable.
         */
        typedef typename the::observable<T, lock_type>::event_handler_type
            event_handler_type;

        /** Type of a class raising events in a thread-safe manner. */
        typedef the::observable<T, lock_type> observable_type;

        /**
         * Type of the delegate handling events raised by a derived class
         * of event_args::unsafe_observable.
         */
        typedef typename the::observable<T, no_lock_type>::event_handler_type
            unsafe_event_handler_type;

        /** Type of a class raising events without locking the listener list. */
        typedef the::observable<T, no_lock_type> unsafe_observable_type;

        /** An empty instance of event_args. */
        static const event_args empty;

        /**
         * Initialises a new instance.
         */
        event_args(void);

        /** Dtor. */
        virtual ~event_args(void);

    };

} /* end namespace the */


/*
 * the::event_args<T, L>::empty
 */
template<class T, class L>
const the::event_args<T, L> the::event_args<T, L>::empty;


/*
 * the::event_args<T, L>::event_args
 */
template<class T, class L> the::event_args<T, L>::event_args(void) {
    THE_STACK_TRACE;
}


/*
 * the::event_args<T, L>::~event_args
 */
template<class T, class L> the::event_args<T, L>::~event_args(void) {
    THE_STACK_TRACE;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_EVENT_ARGS_H_INCLUDED */
