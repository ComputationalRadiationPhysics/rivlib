/*
 * the/system/threading/thread_local_ptr.h
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

#ifndef THE_SYSTEM_THREADING_THREAD_LOCAL_PTR_H_INCLUDED
#define THE_SYSTEM_THREADING_THREAD_LOCAL_PTR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <memory>

#include "the/system/threading/thread_local.h"

#include "the/not_copyable.h"
#include "the/null_pointer_exception.h"
#include "the/single_allocator.h"
#include "the/stack_trace.h"


namespace the {
namespace system {
namespace threading {


    /**
     * This class managed a pointer stored in thread-local storage.
     *
     * The class is a unique smart pointer, i.e. it takes ownership of the 
     * memory designated by the pointer and will release it if the pointer 
     * variable is overwritten or destroyed.
     *
     * The class will use the specified allocator to release the memory. The 
     * user is responsible for only passing memory to the class that is 
     * compatible with the allocator.
     *
     * For an unmanaged TLS pointer, use thread_local<T *>.
     */
    template<class T, class A = the::single_allocator<T> > class thread_local_ptr
            : not_copyable {

    public:

        /** The constant type of the pointer that is stored in TLS. */
        typedef const typename A::target_ptr_type const_pointer_type;

        /** 
         * A constant reference to the defererenced type of the pointer that is 
         * stored in TLS.
         */
        typedef const typename A::target_type& const_reference_type;

        /** The type of the pointer that is stored in TLS. */
        typedef typename A::target_ptr_type pointer_type;

        /** 
         * A reference to the defererenced type of the pointer that is 
         * stored in TLS.
         */
        typedef typename A::target_type reference_type;

        /** The deferenced type of the pointer_type stored in TLS. */
        typedef typename A::target_type value_type;

        /**
         * Initialises a new instance with the specified pointer.
         *
         * The object takes ownership of 'value'.
         *
         * The memory designated by 'value' must have been allocated with the
         * allocator A specified in the template parameter list.
         *
         * @param value The initial value of the variable. The object takes 
         *              ownership of the parameter and will release it using
         *              A::deallocate(). This parameter defaults to NULL.
         *
         * @throws the::system::system_exception In case that the TLS could
         *                                       not be allocated.
         */
        thread_local_ptr(pointer_type value = NULL);

        /**
         * Dtor.
         */
        ~thread_local_ptr(void);

        /**
         * Get the current value of the thread-local pointer.
         *
         * The object will remain owner of the memory.
         *
         * @return The current value of the thread-local pointer.
         */
        inline const_pointer_type get(void) const {
            THE_STACK_TRACE;
            return static_cast<const_pointer_type>(this->tls);
        }

        /**
         * Get the current value of the thread-local pointer.
         *
         * The object will remain owner of the memory.
         *
         * @return The current value of the thread-local pointer.
         */
        inline pointer_type get(void) {
            THE_STACK_TRACE;
            return static_cast<pointer_type>(this->tls);
        }

        /**
         * Dereferences the current value of the thread-local pointer.
         *
         * @return The object designated by the thread-local pointer.
         *
         * @throws the::null_pointer_exception If the thread-local pointer
         *                                     is NULL.
         */
        const_reference_type operator *(void) const;

        /**
         * Dereferences the current value of the thread-local pointer.
         *
         * @return The object designated by the thread-local pointer.
         *
         * @throws the::null_pointer_exception If the thread-local pointer
         *                                     is NULL.
         */
        reference_type operator *(void);

        /**
         * Get the current value of the thread-local pointer.
         *
         * The object will remain owner of the memory.
         *
         * @return The current value of the thread-local pointer.
         */
        inline const_pointer_type operator ->(void) const {
            THE_STACK_TRACE;
            return static_cast<const_pointer_type>(this->tls);
        }

        /**
         * Get the current value of the thread-local pointer.
         *
         * The object will remain owner of the memory.
         *
         * @return The current value of the thread-local pointer.
         */
        inline pointer_type operator ->(void) {
            THE_STACK_TRACE;
            return static_cast<pointer_type>(this->tls);
        }

        /**
         * Assigns a new value to the thread-local variable.
         *
         * The object takes ownership of 'value'.
         *
         * The memory designated by 'value' must have been allocated with the
         * allocator A specified in the template parameter list.
         *
         * @param value The thread-local pointer. The object takes ownership 
         *              of the parameter and will release it using 
         *              A::deallocate().
         *
         * @throws the::system_exception In case the TLS index could not be
         *                               acquired lazily or in case that the
         *                               value could not be written.
         */
        thread_local_ptr& operator =(pointer_type value);

    protected:

        /** Super class typedef. */
        typedef not_copyable base;

    private:

        /** The underlying thread-local storage. */
        thread_local<pointer_type> tls;

    };

} /* end namespace threading */
} /* end namespace system */
} /* end namespace the */


/*
 * the::system::threading::thread_local_ptr::thread_local_ptr
 */
template<class T, class A>
the::system::threading::thread_local_ptr<T, A>::thread_local_ptr(
        pointer_type value) {
    THE_STACK_TRACE;
    this->tls = value;
}


/*
 * the::system::threading::thread_local_ptr::~thread_local_ptr
 */
template<class T, class A>
the::system::threading::thread_local_ptr<T, A>::~thread_local_ptr(void) {
    THE_STACK_TRACE;
    *this = NULL;
}


/*
 * the::system::threading::thread_local_ptr<T, A>::operator *
 */
template<class T, class A>
typename the::system::threading::thread_local_ptr<T, A>::const_reference_type 
the::system::threading::thread_local_ptr<T, A>::operator *(void) const {
    THE_STACK_TRACE;
    const_pointer_type value = this->get();
    if (value == NULL) {
        throw null_pointer_exception(__FILE__, __LINE__);
    }
    return *value;
}


/*
 * the::system::threading::thread_local_ptr<T, A>::operator *
 */
template<class T, class A>
typename the::system::threading::thread_local_ptr<T, A>::reference_type 
the::system::threading::thread_local_ptr<T, A>::operator *(void) {
    THE_STACK_TRACE;
    pointer_type value = this->get();
    if (value == NULL) {
        throw null_pointer_exception(__FILE__, __LINE__);
    }
    return *value;
}


/*
 * the::system::threading::thread_local_ptr<T, A>::operator =
 */
template<class T, class A>
the::system::threading::thread_local_ptr<T, A>& 
the::system::threading::thread_local_ptr<T, A>::operator =(pointer_type value) {
    THE_STACK_TRACE;
    pointer_type old = this->get();
    this->tls = value;
    if (old != NULL) {
        A::deallocate(old);
    }
    return *this;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_SYSTEM_THREADING_THREAD_LOCAL_PTR_H_INCLUDED */
