/*
 * include\the\collections\enumerator_base.h
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

#ifndef THE_COLLECTIONS_ENUMERATOR_BASE_H_INCLUDED
#define THE_COLLECTIONS_ENUMERATOR_BASE_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/assert.h"
#include "the/stack_trace.h"


namespace the {
namespace collections {

    /**
     * The base class for enumerators.
     *
     * @tparam T The collection type the enumerator is intended for. The
     *           collection must expose the following typedefs that are
     *           used by the enumerator:
     *           typedef const <type> *const_pointer_type;
     *           typedef const <type>& const_reference_type;
     *           typedef <type> lock_type;
     *           typedef <type> *pointer_type;
     *           typedef <type>& reference_type;
     *           typedef <type> value_type;
     */
    template<class T> class enumerator_base {

    public:

        /** The type of collection being enumerated. */
        typedef T collection_type;

        /** A constant pointer to the type in the collection. */
        typedef typename T::const_pointer_type *const_pointer_type;

        /** A constant reference to the type in the collection. */
        typedef typename T::const_reference_type& const_reference_type;

        /** The type of lock used in the collection. */
        typedef typename T::lock_type lock_type;

        /** A pointer to the type in the collection. */
        typedef typename T::pointer_type *pointer_type;

        /** A reference to the type in the collection. */
        typedef typename T::reference_type& reference_type;

        /** The type in the collection. */
        typedef typename T::value_type value_type;

        //std::list<int>::iterator

        /** Dtor. */
        virtual ~enumerator_base(void);

        /**
         * Answer whether the enumerated collection contains at least one more 
         * element.
         *
         * Implementation note: Subclasses MUST call unlock_collection() if the
         * method returns false (at least for the first time).
         *
         * @return true if the collection contains more elements and next() can 
         *         be safely called, false otherwise.
         */
        virtual bool has_next(void) = 0;

        /**
         * Return the next element in the enumeration.
         *
         * @return The next element.
         */
        virtual reference_type next(void) = 0;

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        enumerator_base& operator =(const enumerator_base& rhs);

    protected:

        /**
         * Initialises a new instance.
         */
        enumerator_base(collection_type *collection);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        inline enumerator_base(const enumerator_base& rhs) : collection(nullptr) {
            THE_STACK_TRACE;
            *this = rhs;
        }

        /**
         * Gets the collection being enumerated.
         *
         * @return The collection being enumerated.
         */
        inline collection_type *get_collection(void) {
            THE_STACK_TRACE;
            return this->collection;
        }

        /**
         * Checks whether the enumerator has a collection and, if so, locks it.
         */
        void lock_collection(void);

        /**
         * Checks whether the enumerator still holds a collection and, if so,
         * unlocks it.
         *
         * Implementation note: Unlocking the collection will remove it from the
         * enumerator. This ensures that the dtor of the enumerator will not
         * unlock the collection for a second time in case the enumerator
         * reached its end.
         */
        void unlock_collection(void);

    private:

        /** The collection being enumerated. */
        collection_type *collection;
    };

} /* end namespace collections */
} /* end namespace the */


/*
 * the::collections::enumerator_base<T>::~enumerator_base
 */
template<class T>
the::collections::enumerator_base<T>::~enumerator_base(void) {
    THE_STACK_TRACE;
    this->unlock_collection();
}


/*
 * the::collections::enumerator_base<T>::operator =
 */
template<class T> the::collections::enumerator_base<T>&
the::collections::enumerator_base<T>::operator =(const enumerator_base& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->unlock_collection();
        this->collection = rhs.collection;
        this->lock_collection();
    }
    return *this;
}


/*
 * the::collections::enumerator_base<T>::enumerator_base
 */
template<class T> the::collections::enumerator_base<T>::enumerator_base(
        collection_type *collection) : collection(collection) {
    THE_STACK_TRACE;
    this->lock_collection();
}


/*
 * the::collections::enumerator_base<T>::lock_collection
 */
template<class T>
void the::collections::enumerator_base<T>::lock_collection(void) {
    THE_STACK_TRACE;
    if (this->collection != nullptr) {
        this->collection->lock();
    }
}


/*
 * the::collections::enumerator_base<T>::unlock_collection
 */
template<class T>
void the::collections::enumerator_base<T>::unlock_collection(void) {
    THE_STACK_TRACE;
    if (this->collection != nullptr) {
        collection_type *c = this->collection;
        this->collection = nullptr;
        c->unlock();
    }
}

#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_ENUMERATOR_BASE_H_INCLUDED */
