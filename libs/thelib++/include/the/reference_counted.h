/*
 * include\the\reference_counted.h
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
 * ReferenceCounted.h
 *
 * Copyright (C) 2006 - 2008 by Universitaet Stuttgart (VIS). 
 * Alle Rechte vorbehalten.
 * Copyright (C) 2008 by Christoph Mueller. Alle Rechte vorbehalten.
 */


#ifndef THE_REFERENCE_COUNTED_H_INCLUDED
#define THE_REFERENCE_COUNTED_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include <atomic>

#include "the/assert.h"
#include "the/stack_trace.h"
#include "the/single_allocator.h"


namespace the {

    /**
     * This class adds a reference counting mechanism to a class.
     *
     * Objects that inherit from reference_counted must always be allocated on
     * the heap the specified allocator 'A'. Usually, one would use
     * the::single_allocator as allocator, which uses the C++ new/delete
     * operator pair.
     *
     * The reference count of a newly created object is 1.
     *
     * Reference counting is thread-safe by means of atomic operations.
     *
     * @tparam T The derived type.
     * @tparam A The allocator that the object uses to destroy itself once the
     *           reference count reaches zero. This defaults to 
     *           the::single_allocator<T>.
     */
    template<class T, class A = the::single_allocator<T> >
    class reference_counted {

    public:

        /** The type of the allocator used for releasing the object. */
        typedef A allocator_type;

        /** The type that is used to store the counter. */
        typedef unsigned int counter_type;

        /**
         * Increment the reference count.
         *
         * @return The new value of the reference counter.
         */
        counter_type add_ref(void);

        /**
         * Decrement the reference count.
         *
         * If the reference count reaches zero, the object is released using 
         * the given allocator A.
         *
         * @return The new value of the reference counter.
         */
        counter_type release(void);

    protected:

        /**
         * Initialises a new instance.
         */
        reference_counted(void);

        /**
         * Create a clone of 'rhs'.
         *
         * Note that cloning does not change the reference count of an
         * object.
         *
         * @param rhs The object to be cloned.
         */
        reference_counted(const reference_counted& rhs);

        /**
         * Dtor.
         *
         * Making the dtor protected prevents explicit deletion of objects using
         * delete and creation of objects on the stack to a certain extent (e.g.
         * as long child classes keep the dtor explicitly protected).
         */
        virtual ~reference_counted(void);

        /**
         * Assign values of 'rhs' to this object.
         *
         * Note that assignment does not change the reference count of an
         * object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        reference_counted& operator =(const reference_counted& rhs);

    private:

        /** The current reference count. */
        std::atomic<counter_type> cntRefs;

    };

} /* end namespace the */


/*
 * the::reference_counted<A>::add_ref
 */
template<class T, class A> typename the::reference_counted<T, A>::counter_type
the::reference_counted<T, A>::add_ref(void) {
    THE_STACK_TRACE;
    //VLTRACE(Trace::LEVEL_VL_ANNOYINGLY_VERBOSE, "Add reference to 0x%p, "
    //    "reference count is now %u.\n", this, this->cntRefs + 1);
    return ++this->cntRefs;
}


/*
 * the::reference_counted<A>::release
 */
template<class T, class A> typename the::reference_counted<T, A>::counter_type
the::reference_counted<T, A>::release(void) {
    THE_STACK_TRACE;
    counter_type retval = --this->cntRefs;
    //VLTRACE(Trace::LEVEL_VL_ANNOYINGLY_VERBOSE, "Released object 0x%p, "
    //    "reference count is now %u.\n", this, this->cntRefs);
    if (this->cntRefs == 0) {
        // TODO: hugly
        T *that = dynamic_cast<T *>(this);
        allocator_type::deallocate(that);
    }
    return retval;
}


/*
 * the::reference_counted<T, A>::reference_counted
 */
template<class T, class A>
the::reference_counted<T, A>::reference_counted(void) {
    THE_STACK_TRACE;
    this->cntRefs = 1;
}


/*
 * the::reference_counted<T, A>::reference_counted
 */
template<class T, class A>
the::reference_counted<T, A>::reference_counted(const reference_counted& rhs) {
    THE_STACK_TRACE;
    this->cntRefs = 1;
}


/*
 * the::reference_counted<T, A>::~reference_counted
 */
template<class T, class A>
the::reference_counted<T, A>::~reference_counted(void) {
    THE_STACK_TRACE;
    THE_ASSERT(this->cntRefs == 0);
}


/*
 * the::reference_counted<T, A>::operator =
 */
template<class T, class A>
the::reference_counted<T, A>& the::reference_counted<T, A>::operator =(
        const reference_counted& rhs) {
    THE_STACK_TRACE;
    // Nothing to be done! No not modify the reference count!
    return *this;
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_REFERENCE_COUNTED_H_INCLUDED */
