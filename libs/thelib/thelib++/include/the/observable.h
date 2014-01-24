/*
 * include\the\observable.h
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

#ifndef THE_OBSERVABLE_H_INCLUDED
#define THE_OBSERVABLE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/assert.h"
#include "the/delegate.h"
#include "the/stack_trace.h"
#include "the/trace.h"

#include "the/collections/fast_forward_list.h"

#include "the/system/threading/auto_lock.h"
#include "the/system/threading/critical_section.h"


#include "the/not_implemented_exception.h"


namespace the {

    /**
     * This class implements the base for classes that can raise events and
     * notify one or more registered listeners.
     *
     * @tparam T The type of the event arguments that is passed to the listeners
     *           if the event is raised.
     * @tparam L The type of lock that is used to protect the list of listeners
     *           from concurrent accesses. This defaults to 
     *           the::system::threading::critical_section.
     */
    template<class T, class L = system::threading::critical_section>
    class observable {

    public:

        /**
         * The type of event arguments that are passed to registered listeners
         * when the event is raised.
         */
        typedef T event_args_type;

        /** The type of the delegate that handles the event. */
        typedef delegate<void, observable&, event_args_type&>
            event_handler_type;

        //typedef multicast_delegate<observable, event_args_type> observer_list_type;

        /**
         * Add a new observer that is invoked if the observable raises an event.
         *
         * If 'observer' has already been added to the list, the operation has
         * no effect. The standard comparison operator ==() of
         * event_handler_type is used to check the equality of the parameter and
         * any existing observer.
         *
         * If L is an actual lock, the operation is thread-safe.
         *
         * @param observer The observer to be added.
         */
        void add_observer(const event_handler_type& observer);

        void remove_observer(const event_handler_type& observer);

        /** Dtor. */
        virtual ~observable(void);

    protected:

        //typedef collections::fast_forward_list<

        /**
         * Initialises a new instance.
         */
        observable(void);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        observable(const observable& rhs);

        virtual void notify(event_args_type& eventArgs) throw();

        virtual void on_notified(const event_handler_type& handler,
            event_args_type& eventArgs) throw();

        virtual bool on_notifying(const event_handler_type& handler,
            event_args_type& eventArgs) throw();

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        observable& operator =(const observable& rhs);

    private:

        /**
         * The type of lock used to protect the list of listeners from
         * concurrent accesses.
         */
        typedef L lock_type;

        /** The type of the collection that stores the observers. */
        typedef collections::fast_forward_list<event_handler_type, lock_type>
            observer_list_type;

        ///** A scoped lock for 'lock_type'. */
        //typedef system::threading::auto_lock<lock_type> auto_lock_type;

        /** The list of registered observers. */
        observer_list_type observers;

        ///** The lock that protects the list of listeners. */
        //mutable lock_type syncRoot;

    };

} /* end namespace the */


/*
 * the::observable<T, L>::~observable
 */
template<class T, class L> the::observable<T, L>::~observable(void) {
    THE_STACK_TRACE;
}


/*
 * the::observable<T, L>::add_observer
 */
template<class T, class L>
void the::observable<T, L>::add_observer(const event_handler_type& observer) {
    THE_STACK_TRACE;
    this->observers.lock();
    if (!this->observers.contains(observer)) {
        this->observers.add(observer);
    }
    this->observers.unlock();
}


/*
 * the::observable<T, L>::remove_observer
 */
template<class T, class L>
void the::observable<T, L>::remove_observer(const event_handler_type& observer) {
    THE_STACK_TRACE;
#ifdef THE_DEBUG
    this->observers.lock();
#endif /* THE_DEBUG */
    this->observers.remove(observer);
#ifdef THE_DEBUG
    THE_ASSERT(!this->observers.contains(observer));
    this->observers.unlock();
#endif /* THE_DEBUG */
}


/*
 * the::observable<T, L>::observable
 */
template<class T, class L> the::observable<T, L>::observable(void) {
    THE_STACK_TRACE;
}


/*
 * the::observable<T, L>::observable
 */
template<class T, class L>
the::observable<T, L>::observable(const observable& rhs) {
    THE_STACK_TRACE;
    *this = rhs;
}


/*
 * the::observable<T, L>::notify
 */
template<class T, class L>
void the::observable<T, L>::notify(event_args_type& eventArgs) throw() {
    THE_STACK_TRACE;
    typename observer_list_type::enumerator it = this->observers.get_enumerator();
    while (it.has_next()) {
        try {
            typename observer_list_type::reference_type i = it.next();
            if (this->on_notifying(i, eventArgs)) {
                i(*this, eventArgs);
                this->on_notified(i, eventArgs);
            }
        } catch (...) {
            THE_TRACE(THE_TRCCHL_THELIB_COMMON, THE_TRCLVL_WARN, "Notifying an "
                "observer failed because the handler raised an exception. The "
                "observable will continue notifying the other listeners.");
        }
    }
}


/*
 * the::observable<T, L>::on_notified
 */
template<class T, class L>
void the::observable<T, L>::on_notified(const event_handler_type& handler,
        event_args_type& eventArgs) throw() {
    THE_STACK_TRACE;
}


/*
 * the::observable<T, L>::on_notifying
 */
template<class T, class L>
bool the::observable<T, L>::on_notifying(const event_handler_type& handler,
        event_args_type& eventArgs)  throw() {
    THE_STACK_TRACE;
    return true;
}


/*
 * the::observable<T, L>::operator =
 */
template<class T, class L>
the::observable<T, L>& the::observable<T, L>::operator =(
        const observable& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        //auto_lock_type(this->syncRoot);
        this->observers = rhs.observers;
    }
    return *this;
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_OBSERVABLE_H_INCLUDED */
