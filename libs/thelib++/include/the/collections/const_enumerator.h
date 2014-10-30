/*
 * include\the\collections\const_enumerator.h
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

#ifndef THE_COLLECTIONS_CONST_ENUMERATOR_H_INCLUDED
#define THE_COLLECTIONS_CONST_ENUMERATOR_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */
#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(push, off)
#endif /* defined(_WIN32) && defined(_MANAGED) */

#include "the/config.h"

#include "the/collections/enumerator_base.h"
#include "the/stack_trace.h"


namespace the {
namespace collections {

    /**
     * An enumerator wrapper for constant collections.
     *
     * @tparam The non-constant enumerator to be wrapped.
     */
    template<class T> class const_enumerator {

    public:

        /** The type of collection being enumerated. */
        typedef typename T::collection_type collection_type;

        /** A constant pointer to the type in the collection. */
        typedef const typename T::const_pointer_type *const_pointer_type;

        /** A constant reference to the type in the collection. */
        typedef const typename T::const_reference_type& const_reference_type;

        /** The non-constant enumerator type being wrapped. */
        typedef T enumerator_type;

        /** The type of lock used in the collection. */
        typedef typename T::lock_type lock_type;

        /** A pointer to the type in the collection. */
        typedef typename T::pointer_type *pointer_type;

        /** A reference to the type in the collection. */
        typedef typename T::reference_type& reference_type;

        /** The type in the collection. */
        typedef typename T::value_type value_type;

        /**
         * Initialises a new instance.
         */
        const_enumerator(void);

        /**
         * Initialise an enumerator enumerating 'collection'.
         *
         * @param enumerator The enumerator being wrapped.
         */
        explicit const_enumerator(const enumerator_type& enumerator);

        /**
         * Create a clone of 'rhs'.
         *
         * @param rhs The object to be cloned.
         */
        const_enumerator(const const_enumerator& rhs);

        /** Dtor. */
        virtual ~const_enumerator(void);

        /**
         * Answer whether the enumerated collection contains at least one
         * more element.
         *
         * @return true if the collection contains more elements and next()
         *         can be safely called, false otherwise.
         */
        inline bool has_next(void) const {
            THE_STACK_TRACE;
            return this->enumerator.has_next();
        }

        /**
         * Return the next element in the enumeration.
         *
         * @return The next element.
         */
        inline const_reference_type next(void) {
            THE_STACK_TRACE;
            return this->enumerator.next();
        }

        /**
         * Assign values of 'rhs' to this object.
         *
         * @param rhs The right hand side operand.
         *
         * @return *this.
         */
        const_enumerator& operator =(const const_enumerator& rhs);

    protected:

        /** The enumerator of the collection which does the real work. */
        mutable enumerator_type enumerator;
    };

} /* end namespace collections */
} /* end namespace the */


/*
 * the::collections::const_enumerator<T>::const_enumerator
 */
template<class T>
the::collections::const_enumerator<T>::const_enumerator(void) {
    THE_STACK_TRACE;
}


/*
 * the::collections::const_enumerator<T>::const_enumerator
 */
template<class T> the::collections::const_enumerator<T>::const_enumerator(
        const enumerator_type& enumerator) : enumerator(enumerator) {
    THE_STACK_TRACE;
}


/*
 * the::collections::const_enumerator<T>::const_enumerator
 */
template<class T> the::collections::const_enumerator<T>::const_enumerator(
        const const_enumerator& rhs) : enumerator(rhs.enumerator) {
    THE_STACK_TRACE;
}


/*
 * the::collections::const_enumerator<T>::~const_enumerator
 */
template<class T>
the::collections::const_enumerator<T>::~const_enumerator(void) {
    THE_STACK_TRACE;
}


/*
 * the::collections::const_enumerator<T>::operator =
 */
template<class T> the::collections::const_enumerator<T>&
the::collections::const_enumerator<T>::operator =(const const_enumerator& rhs) {
    THE_STACK_TRACE;
    if (this != &rhs) {
        this->enumerator = rhs.enumerator;
    }
    return *this;
}


#if defined(_WIN32) && defined(_MANAGED)
#pragma managed(pop)
#endif /* defined(_WIN32) && defined(_MANAGED) */
#endif /* THE_COLLECTIONS_CONST_ENUMERATOR_H_INCLUDED */
